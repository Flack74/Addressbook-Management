#pragma once

#include <QString>

// Static utility class for field validation.
// Keeping this separate means it's easy to unit-test without touching any UI.
class Validator {
public:
    static bool isValidEmail(const QString &email);
    static bool isValidPhone(const QString &mobile);
    static bool isValidDate(const QString &date);  // expects YYYY-MM-DD
};
