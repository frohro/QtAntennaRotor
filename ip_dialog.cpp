#include "ip_dialog.h"
#include "ui_ip_dialog.h"


IP_Dialog::IP_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IP_Dialog)
{
    ui->setupUi(this);
}

IP_Dialog::~IP_Dialog()
{
    delete ui;
}

void IP_Dialog::on_buttonBox_accepted()
{
    QHostAddress myIP;
    if (myIP.setAddress( ui->ipLineEdit->text()))
    {
        qDebug() << "IP address changed to: " << ui->ipLineEdit->text();
        emit changeIPAddress(ui->ipLineEdit->text());
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Bad hostname or IP address!  Better try again.");
        msgBox.exec();
    }
}
