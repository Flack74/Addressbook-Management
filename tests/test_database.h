#pragma once
#include <QtTest>
#include "../src/database.h"
#include "../src/contact.h"

class TestDatabase : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testInsertAndFetch();
    void testUpdate();
    void testDelete();
};

inline void TestDatabase::initTestCase()
{
    QVERIFY(Database::init(":memory:"));
}

inline void TestDatabase::cleanupTestCase() {}

inline void TestDatabase::testInsertAndFetch()
{
    Contact c;
    c.name     = "Alice";
    c.mobile   = "+911234567890";
    c.email    = "alice@example.com";
    c.birthday = "1990-03-15";

    QVERIFY(Database::save(c));
    QVERIFY(c.id > 0);

    QList<Contact> all = Database::fetchAll();
    QVERIFY(!all.isEmpty());

    bool found = false;
    for (const Contact &x : all) {
        if (x.id == c.id && x.name == "Alice") { found = true; break; }
    }
    QVERIFY(found);
}

inline void TestDatabase::testUpdate()
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

inline void TestDatabase::testDelete()
{
    Contact c;
    c.name = "Temp Contact";
    Database::save(c);
    int id = c.id;

    QVERIFY(Database::remove(id));

    QList<Contact> all = Database::fetchAll();
    for (const Contact &x : all) {
        QVERIFY(x.id != id);
    }
}
