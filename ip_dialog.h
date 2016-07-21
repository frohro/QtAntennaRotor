#ifndef IP_DIALOG_H
#define IP_DIALOG_H

#include <QDialog>

#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include "stdint.h"
#include <QHBoxLayout>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>
#include <QString>
#include <QDebug>
#include <QSignalSpy>

#include <QHostAddress>
#include <QMessageBox>


namespace Ui {
class IP_Dialog;
}

class IP_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit IP_Dialog(QWidget *parent = 0);
    ~IP_Dialog();
     Ui::IP_Dialog *ui;
//     IPCtrl *ipControl;

signals:
     void changeIPAddress(QString);
     void changePort(int);

private slots:
     void on_buttonBox_accepted();

private:

};

#endif // IP_DIALOG_H
