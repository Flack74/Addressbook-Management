#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Database::init(const QString &path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qWarning() << "Database: could not open" << path
                   << db.lastError().text();
        return false;
    }

    // Create the table if it does not exist yet.
    // This is our schema one table.
    QSqlQuery q;
    bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS contacts ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name     TEXT    NOT NULL,"
        "  mobile   TEXT    DEFAULT '',"
        "  email    TEXT    DEFAULT '',"
        "  birthday TEXT    DEFAULT ''"
        ");"
    );

    if (!ok) {
        qWarning() << "Database: schema creation failed:" << q.lastError().text();
    }

    return ok;
}

QList<Contact> Database::fetchAll()
{
    QList<Contact> list;
    QSqlQuery q("SELECT id, name, mobile, email, birthday FROM contacts ORDER BY name ASC;");

    while (q.next()) {
        list.append(Contact(
            q.value(0).toInt(),
            q.value(1).toString(),
            q.value(2).toString(),
            q.value(3).toString(),
            q.value(4).toString()
        ));
    }

    return list;
}

bool Database::save(Contact &contact)
{
    QSqlQuery q;

    if (contact.id == -1) {
        // New contact — INSERT
        q.prepare(
            "INSERT INTO contacts (name, mobile, email, birthday) "
            "VALUES (:name, :mobile, :email, :birthday);"
        );
    } else {
        // Existing contact — UPDATE
        q.prepare(
            "UPDATE contacts SET name=:name, mobile=:mobile, "
            "email=:email, birthday=:birthday WHERE id=:id;"
        );
        q.bindValue(":id", contact.id);
    }

    q.bindValue(":name",     contact.name);
    q.bindValue(":mobile",   contact.mobile);
    q.bindValue(":email",    contact.email);
    q.bindValue(":birthday", contact.birthday);

    if (!q.exec()) {
        qWarning() << "Database: save failed:" << q.lastError().text();
        return false;
    }

    // After INSERT, grab the auto-assigned ID so the caller knows it
    if (contact.id == -1) {
        contact.id = q.lastInsertId().toInt();
    }

    return true;
}

bool Database::remove(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM contacts WHERE id=:id;");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "Database: delete failed:" << q.lastError().text();
        return false;
    }

    return true;
}
