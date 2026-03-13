#include <QtTest>
#include "../src/database.h"
#include "../src/contact.h"

class TestDatabase : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();   // runs once before all tests
    void cleanupTestCase();

    void testInsertAndFetch();
    void testUpdate();
    void testDelete();
};

void TestDatabase::initTestCase()
{
    // Use an in-memory SQLite database so tests don't leave files around
    QVERIFY(Database::init(":memory:"));
}

void TestDatabase::cleanupTestCase()
{
    // Nothing to clean up — in-memory DB disappears on its own
}

void TestDatabase::testInsertAndFetch()
{
    Contact c;
    c.name     = "Alice";
    c.mobile   = "+911234567890";
    c.email    = "alice@example.com";
    c.birthday = "1990-03-15";

    QVERIFY(Database::save(c));
    QVERIFY(c.id > 0); // DB should have assigned a real ID

    QList<Contact> all = Database::fetchAll();
    QVERIFY(!all.isEmpty());

    bool found = false;
    for (const Contact &x : all) {
        if (x.id == c.id && x.name == "Alice") {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestDatabase::testUpdate()
{
    Contact c;
    c.name  = "Bob";
    c.email = "bob@example.com";
    Database::save(c);

    c.name = "Robert";
    QVERIFY(Database::save(c));

    QList<Contact> all = Database::fetchAll();
    bool found = false;
    for (const Contact &x : all) {
        if (x.id == c.id) {
            QCOMPARE(x.name, QString("Robert"));
            found = true;
        }
    }
    QVERIFY(found);
}

void TestDatabase::testDelete()
{
    Contact c;
    c.name = "Temp Contact";
    Database::save(c);
    int id = c.id;

    QVERIFY(Database::remove(id));

    QList<Contact> all = Database::fetchAll();
    for (const Contact &x : all) {
        QVERIFY(x.id != id); // should not exist anymore
    }
}

QTEST_MAIN(TestDatabase)
#include "test_database.moc"
