#pragma once

#include "contact.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ContactDialog; }
QT_END_NAMESPACE

class ContactDialog : public QDialog {
    Q_OBJECT

public:
    explicit ContactDialog(const Contact &contact, QWidget *parent = nullptr);
    ~ContactDialog();

    Contact contact() const;

private slots:
    void onSaveClicked();

private:
    Ui::ContactDialog *ui;
    Contact            m_contact;
};
