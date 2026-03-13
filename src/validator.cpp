#include "validator.h"
#include <QRegularExpression>
#include <QDate>

bool Validator::isValidEmail(const QString &email)
{
    // Standard email regex — not RFC 5322 complete, but covers 99% of real addresses
    static const QRegularExpression re(
        R"(^[A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,}$)"
    );
    return re.match(email).hasMatch();
}

bool Validator::isValidPhone(const QString &mobile)
{
    // Optional leading +, then 7-15 digits (covers international formats)
    static const QRegularExpression re(R"(^\+?[0-9]{7,15}$)");
    return re.match(mobile).hasMatch();
}

bool Validator::isValidDate(const QString &date)
{
    // Qt's own date parser handles YYYY-MM-DD correctly, including leap years
    QDate d = QDate::fromString(date, "yyyy-MM-dd");
    return d.isValid();
}
