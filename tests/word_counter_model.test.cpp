#include <QtTest>

#include "../src/models/word_counter.h"
#include "../src/models/word_counter_error.h"
#include "../src/models/word_counter_model.h"
#include "../src/models/word_counter_result.h"

class WordCounterModelTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void readNullData();
    void readTestData();

private:
    QString m_null_data_path;
    QString m_test_data_path;
};

void WordCounterModelTest::initTestCase() {
    m_null_data_path = QFINDTESTDATA("data/null_data.txt");

    if (m_null_data_path.isEmpty()) {
        QSKIP("test/null_data.txt not found");
    }

    m_test_data_path = QFINDTESTDATA("data/test_data.txt");

    if (m_test_data_path.isEmpty()) {
        QSKIP("data/test_data.txt not found");
    }
}

void WordCounterModelTest::readNullData() {
    WordCounterModel         model;
    QAbstractItemModelTester tester{&model};

    model.openFile(QUrl::fromLocalFile(m_null_data_path));

    QTest::qWait(500);

    QCOMPARE(model.rowCount({}), 0);
}

void WordCounterModelTest::readTestData() {
    WordCounterModel         model;
    QAbstractItemModelTester tester{&model};

    model.openFile(QUrl::fromLocalFile(m_test_data_path));

    QTest::qWait(500);

    QCOMPARE(model.rowCount({}), 15);

    const auto wordAt = [&model](int row) -> QString {
        return model.data(model.index(row), WordCounterModel::WordRole).toString();
    };
    const auto countAt = [&model](int row) -> quint32 {
        return model.data(model.index(row), WordCounterModel::WordCountRole).toUInt();
    };

    QCOMPARE(wordAt(0), QStringLiteral("a"));
    QCOMPARE(countAt(0), 2);
    QCOMPARE(wordAt(1), QStringLiteral("accumsan"));
    QCOMPARE(countAt(1), 2);
    QCOMPARE(wordAt(2), QStringLiteral("amet"));
    QCOMPARE(countAt(2), 2);
    QCOMPARE(wordAt(3), QStringLiteral("consectetur"));
    QCOMPARE(countAt(3), 2);
    QCOMPARE(wordAt(4), QStringLiteral("donec"));
    QCOMPARE(countAt(4), 2);
    QCOMPARE(wordAt(5), QStringLiteral("eget"));
    QCOMPARE(countAt(5), 3);
    QCOMPARE(wordAt(6), QStringLiteral("hendrerit"));
    QCOMPARE(countAt(6), 2);
    QCOMPARE(wordAt(7), QStringLiteral("in"));
    QCOMPARE(countAt(7), 3);
    QCOMPARE(wordAt(8), QStringLiteral("lectus"));
    QCOMPARE(countAt(8), 2);
    QCOMPARE(wordAt(9), QStringLiteral("leo"));
    QCOMPARE(countAt(9), 2);
    QCOMPARE(wordAt(10), QStringLiteral("magna"));
    QCOMPARE(countAt(10), 2);
    QCOMPARE(wordAt(11), QStringLiteral("mauris"));
    QCOMPARE(countAt(11), 2);
    QCOMPARE(wordAt(12), QStringLiteral("metus"));
    QCOMPARE(countAt(12), 2);
    QCOMPARE(wordAt(13), QStringLiteral("purus"));
    QCOMPARE(countAt(13), 2);
    QCOMPARE(wordAt(14), QStringLiteral("ut"));
    QCOMPARE(countAt(14), 3);
}

QTEST_MAIN(WordCounterModelTest)

#include "word_counter_model.test.moc"
