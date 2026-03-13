#include <QtTest>
#include "../src/validator.h"

class TestValidator : public QObject {
    Q_OBJECT

private slots:
    void emailValid_data();
    void emailValid();
    void phoneValid_data();
    void phoneValid();
    void dateValid_data();
    void dateValid();
};

void TestValidator::emailValid_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<bool>("expected");

    QTest::newRow("standard")        << "user@example.com"    << true;
    QTest::newRow("subdomain")       << "a@b.co.uk"           << true;
    QTest::newRow("plus tag")        << "user+tag@gmail.com"  << true;
    QTest::newRow("missing @")       << "notanemail"          << false;
    QTest::newRow("missing domain")  << "user@"               << false;
    QTest::newRow("double dot")      << "user@ex..com"        << false;
    QTest::newRow("empty")           << ""                    << false;
}

void TestValidator::emailValid()
{
    QFETCH(QString, email);
    QFETCH(bool, expected);
    QCOMPARE(Validator::isValidEmail(email), expected);
}

void TestValidator::phoneValid_data()
{
    QTest::addColumn<QString>("phone");
    QTest::addColumn<bool>("expected");

    QTest::newRow("international")   << "+919876543210"  << true;
    QTest::newRow("local 10 digit")  << "9876543210"     << true;
    QTest::newRow("too short")       << "123"            << false;
    QTest::newRow("letters")         << "abcdefg"        << false;
    QTest::newRow("empty")           << ""               << false;
}

void TestValidator::phoneValid()
{
    QFETCH(QString, phone);
    QFETCH(bool, expected);
    QCOMPARE(Validator::isValidPhone(phone), expected);
}

void TestValidator::dateValid_data()
{
    QTest::addColumn<QString>("date");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid date")      << "1995-07-21"    << true;
    QTest::newRow("leap day valid")  << "2000-02-29"    << true;
    QTest::newRow("leap day invalid")<< "1900-02-29"    << false;
    QTest::newRow("wrong format")    << "21-07-1995"    << false;
    QTest::newRow("garbage")         << "not-a-date"    << false;
}

void TestValidator::dateValid()
{
    QFETCH(QString, date);
    QFETCH(bool, expected);
    QCOMPARE(Validator::isValidDate(date), expected);
}

QTEST_MAIN(TestValidator)
#include "test_validator.moc"
