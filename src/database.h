#pragma once

#include "contact.h"
#include <QList>
#include <QString>

class Database {
public:
    // Opens or creates the SQLite database and runs schema setup.
    // Returns false if something went wrong.
    static bool init(const QString &path = "contacts.db");

    static QList<Contact>  fetchAll();
    static bool            save(Contact &contact);   // INSERT or UPDATE
    static bool            remove(int id);
};
