#pragma once

#include <QAbstractListModel>
#include <QUrl>

#include "word_counter.h"

class WordCounterModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(WordCounter *counter READ counter CONSTANT FINAL)

public:
    enum DataRole {
        WordRole = Qt::UserRole,
        WordCountRole,
        WordCountFractionRole,
    };

    Q_ENUM(DataRole)

public:
    explicit WordCounterModel(QObject *parent = nullptr);

public:
    auto data(const QModelIndex &index, int role) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;
    auto rowCount(const QModelIndex &parent) const -> int override;

public slots:
    void openFile(const QUrl &file_url);
    void closeFile();

private:
    auto counter() -> WordCounter *;

    void updateResult();
    void clearResult();

private:
    void timerEvent(QTimerEvent *event) override;

private:
    WordCounter       m_counter;
    WordCounterResult m_result;

    quint32 m_max_count = 0U;

    int  m_update_timer  = -1;
    bool m_update_queued = false;
};
