#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ipDialog = new IP_Dialog(this);
    myRotCtlSocket.Connect();
    if((myRotCtlSocket.connected))
        ui->connectButton->hide();
    else
        ui->connectButton->show();

    assert(connect(rotationEstimateTimer,SIGNAL(timeout()),this,SLOT(updateProgressBar())));
    assert(connect(&myRotCtlSocket,SIGNAL(bearingReturned(int)),ui->lcdNumber,SLOT(display(int))));
    assert(connect(this, SIGNAL(setBearing(int)),ui->lcdNumber,SLOT(display(int))));
    assert(connect(ui->getBearingButton,SIGNAL(clicked()),&myRotCtlSocket,SLOT(getPresentBearing())));
    assert(connect(ui->graphicsView,SIGNAL(directionSelected(int,int)),this,SLOT(getNewDirection(int,int))));
    assert(connect(this,SIGNAL(setBearing(int)),&myRotCtlSocket,SLOT(setBearing(int))));
    assert(connect(&myRotCtlSocket,SIGNAL(bearingReturned(int)),this,SLOT(updatePointingBearing(int))));
    assert(connect(ui->ipPushButton,SIGNAL(clicked()),this,SLOT(getIPAddress())));
    assert(connect(ipDialog,SIGNAL(changeIPAddress(QString)),this,SLOT(updateIPAddress(QString))));
    assert(connect(&myRotCtlSocket,SIGNAL(isConnected(bool)),this,SLOT(showConnectedButton(bool))));

    myRotCtlSocket.getPresentBearing(); //Initialize the readout.
}

MainWindow::~MainWindow()
{
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

void MainWindow::updateIPAddress(QString ipAddress)
{
    qDebug() << "MainWindow::in updateIPAddress with IP address" << ipAddress;

    myRotCtlSocket.setIPAddress(ipAddress);
    if((myRotCtlSocket.connected))
        ui->connectButton->hide();
    else
        ui->connectButton->show();
}

void MainWindow::showConnectedButton(bool connected)
{
    if(connected)
    {
        ui->connectButton->hide();
        //myRotCtlSocket.getPresentBearing();
    }
    else
        ui->connectButton->show();
}
