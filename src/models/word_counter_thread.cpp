#include "word_counter_thread.h"

#include <algorithm>
#include <memory>

#include <QByteArray>
#include <QFile>
#include <QMutexLocker>
#include <QTextBoundaryFinder>
#include <QTextCodec>
#include <QTextDecoder>

WordCounterThread::WordCounterThread(QString file_name, QObject *parent) :
    QThread(parent),
    m_file_name(std::move(file_name)) {
    qRegisterMetaType<WordCounterError>("WordCounterError");
}

auto WordCounterThread::result(int count) const -> WordCounterResult {
    WordCounterResult res(std::max(0, count));

    if (res.empty()) {
        return res;
    }

    const auto res_end = [this, &res] {
        const QMutexLocker lock{&m_histogram_mutex};

        return std::partial_sort_copy(m_histogram.constKeyValueBegin(),
                                      m_histogram.constKeyValueEnd(),
                                      res.begin(),
                                      res.end(),
                                      [](const auto &l, const auto &r) {
                                          return l.second == r.second
                                                     ? l.first.localeAwareCompare(r.first) < 0
                                                     : l.second > r.second;
                                      });
    }();

    if (res_end != res.end()) {
        res.erase(res_end, res.end());
    }

    std::sort(res.begin(), res.end(), [](const auto &l, const auto &r) {
        return l.first.localeAwareCompare(r.first) < 0;
    });

    return res;
}

void WordCounterThread::run() {
    QFile input_file{m_file_name};

    if (!input_file.open(QFile::ExistingOnly | QFile::ReadOnly | QFile::Text)) {
        emit resultError(WordCounterError::FileOpenError, input_file.errorString());
        return;
    }

    emit progressRangeChanged(input_file.pos(), input_file.size());
    emit progressValueChanged(input_file.pos());

    QByteArray input_data{8192, '\0'};
    QString    input_line;

    std::unique_ptr<QTextDecoder> text_decoder;

    while (!isInterruptionRequested() && !input_file.atEnd()) {
        const auto input_size = static_cast<int>(
            input_file.readLine(input_data.data(), input_data.size()));

        if (input_size == -1LL) {
            emit resultError(WordCounterError::FileReadError, input_file.errorString());
            return;
        }

        if (text_decoder == nullptr) {
            text_decoder.reset(
                QTextCodec::codecForUtfText(input_data.left(input_size))->makeDecoder());
        }

        input_line = text_decoder->toUnicode(input_data.constData(), input_size);

        for (auto &ch : input_line) {
            if (ch.isUpper()) {
                ch = ch.toLower();
            }
        }

        const auto words = wordsFrom(input_line);

        {
            const QMutexLocker lock{&m_histogram_mutex};

            for (const auto &word : words) {
                if (auto it = m_histogram.find(QString::fromRawData(word.unicode(), word.size()));
                    it != m_histogram.end()) {
                    *it += 1U;
                } else {
                    m_histogram.insert(word.toString(), 1U);
                }
            }
        }

        emit progressValueChanged(input_file.pos());
    }

    if (!isInterruptionRequested()) {
        emit resultReady();
    }
}

auto WordCounterThread::wordsFrom(const QString &str) -> QVector<QStringRef> {
    QVector<QStringRef> words;
    QTextBoundaryFinder words_finder{QTextBoundaryFinder::Word, str.unicode(), str.size()};

    int word_pos = -1;
    int word_end = -1;

    while (words_finder.isAtBoundary()) {
        if (words_finder.boundaryReasons().testFlag(QTextBoundaryFinder::StartOfItem)) {
            word_pos = words_finder.position();
        }
        if (words_finder.boundaryReasons().testFlag(QTextBoundaryFinder::EndOfItem)) {
            word_end = words_finder.position();
        }

        if ((word_pos != -1) && (word_end != -1)) {
            words.append(str.midRef(word_pos, word_end - word_pos));

            word_pos = -1;
            word_end = -1;
        }

        words_finder.toNextBoundary();
    }

    return words;
}
