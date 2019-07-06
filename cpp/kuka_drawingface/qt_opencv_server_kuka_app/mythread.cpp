#include "mythread.h"
#include <QThread>
#include <QFile>
#include <QDataStream>
mythread::mythread(int ID, QObject *parent, CurrentX reg) : QThread(parent)
{
    this->elemcountregime = ALL_ELEM_REGIME;
    this-> socketDescriptor = ID;
}

QString& mythread::getAbsolutePath (){
 return absolutePath;
}

void mythread::getcurrentX(int i){
    switch (i) {
    case ALL_ELEM_REGIME:
        qDebug()<< "hello_all_elem";
        elemcountregime = ALL_ELEM_REGIME;
        break;
    case TWO_ELEM_REGIME:
        qDebug()<< "hello_two_elem";
        elemcountregime = TWO_ELEM_REGIME;
        break;
    }
}

void mythread::getrestartserver(){
    isrestarted = 1;
    wasfinished = false;
}

void mythread::getFlag_mythread(){
    qDebug()<<"flag";
    if (flag == false)flag = true;
    else flag = false;

    qDebug()<< flag << " now!";
}

void mythread::getZ(QString str){
    Z = str;
}

void mythread:: run(){
    qDebug() << socketDescriptor <<"  Starting thread..";
    QString s = QString::number(socketDescriptor);
    emit(sendLog(s+"  Starting thread.."));
    socket = new QTcpSocket(this);

    if(!socket -> setSocketDescriptor(this -> socketDescriptor)){
        emit error(socket -> error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    s = QString::number(socketDescriptor);
    qDebug() << socketDescriptor << " Client connected";
    emit(sendLog(s+" Client connected"));

    if(elemcountregime == TWO_ELEM_REGIME){
        startTransferTwoElement();
    }else {
        startTransferAllElement();
        qDebug()<< " hello";
    }
}

void mythread::sockReady(){
}

void mythread::startTransferAllElement(){
    QString path = absolutePath +"pointsFull.txt";
    QFile file(path);

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QDataStream socketStream(socket);
        QTextStream fileStream(&file);
        QByteArray mydata=file.readAll();
        qDebug() << "Final Testing is size = " << mydata.size();
        socket->write(mydata);
    }
    socket->waitForBytesWritten();

}

void mythread::startTransferTwoElement(){
    while(wasfinished != true){
        while(wasfinished == true){qDebug()<< "finished!///////////////";}
        emit(sendstart());
        vector<double> vecX;
        vector<double> vecY;
        QString path = absolutePath + "points.txt";
        QByteArray ba = path.toLocal8Bit();
        const char *c_str2 = ba.data();
        ifstream file(c_str2);  //read points
        string first;
        string second;
        string entire;
        double x1;
        double y1;
        double x2;
        double y2;
        volatile double counterForVec = 0;
        if (file.is_open()) {
            while (getline(file, entire))
            {
                int pos = entire.find(" ");
                entire = entire.substr(pos+1, entire.size()-1);
                pos = entire.find("&");
                first = entire.substr(0, pos);
                second = entire.substr(pos+1, entire.size()-1);
                pos = first.find(" ");
                x1 = atof(first.substr(0,pos).c_str());
                y1 = atof(first.substr(pos+1, first.size()-1).c_str());
                pos = second.find(" ");
                x2 = atof(second.substr(0,pos).c_str());
                y2 = atof(second.substr(pos+1, second.size()-1).c_str());
                vecX.push_back(x1);
                vecX.push_back(x2);
                vecY.push_back(y1);
                vecY.push_back(y2);
            }
        }else{
            qDebug() << "cannot open file!";
            emit(sendLog("cannot open file!"));
            return;
        }


        while(counterForVec != vecX.size()-2){
            while(flag == false){
//                this->sleep(500);
//                qDebug()<< "hello!";
//                someFunCounter++;
            }
            emit(sendprogbar(counterForVec/(vecX.size()-1)*100));

            while(counter2 < 2){
                out ="<Server><Pos2><X>" + to_string(vecX.at(counterForVec)) + "</X><Y>"+ to_string(vecY.at(counterForVec))+"</Y><Z>"+  Z.toUtf8().constData() +"</Z><A>65.75</A><B>-81.44</B><C>150.0</C><S>2</S><T>3</T></Pos2></Server>";
                strcpy(buf, out.c_str());
                counterForVec++;
                socket->write(buf);
                counter +=1;
                counter2++;
                s = "";

                while(s ==""){
                    socket->waitForReadyRead(100);
                    Data = socket->readAll();
                    s = Data.data();
                    if(s != ""){
                        QString st = QString::number(counter);
                        QString s_buf =st + " from client: " + s;
                        emit(sendLog(s_buf));
                    }
                }
                qDebug() <<"from client: " + s;
            }

            if(isrestarted == 1){
                break;
            }

            while(s != "<Robot><Cmd>100</Cmd></Robot>") {
                socket->waitForReadyRead(100);
                Data = socket->readAll();
                s = Data.data();

                if(s == "<Robot><Cmd>100</Cmd></Robot>"){
                    QString s_buf = "got flag: " + s;
                    counter2 = 0;
                    qDebug()<< "Got flag: "<< s;
                    emit(sendLog(s_buf));
                }
            }
        }

        if(isrestarted == 1){
            counter = 0;
            isrestarted = 0;
            file.close();
            emit(sendLog("restarting process..."));
            continue;
        }
        emit(sendprogbar(100));
        qDebug() << "myThread: I have finished! ";
        emit(sendfinish());
        wasfinished = true;
    }
}

void mythread::disconnected(){
    qDebug() << socketDescriptor <<"  Disconnected thread..";
    QString s = QString::number(socketDescriptor);
    emit(sendLog( s + "  Disconnected thread.."));
//    emit(sendLog("Close connection. Stop server. "));
    socket->deleteLater(); // delete object
    this->exit(0);
//    exit(0);
}
