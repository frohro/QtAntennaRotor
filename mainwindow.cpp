#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "unistd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    rotationEstimateTimer = new QTimer(this);
    radius = 244;

    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->graphicsView->centerOn(0,0);

    directionScene = new QGraphicsScene(this);
    directionScene->setSceneRect(-187,-222,400,400);
    ui->graphicsView->setScene(directionScene);

    loadSettings();

    showConnectedButton(myRotCtlSocket.connected);

    ipDialog = new IP_Dialog(this);
    ipDialog->ui->ipLineEdit->setText(myRotCtlSocket.getIPAddress());
    ipDialog->ui->lineEdit->setText(QString::number(myRotCtlSocket.getPort()));

    assert(connect(rotationEstimateTimer,SIGNAL(timeout()),this,SLOT(updateProgressBar())));
    assert(connect(&myRotCtlSocket,SIGNAL(bearingReturned(int)),ui->lcdNumber,SLOT(display(int))));
    assert(connect(this, SIGNAL(setBearing(int)),ui->lcdNumber,SLOT(display(int))));
    assert(connect(ui->getBearingButton,SIGNAL(clicked()),&myRotCtlSocket,SLOT(getPresentBearing())));
    assert(connect(ui->graphicsView,SIGNAL(directionSelected(int,int)),this,SLOT(getNewDirection(int,int))));
    assert(connect(this,SIGNAL(setBearing(int)),&myRotCtlSocket,SLOT(setBearing(int))));
    assert(connect(&myRotCtlSocket,SIGNAL(bearingReturned(int)),this,SLOT(updatePointingBearing(int))));
    assert(connect(ipDialog,SIGNAL(changeIPAddress(QString)),this,SLOT(updateIPAddress(QString))));
    assert(connect(ipDialog,SIGNAL(changePort(int)),this,SLOT(updatePort(int))));
    assert(connect(&myRotCtlSocket,SIGNAL(isConnected(bool)),this,SLOT(showConnectedButton(bool))));

    if(myRotCtlSocket.connected) myRotCtlSocket.getPresentBearing(); //Initialize the readout.
}

MainWindow::~MainWindow()
{
    qDebug() << "Closing down, writing settings, etc.";
    saveSettings();
    myRotCtlSocket.Cleanup();
    delete ui;
}

void MainWindow::getNewDirection(int x, int y)
{
    int direction;
    int old_bearing = ui->lcdNumber->value();
    x -= ui->graphicsView->width()/2-12;
    y -= ui->graphicsView->height()/2+22;
    y = -y;
    qDebug() << "x = " << x <<"y = " << y;
    direction = -((int)(atan2((double)y, (double)x)*180/3.141592654)-90);
    if (ui->longPathButton->isChecked()) direction +=180;
    if (direction > 180) direction -= 360;
    ui->progressBar->setVisible(true);
    this->rotationEstimateTimer->start(abs(direction-old_bearing)*1000*60/360/100*1.5+200);
    updatePointingBearing(direction);
    emit setBearing(direction);
}

void MainWindow::updateProgressBar()
{
    int value = this->ui->progressBar->value();
    if (value < 100) this->ui->progressBar->setValue(value+1);
    else {
        this->ui->progressBar->setVisible(false);
        this->ui->progressBar->setValue(0);
        emit myRotCtlSocket.getPresentBearing();
        emit myRotCtlSocket.getPresentBearing();
    }
}

void MainWindow::updatePointingBearing(int bearing)
{
    int x = ui->graphicsView->width()/2-12;
    int y = ui->graphicsView->height()/2+22;
    qDebug() << "x" << x << "y" << y;

    QPen blackPen(Qt::black);
    blackPen.setWidth(4);
    QPen redPen(Qt::red);
    QPen greenPen(Qt::green);
    QPen blueePen(Qt::blue);
    directionScene->clear();
    directionScene->addLine(0,0,radius*sin(bearing*3.14159/180),-radius*cos(bearing*3.14159/180),blackPen);
    ui->lcdNumber->display(bearing);

}

void MainWindow::getIPAddress()
{
    ipDialog->show();
}

void MainWindow::on_connectButton_clicked()
{
    myRotCtlSocket.Connect();
    if(myRotCtlSocket.connected)
        ui->connectButton->hide();
}

void MainWindow::updateIPAddress(QString ipAddressString)
{
    qDebug() << "MainWindow::in updateIPAddress with IP address" << ipAddressString;
    myRotCtlSocket.setIPAddress(ipAddressString);
}

void MainWindow::updatePort(int port)
{
    qDebug() << "MainWindow::in updatePort with port" << port;
    myRotCtlSocket.setPort(port);
}

void MainWindow::showConnectedButton(bool connected)
{
    if(connected)
    {
        ui->connectButton->hide();
        myRotCtlSocket.getPresentBearing();
    }
    else
        ui->connectButton->show();
}


void MainWindow::loadSettings()
{
    QSettings settings("rotor","rotor");

    settings.beginGroup("Common");
    updateIPAddress(settings.value("ipAddress", "192.168.2.156").toString());
    updatePort(settings.value("port", 4533).toInt());
    backgroundImage = static_cast<MainWindow::BackgroundImage>(settings.value("background",kl7na).toInt());
    qDebug() << "backgroundImage is: " << backgroundImage;
    if(backgroundImage == compass)
    {
        on_actionCompass_triggered();
    }
    else if(backgroundImage == kl7na)
    {
        on_actionKL7NA_Great_Circle_triggered();
    }
    else
    {
        qDebug() << "Loading background image settings did not work.";
    }
    settings.endGroup();
}

void MainWindow::saveSettings()
{
//    QSettings settings(ourSettingsFile, QSettings::NativeFormat);
    QSettings settings("rotor","rotor");
    settings.beginGroup("Common");
    settings.setValue("ipAddress", myRotCtlSocket.getIPAddress());
    settings.setValue("port", myRotCtlSocket.getPort());
    settings.setValue("background",backgroundImage);
    settings.endGroup();
}


void MainWindow::on_actionHost_Address_triggered()
{
    ipDialog->show();
}

void MainWindow::on_actionCompass_triggered()
{
    backgroundImage = compass;
    backgroundImageResource = tr("background-image:url(:/Images/compass.png)");
    this->centralWidget()->setStyleSheet(backgroundImageResource);
    ui->label_3->hide();
    ui->groupBox->hide();
}

void MainWindow::on_actionKL7NA_Great_Circle_triggered()
{
    qDebug() << "In on_actionKL7NA_Great_Circle_triggered, with backgroundImage starting as: " << backgroundImage;
    backgroundImage = kl7na;
    backgroundImageResource = tr("background-image:url(:/Images/aeqd.png)");
    ui->label_3->show();
    ui->groupBox->show();
    this->centralWidget()->setStyleSheet(backgroundImageResource);
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("QtAntennaRotor"),
                       tr("This program connects to a remote server running hamlib's rotctld to rotate an antenna. "
                          "GPL code from KL7NA."));
}
