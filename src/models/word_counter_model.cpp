#include "word_counter_model.h"

#include <QTimerEvent>

WordCounterModel::WordCounterModel(QObject *parent) : QAbstractListModel(parent) {
    connect(&m_counter, &WordCounter::statusChanged, [this] {
        switch (m_counter.status()) {
        case WordCounter::Status::None:
        case WordCounter::Status::Canceled:
        case WordCounter::Status::Error:
            clearResult();
            break;
        case WordCounter::Status::Running:
            m_update_timer = startTimer(17);
            break;
        case WordCounter::Status::Finished:
            killTimer(m_update_timer);
            updateResult();
            break;
        }
    });

    connect(&m_counter, &WordCounter::progressValChanged, [this] {
        m_update_queued = m_counter.progressVal() > 0LL;
    });
}

auto WordCounterModel::data(const QModelIndex &index, int role) const -> QVariant {
    QVariant role_data;

    if (!hasIndex(index.row(), index.column(), index.parent())) {
        return role_data;
    }

    switch (role) {
    case DataRole::WordRole:
        role_data = QVariant::fromValue(m_result.at(index.row()).first);
        break;
    case DataRole::WordCountRole:
        role_data = QVariant::fromValue(m_result.at(index.row()).second);
        break;
    case DataRole::WordCountFractionRole:
        role_data = m_max_count == 0U
                        ? QVariant::fromValue(0.0)
                        : QVariant::fromValue(static_cast<double>(m_result.at(index.row()).second) /
                                              static_cast<double>(m_max_count));
        break;
    }

    return role_data;
}

auto WordCounterModel::roleNames() const -> QHash<int, QByteArray> {
    return {
        {DataRole::WordRole, QByteArrayLiteral("word")},
        {DataRole::WordCountRole, QByteArrayLiteral("wordCount")},
        {DataRole::WordCountFractionRole, QByteArrayLiteral("wordCountFraction")},
    };
}

auto WordCounterModel::rowCount(const QModelIndex &parent) const -> int {
    return !parent.isValid() ? m_result.size() : 0;
}

void WordCounterModel::openFile(const QUrl &file_url) {
    m_counter.process(file_url);
}

void WordCounterModel::closeFile() {
    m_counter.reset();
}

auto WordCounterModel::counter() -> WordCounter * {
    return &m_counter;
}

void WordCounterModel::updateResult() {
    const auto result = m_counter.result(15);

    if (m_result == result) {
        return;
    }

    if (m_result.size() > result.size()) {
        beginRemoveRows({}, result.size(), m_result.size() - 1);
        m_result.resize(result.size());
        endRemoveRows();
    }

    quint32 max_count = 0U;
    for (const auto &result_item : result) {
        max_count = std::max(max_count, result_item.second);
    }

    bool max_count_changed = false;
    if ((max_count_changed = (m_max_count != max_count))) {
        m_max_count = max_count;
    }

    if (m_result.size() < result.size()) {
        beginInsertRows({}, m_result.size(), result.size() - 1);

        for (auto row = m_result.size(); row < result.size(); ++row) {
            m_result.append(result.at(row));
        }

        endInsertRows();
    }

    QVector<int> updated_roles;

    for (auto row = 0; row < result.size(); ++row) {
        if (m_result[row].first != result[row].first) {
            updated_roles.append(DataRole::WordRole);

            m_result[row].first = result[row].first;
        }

        if (m_result[row].second != result[row].second) {
            updated_roles.append(DataRole::WordCountRole);
            updated_roles.append(DataRole::WordCountFractionRole);

            m_result[row].second = result[row].second;
        } else if (max_count_changed) {
            updated_roles.append(DataRole::WordCountFractionRole);
        }

        if (!updated_roles.empty()) {
            const auto row_index = index(row);

            emit dataChanged(row_index, row_index, updated_roles);

            updated_roles.clear();
        }
    }
}

void WordCounterModel::clearResult() {
    if (!m_result.empty()) {
        beginRemoveRows({}, 0, m_result.size() - 1);
        m_result.clear();
        endRemoveRows();
    }
}

void WordCounterModel::timerEvent(QTimerEvent *event) {
    if (m_update_queued && (m_update_timer == event->timerId())) {
        updateResult();
        m_update_queued = false;
    }
}
