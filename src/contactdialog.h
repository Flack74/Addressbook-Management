#pragma once

#include "contact.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ContactDialog; }
QT_END_NAMESPACE

class ContactDialog : public QDialog {
    Q_OBJECT

public:
    // Pass in a blank Contact for Add, or an existing one for Edit
    explicit ContactDialog(const Contact &contact, QWidget *parent = nullptr);
    ~ContactDialog();

    Contact contact() const; // call this after exec() == Accepted

private slots:
    void onAccepted();

private:
    Ui::ContactDialog *ui;
    Contact            m_contact; // holds the working copy
};
