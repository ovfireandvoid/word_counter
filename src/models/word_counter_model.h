#pragma once

#include <QAbstractListModel>
#include <QPointer>
#include <QUrl>

#include "word_counter_result.h"

class WordCounter;

class WordCounterModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(WordCounter *counter READ counter WRITE setCounter NOTIFY counterChanged FINAL)

public:
    enum DataRole {
        WordRole = Qt::UserRole,
        WordCountRole,
        WordCountFractionRole,
    };

    Q_ENUM(DataRole)

public:
    explicit WordCounterModel(QObject *parent = nullptr);

    auto counter() -> WordCounter *;
    void setCounter(WordCounter *counter);

public:
    auto data(const QModelIndex &index, int role) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;
    auto rowCount(const QModelIndex &parent) const -> int override;

private:
    void fetchCounterStatus();

    void fetchCounterResult();
    void resetCounterResult();

    void timerEvent(QTimerEvent *event) override;

signals:
    void counterChanged();

private:
    QPointer<WordCounter> m_counter;
    WordCounterResult     m_result;

    quint32 m_max_count = 0U;

    int  m_update_timer  = -1;
    bool m_update_queued = false;
};
