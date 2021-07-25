#pragma once

#include <memory>

#include <QObject>
#include <QString>
#include <QUrl>

#include "word_counter_error.h"
#include "word_counter_result.h"

class WordCounterThread;

class WordCounter : public QObject {
    Q_OBJECT

public:
    enum class Status {
        None,

        Running,
        Canceled,
        Error,
        Finished,
    };

    Q_ENUM(Status)

private:
    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)

    Q_PROPERTY(qint64 progressMin READ progressMin NOTIFY progressMinChanged FINAL)
    Q_PROPERTY(qint64 progressVal READ progressVal NOTIFY progressValChanged FINAL)
    Q_PROPERTY(qint64 progressMax READ progressMax NOTIFY progressMaxChanged FINAL)

    Q_PROPERTY(WordCounterError error READ error NOTIFY errorChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged FINAL)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged FINAL)

public:
    explicit WordCounter(QObject *parent = nullptr);

    ~WordCounter();

    auto status() const -> Status;

    auto progressMin() const -> qint64;
    auto progressVal() const -> qint64;
    auto progressMax() const -> qint64;

    auto error() const -> WordCounterError;
    auto errorString() const -> QString;

    auto fileName() const -> QString;

    auto result(int count) const -> WordCounterResult;

    void process(const QString &file_name);
    void process(const QUrl &file_url);
    void cancel();

    void reset();

private:
    void setStatus(Status status);

    void setProgressRange(qint64 progress_min, qint64 progress_max);
    void setProgressValue(qint64 progress_val);

    void setError(WordCounterError error, const QString &error_string = QString());

    void setFilename(const QString &filename);

signals:
    void statusChanged();

    void progressMinChanged();
    void progressValChanged();
    void progressMaxChanged();

    void errorChanged();

    void fileNameChanged();

private:
    Status m_status = Status::None;

    qint64 m_progress_min = 0LL;
    qint64 m_progress_max = 0LL;
    qint64 m_progress_val = 0LL;

    WordCounterError m_error = WordCounterError::NoError;
    QString          m_error_string;

    QString m_file_name;

    std::unique_ptr<WordCounterThread> m_thread;
};
