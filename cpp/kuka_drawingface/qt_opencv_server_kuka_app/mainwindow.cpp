#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);
    absolutePath = QCoreApplication::applicationDirPath();
    absolutePath +="/";
    ui->pbar->setRange(0,100);
    ui->pbar->setValue(0);
    ui->slidertop->setRange(0,100);
    ui->sliderdown->setRange(0,100);
    ui->slidertop->setValue(45);
    ui->sliderdown->setValue(90);
    ui->slidertop->setVisible(0);
    ui->sliderdown->setVisible(0);
    ui->Xregime->setVisible(0);
    ui->XYregime->setVisible(0);
    ui->Yregime->setVisible(0);
    ui->full->setVisible(0);
    ui->two->setVisible(0);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("x");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{

    if(ui->start->text() == "createServer"){
        qDebug() <<"  Starting server click..";
        serverproxy *sp = new serverproxy(this);
        sp->getabsolutePath() = absolutePath;
        connect(this, SIGNAL(sendcurrentX(int)),sp,SLOT(getcurrentX(int)));
        connect(this, SIGNAL(sendFlag_mythread()),sp,SLOT(getFlag_mythread()));
        connect(sp, SIGNAL(sendLog(QString)),this,SLOT(getLog(QString)));
        connect(sp, SIGNAL(sendprogbar(int)),this,SLOT(getprogbar(int)));
        connect(sp, SIGNAL(sendstart()),this,SLOT(getstart()));
        connect(sp, SIGNAL(sendfinish()),this,SLOT(getfinish()));
        connect(this, SIGNAL(sendrestartserver()),sp,SLOT(getrestartserver()));
        connect(this, SIGNAL(sendZ(QString)),sp,SLOT(getZ(QString)));
        sp->start();
        serverwasstarted = true;
        emit(sendcurrentX(currentX));
        emit(sendFlag_mythread());
        ui->start->setText("Pause");
        ui->start->show();
    }else
        if(ui->start->text() == "Pause"){
            emit(sendFlag_mythread());
            ui->start->setText("Continue");
            ui->start->show();
        }else
            if(ui->start->text() == "Continue"){
                emit(sendFlag_mythread());
                ui->start->setText("Pause");
                ui->start->show();
            }
}

void MainWindow::on_portrait_clicked()
{

    if(counter == 0){
        ui->slidertop->setVisible(1);
        ui->sliderdown->setVisible(1);
        ui->Xregime->setVisible(1);
        ui->XYregime->setVisible(1);
        ui->Yregime->setVisible(1);
        ui->full->setVisible(1);
        ui->two->setVisible(1);
        counter++;
        opencv* ocv = new opencv();
        ocv->getAbsolutePath() = absolutePath.toUtf8().constData();
        connect(this, SIGNAL(aboutToQuit()), ocv, SLOT(deleteLater()));
        connect(this, SIGNAL(takePortrait()), ocv, SLOT(getRequestForPortrait()));
        connect(ocv, SIGNAL(sendPortrait(QImage)), this, SLOT(listenPortrait(QImage)));
        connect(this, SIGNAL(sendFlagON()),ocv,SLOT(getFlagON()));
        connect(this, SIGNAL(sendtopborder(int)),ocv,SLOT(gettopborder(int)));
        connect(this, SIGNAL(sendbottomborder(int)),ocv,SLOT(getbottomborder(int)));
        connect(this, SIGNAL(sendregime(int)),ocv,SLOT(getregime(int)));
        connect(this, SIGNAL(sendcurrentX(int)),ocv,SLOT(getcurrentX(int)));
        connect(this, SIGNAL(sendlenaattach(int)),ocv,SLOT(getlenaattach(int)));
        connect(ocv, SIGNAL(sendLog2(QString)),this,SLOT(getLog2(QString)));
        connect(ocv, SIGNAL(sendImg(QString)),this,SLOT(listenImg(QString)));
        connect(ocv, SIGNAL(sendQImg(QImage)),this,SLOT(listenQImg(QImage)));
        ocv ->start();
    }

    if(ui->portrait->text() == "startCam"){
        ui->slidertop->setVisible(1);
        ui->sliderdown->setVisible(1);
        ui->Xregime->setVisible(1);
        ui->XYregime->setVisible(1);
        ui->Yregime->setVisible(1);
        ui->two->setVisible(1);
        ui->full->setVisible(1);

        emit(sendFlagON());
        ui->portrait->setText("takePortrait");
        ui->portrait->show();
    }else
        if(ui->portrait->text() == "takePortrait"){
            emit(takePortrait());
            ui->Xregime->setVisible(0);
            ui->XYregime->setVisible(0);
            ui->Yregime->setVisible(0);
            ui->slidertop->setVisible(0);
            ui->sliderdown->setVisible(0);
            ui->full->setVisible(0);
            ui->two->setVisible(0);
            ui->portrait->setText("startCam");
            ui->portrait->show();
            counter = 0;
            defaultSettings();
        }
}

void MainWindow::listenImg(QString str){
    //    qDebug()<<"ich bin hier-MainW!";
    QPixmap pixmap(str);
    ui -> label->setPixmap(pixmap);
    ui -> label->setMask(pixmap.mask());
    ui ->label ->show();
}

void MainWindow::listenQImg(QImage i){
    //        qDebug()<<"ich bin hier-MainW!" << i;
    ui -> label->setPixmap(QPixmap::fromImage(i));
}

void MainWindow::listenPortrait(QImage qi){
    qDebug()<< " listenPortrait!";
    ui -> label->setPixmap(QPixmap::fromImage(qi));
}

void MainWindow::on_pushButton_clicked()
{
    emit(aboutToQuit());
    close();
}

void MainWindow::getLog(QString str){
    if(str == "Close connection. Stop server. ")
        ui->start->setText("createServer");
    ui->log->appendPlainText(str);
}

void MainWindow::getLog2(QString str){
    ui->log->appendPlainText(str);
}

void MainWindow::getprogbar(int i){
    ui->pbar->setValue(i);
}


void MainWindow::getfinish(){
    ui->finish_label->setText("Done.");
}

void MainWindow::getstart(){
    ui->finish_label->setText("");
}

void MainWindow::on_portrait_2_clicked()
{
    if(ui->portrait_2->text() == "downloadImg"){
        ui->portrait_2->setText("unattachImg");
        emit(sendlenaattach(1));
    }else if(ui->portrait_2->text() == "unattachImg"){
        ui->portrait_2->setText("downloadImg");
        emit(sendlenaattach(0));
    }
}

void MainWindow::on_sliderdown_valueChanged(int value)
{
    emit(sendbottomborder(value));
}

void MainWindow::on_slidertop_valueChanged(int value)
{
    emit(sendtopborder(value));
}

void MainWindow::on_XYregime_clicked()
{
    ui->XYregime->setStyleSheet("QPushButton {background-color: #6000DB;color:black;}");
    ui->Xregime->setStyleSheet("");
    ui->Yregime->setStyleSheet("");
    emit(sendregime(0));
}

void MainWindow::on_Yregime_clicked()
{
    ui->Yregime->setStyleSheet("QPushButton {background-color: #6000DB;color:black;}");
    ui->XYregime->setStyleSheet("");
    ui->Xregime->setStyleSheet("");
    emit(sendregime(2));
}

void MainWindow::on_Xregime_clicked()
{
    ui->Xregime->setStyleSheet("QPushButton {background-color: #6000DB;color:black;}");
    ui->XYregime->setStyleSheet("");
    ui->Yregime->setStyleSheet("");
    emit(sendregime(1));
}

void MainWindow::defaultSettings()
{
    ui->full->setStyleSheet("");
    ui->XYregime->setStyleSheet("");
    ui->Yregime->setStyleSheet("");
    ui->Xregime->setStyleSheet("");
    ui->portrait_2->setText("downloadImg");
}

void MainWindow::on_two_clicked()
{
    ui->two->setStyleSheet("QPushButton {background-color: #6000DB;color:black;}");
    ui->full->setStyleSheet("");
    emit(sendcurrentX(0));
    currentX = TWO_ELEM_REGIME;
}

void MainWindow::on_full_clicked()
{
    ui->full->setStyleSheet("QPushButton {background-color: #6000DB;color:black;}");
    ui->two->setStyleSheet("");
    emit(sendcurrentX(1));
    currentX = ALL_ELEM_REGIME;
}

void MainWindow::on_pushButton_2_clicked()
{
    emit(sendrestartserver());
}

void MainWindow::on_clear_clicked()
{
    ui->log->clear();
}

void MainWindow::on_sendZ_clicked()
{
    emit(sendZ(ui->inputZ->text()));
}
