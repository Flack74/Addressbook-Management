#pragma once

#include <QString>

// Plain data class representing a single contact.
// Keeping data separate from UI makes the code easier to test.
struct Contact {
    int     id;       // -1 means not yet saved to DB
    QString name;
    QString mobile;
    QString email;
    QString birthday; // stored as "YYYY-MM-DD"

    Contact()
        : id(-1)
    {}

    Contact(int id, const QString &name, const QString &mobile,
            const QString &email, const QString &birthday)
        : id(id), name(name), mobile(mobile), email(email), birthday(birthday)
    {}
};
