#include "word_counter.h"
#include "word_counter_thread.h"

WordCounter::WordCounter(QObject *parent) : QObject(parent) { }

WordCounter::~WordCounter() {
    if ((m_thread != nullptr) && (!m_thread->isFinished())) {
        m_thread->requestInterruption();
        m_thread->wait();
    }
}

auto WordCounter::status() const -> Status {
    return m_status;
}

auto WordCounter::progressMin() const -> qint64 {
    return m_progress_min;
}

auto WordCounter::progressVal() const -> qint64 {
    return m_progress_val;
}

auto WordCounter::progressMax() const -> qint64 {
    return m_progress_max;
}

auto WordCounter::error() const -> WordCounterError {
    return m_error;
}

auto WordCounter::errorString() const -> QString {
    return m_error_string;
}

auto WordCounter::fileName() const -> QString {
    return m_file_name;
}

auto WordCounter::result(int count) const -> WordCounterResult {
    if (m_thread != nullptr) {
        return m_thread->result(count);
    }
    return {};
}

void WordCounter::process(const QString &file_name) {
    if ((m_thread != nullptr) && !m_thread->isFinished()) {
        qCritical("another file processing already running");
        return;
    }

    setFilename(file_name);

    m_thread = std::make_unique<WordCounterThread>(file_name);

    connect(m_thread.get(), &WordCounterThread::started, this, [this] {
        setStatus(Status::Running);
    });

    connect(m_thread.get(),
            &WordCounterThread::progressRangeChanged,
            this,
            &WordCounter::setProgressRange);
    connect(m_thread.get(),
            &WordCounterThread::progressValueChanged,
            this,
            &WordCounter::setProgressValue);
    connect(m_thread.get(),
            &WordCounterThread::resultError,
            this,
            [this](WordCounterError err, const QString &msg) {
                setError(err, msg);
                setStatus(Status::Error);
            });
    connect(m_thread.get(), &WordCounterThread::resultReady, this, [this] {
        m_thread->wait();
        setStatus(Status::Finished);
    });

    m_thread->start();
}

void WordCounter::process(const QUrl &file_url) {
    process(file_url.toLocalFile());
}

void WordCounter::cancel() {
    if ((m_thread == nullptr) || !m_thread->isRunning()) {
        return;
    }

    m_thread->requestInterruption();
    m_thread->wait();

    setStatus(Status::Canceled);
}

void WordCounter::reset() {
    if ((m_thread != nullptr) && !m_thread->isFinished()) {
        m_thread->requestInterruption();
        m_thread->wait();

        m_thread.reset();
    }

    setStatus(Status::None);
    setError(WordCounterError::NoError);
    setFilename({});
    setProgressRange(0LL, 0LL);
    setProgressValue(0LL);
}

void WordCounter::setStatus(Status status) {
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void WordCounter::setProgressRange(qint64 progress_min, qint64 progress_max) {
    const auto &[min, max] = std::minmax(progress_min, progress_max);

    if (m_progress_min != min) {
        m_progress_min = min;
        emit progressMinChanged();
    }

    if (m_progress_max != max) {
        m_progress_max = max;
        emit progressMaxChanged();
    }

    setProgressValue(std::clamp(m_progress_val, m_progress_min, m_progress_max));
}

void WordCounter::setProgressValue(qint64 progress_val) {
    const auto &val = std::clamp(progress_val, m_progress_min, m_progress_max);

    if (m_progress_val != val) {
        m_progress_val = val;
        emit progressValChanged();
    }
}

void WordCounter::setError(WordCounterError error, const QString &error_string) {
    if ((m_error == error) && (m_error_string == error_string)) {
        return;
    }

    m_error        = error;
    m_error_string = error_string;

    emit errorChanged();
}

void WordCounter::setFilename(const QString &filename) {
    if (m_file_name != filename) {
        m_file_name = filename;
        emit fileNameChanged();
    }
}
