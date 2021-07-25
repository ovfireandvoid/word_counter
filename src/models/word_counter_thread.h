#pragma once

#include <QHash>
#include <QMutex>
#include <QString>
#include <QStringRef>
#include <QThread>
#include <QVector>

#include "word_counter_error.h"
#include "word_counter_result.h"

class WordCounterThread : public QThread {
    Q_OBJECT

public:
    explicit WordCounterThread(QString filename, QObject *parent = nullptr);

    auto result(int count) const -> WordCounterResult;

private:
    void run() override;

    static auto wordsFrom(const QString &str) -> QVector<QStringRef>;

signals:
    void progressRangeChanged(qint64 progress_min, qint64 progress_max);
    void progressValueChanged(qint64 progress_val);

    void resultError(WordCounterError error, const QString &error_string);
    void resultReady();

private:
    QString m_file_name;

    QHash<QString, quint32> m_histogram;
    mutable QMutex          m_histogram_mutex;
};
