#include <QtTest>

#include <QFileInfo>

#include "../src/models/word_counter.h"
#include "../src/models/word_counter_error.h"
#include "../src/models/word_counter_result.h"

class WordCounterTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void readNullData();
    void readTestData();

private:
    static auto testFileSize(const QString &file_name) -> qint64 {
        return QFileInfo(file_name).size();
    }

private:
    QString m_null_data_path;
    QString m_test_data_path;
};

void WordCounterTest::initTestCase() {
    m_null_data_path = QFINDTESTDATA("data/null_data.txt");

    if (m_null_data_path.isEmpty()) {
        QSKIP("test/null_data.txt not found");
    }

    m_test_data_path = QFINDTESTDATA("data/test_data.txt");

    if (m_test_data_path.isEmpty()) {
        QSKIP("data/test_data.txt not found");
    }
}

void WordCounterTest::readNullData() {
    WordCounter counter;

    counter.process(m_null_data_path);

    QVERIFY(
        QTest::qWaitFor([&counter] { return counter.status() == WordCounter::Status::Finished; }));

    QCOMPARE(counter.status(), WordCounter::Status::Finished);
    QCOMPARE(counter.error(), WordCounterError::NoError);

    const auto file_size = testFileSize(counter.fileName());

    QCOMPARE(counter.progressMin(), 0LL);
    QCOMPARE(counter.progressVal(), file_size);
    QCOMPARE(counter.progressMax(), file_size);

    const auto result = counter.result(15);

    QVERIFY(result.empty());
}

void WordCounterTest::readTestData() {
    WordCounter counter;

    counter.process(m_test_data_path);

    QVERIFY(
        QTest::qWaitFor([&counter] { return counter.status() == WordCounter::Status::Finished; }));

    QCOMPARE(counter.status(), WordCounter::Status::Finished);
    QCOMPARE(counter.error(), WordCounterError::NoError);

    const auto file_size = testFileSize(counter.fileName());

    QCOMPARE(counter.progressMin(), 0LL);
    QCOMPARE(counter.progressVal(), file_size);
    QCOMPARE(counter.progressMax(), file_size);

    {
        const auto result = counter.result(1);

        QCOMPARE(result.size(), 1);

        QCOMPARE(result.first().first, QStringLiteral("eget"));
        QCOMPARE(result.first().second, 3);
    }

    {
        const auto result = counter.result(4);

        QCOMPARE(result.size(), 4);

        QCOMPARE(result.at(0).first, QStringLiteral("a"));
        QCOMPARE(result.at(0).second, 2);
        QCOMPARE(result.at(1).first, QStringLiteral("eget"));
        QCOMPARE(result.at(1).second, 3);
        QCOMPARE(result.at(2).first, QStringLiteral("in"));
        QCOMPARE(result.at(2).second, 3);
        QCOMPARE(result.at(3).first, QStringLiteral("ut"));
        QCOMPARE(result.at(3).second, 3);
    }

    {
        const auto result = counter.result(15);

        QCOMPARE(result.size(), 15);

        QCOMPARE(result.at(0).first, QStringLiteral("a"));
        QCOMPARE(result.at(0).second, 2);
        QCOMPARE(result.at(1).first, QStringLiteral("accumsan"));
        QCOMPARE(result.at(1).second, 2);
        QCOMPARE(result.at(2).first, QStringLiteral("amet"));
        QCOMPARE(result.at(2).second, 2);
        QCOMPARE(result.at(3).first, QStringLiteral("consectetur"));
        QCOMPARE(result.at(3).second, 2);
        QCOMPARE(result.at(4).first, QStringLiteral("donec"));
        QCOMPARE(result.at(4).second, 2);
        QCOMPARE(result.at(5).first, QStringLiteral("eget"));
        QCOMPARE(result.at(5).second, 3);
        QCOMPARE(result.at(6).first, QStringLiteral("hendrerit"));
        QCOMPARE(result.at(6).second, 2);
        QCOMPARE(result.at(7).first, QStringLiteral("in"));
        QCOMPARE(result.at(7).second, 3);
        QCOMPARE(result.at(8).first, QStringLiteral("lectus"));
        QCOMPARE(result.at(8).second, 2);
        QCOMPARE(result.at(9).first, QStringLiteral("leo"));
        QCOMPARE(result.at(9).second, 2);
        QCOMPARE(result.at(10).first, QStringLiteral("magna"));
        QCOMPARE(result.at(10).second, 2);
        QCOMPARE(result.at(11).first, QStringLiteral("mauris"));
        QCOMPARE(result.at(11).second, 2);
        QCOMPARE(result.at(12).first, QStringLiteral("metus"));
        QCOMPARE(result.at(12).second, 2);
        QCOMPARE(result.at(13).first, QStringLiteral("purus"));
        QCOMPARE(result.at(13).second, 2);
        QCOMPARE(result.at(14).first, QStringLiteral("ut"));
        QCOMPARE(result.at(14).second, 3);
    }
}

QTEST_MAIN(WordCounterTest)

#include "word_counter.test.moc"
