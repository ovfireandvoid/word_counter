#include "word_counter_model.h"

#include <QTimerEvent>

#include "word_counter.h"

WordCounterModel::WordCounterModel(QObject *parent) : QAbstractListModel(parent) { }

auto WordCounterModel::counter() -> WordCounter * {
    return m_counter;
}

void WordCounterModel::setCounter(WordCounter *counter) {
    if (m_counter == counter) {
        return;
    }

    if (m_counter != nullptr) {
        m_counter->disconnect(this);
    }

    m_counter = counter;

    if (m_counter) {
        connect(m_counter, &WordCounter::statusChanged, this, &WordCounterModel::fetchCounterStatus);
        fetchCounterStatus();
    } else {
        resetCounterResult();
    }

    emit counterChanged();
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
        role_data = m_max_count != 0U
                        ? QVariant::fromValue(static_cast<double>(m_result.at(index.row()).second) /
                                              static_cast<double>(m_max_count))
                        : QVariant::fromValue(0.0);
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

void WordCounterModel::update() {
    fetchCounterResult();
}

void WordCounterModel::fetchCounterStatus() {
    if (m_counter == nullptr) {
        return;
    }

    switch (m_counter->status()) {
    case WordCounter::Status::None:
    case WordCounter::Status::Canceled:
    case WordCounter::Status::Error:
        resetCounterResult();
        break;
    case WordCounter::Status::Running:
    case WordCounter::Status::Finished:
        fetchCounterResult();
        break;
    }
}

void WordCounterModel::fetchCounterResult() {
    if (m_counter == nullptr) {
        return resetCounterResult();
    }

    const auto result = m_counter->result(15);

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

void WordCounterModel::resetCounterResult() {
    if (!m_result.empty()) {
        beginRemoveRows({}, 0, m_result.size() - 1);
        m_result.clear();
        endRemoveRows();
    }
}
