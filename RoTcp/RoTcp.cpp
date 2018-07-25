#include "RoTcp.h"
#include "ui_RoTcp.h"
#include <QtDebug>
//RoTcp::RoTcp(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::RoTcp)
//{
//    ui->setupUi(this);
//    _TcpServer = new QTcpServer();
//    _TcpSocket = new QTcpSocket;
//    //关联信号槽
//    connect(_TcpServer,SIGNAL(newConnection()),this,SLOT(slotNewClient()));
//    _TcpServer->listen(QHostAddress::Any,_port);
//}

//RoTcp::~RoTcp()
//{
//    delete ui;
//}
//void RoTcp::slotNewClient()
//{
//    qDebug()<<"新客户端连接";
//    _TcpSocket = _TcpServer->nextPendingConnection();//与客户端通信的套接字
//    //关联接收客户端数据信号readRead信号，客户端有数据就会发出readRead信号
//    connect(_TcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadClientData()));
//    connect(_TcpSocket,SIGNAL(disconnected()),this,SLOT(slotClientDisConnect()));

//}
//void RoTcp::slotReadClientData()
// {
//     //可以实现同时读取多个客户端发送过来的消息
//     QTcpSocket *obj = (QTcpSocket*)sender();
//     QString msg = obj->readAll();
//     qDebug()<<msg;
//}
//void RoTcp::slotClientDisConnect()
// {
//     QTcpSocket *obj = (QTcpSocket*)sender();//掉线对象
//     qDebug()<<obj->peerAddress().toString();//打印出掉线对象的ip
// }


RoTcp * RoTcp::pRoTcp = Q_NULLPTR;
RoTcp::RoTcp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoTcp)
{
    ui->setupUi(this);
    _receSize = 0;
    _sendSize = 0;
    //初始化定时器
    _timer  = new QTimer();
    connect(_timer,SIGNAL(timeout()),this,SLOT(slotAutoTimeSend()));
    ui->lineEdit_ip->setPlaceholderText(QStringLiteral("请输入IP"));
    ui->lineEdit_port->setPlaceholderText(QStringLiteral("请输入端口"));
    ui->lineEdit_time->setPlaceholderText(QStringLiteral("请输入定时器时间"));
}
RoTcp *RoTcp::init()
{
    if(!pRoTcp)
        {
            pRoTcp = new RoTcp;
        }
    return pRoTcp;
}
RoTcp::~RoTcp()
{
    delete ui;
}
//选择作为服务端
void RoTcp::on_radioButton_server_clicked()
{
    this->_isCheckServer = true;
    this->_isServer = true;
    //获取本地ip显示在edit中;
    ui->lineEdit_ip->setText(QHostAddress(QHostAddress::LocalHost).toString());
    ui->lineEdit_ip->setEnabled(false);
    this->_isCheckClient = false;
}
//选择作为客户端
//作为客户端需要绑定服务端的ip和端口
void RoTcp::on_radioButton_client_clicked()
{
    this->_isCheckClient = true;
    this->_isServer = false;
    ui->lineEdit_ip->setEnabled(true);//打开ip输入编辑器
    this->_isCheckServer = false;
}
//启动服务器或者连接服务器
void RoTcp::on_pushButton_start_clicked()
{
    if(_isServer)//服务端
    {
        _TcpServer = new QTcpServer();
        //关联新客户端连接信号
        connect(_TcpServer,SIGNAL(newConnection()),this,SLOT(slotAcceptConnect()));
        //监听客户端端口
        _TcpServer->listen(QHostAddress::Any,ui->lineEdit_port->text().toInt());//服务端监听端口和ip
        ui->pushButton_start->setEnabled(false);//开启之后禁用开启按钮
    }
    if(!_isServer)//客户端
    {
        _TcpSocket = new QTcpSocket();
        //检测连接成功信号
        connect(_TcpSocket,SIGNAL(connected()),this,SLOT(slotClientConnectSucess()));//客户端去连接服务器
        //绑定服务端的ip和端口
        _TcpSocket->connectToHost(ui->lineEdit_ip->text(),ui->lineEdit_port->text().toInt());
        //关联接收数据信号
        connect(_TcpSocket,SIGNAL(readyRead()),this,SLOT(slotReciveData()));
        //关联掉线信号
        connect(_TcpSocket,SIGNAL(disconnected()),this,SLOT(slotClientDisconnect()));//客户端掉线时触发
    }
    if(_isCheckServer==false&&_isCheckClient==false)//都没选
    {
        QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("请选择服务端或者客户端"));
        ui->pushButton_start->setEnabled(true);
        return;
    }
    if(_isCheckServer)//选择服务端
    {
        if(/*ui->lineEdit_ip->text().isEmpty()||*/ui->lineEdit_port->text().isEmpty())
        {
            QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("请输入端口号"));
            ui->pushButton_start->setEnabled(true);
            return;
        }
    }

    if(_isCheckClient)
    {
        if(ui->lineEdit_ip->text().isEmpty()||ui->lineEdit_port->text().isEmpty())
        {
            QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("请输入IP和端口"));
            ui->pushButton_start->setEnabled(true);
            return;
        }
    }
}
//关闭服务器或者断开
void RoTcp::on_pushButton_close_clicked()
{
    if(_isServer)//服务端
    {
        for(int i=0;i<_Clients.count();i++)
        {
            _Clients.at(i)->close();//关闭所有客户端
        }
        //关闭所有服务器之后开始按钮才能启用
        _TcpServer->close();
        ui->pushButton_start->setEnabled(true);
    }
    else {
        _TcpSocket->close();//关闭客户端
        ui->pushButton_start->setEnabled(true);//启用开始按钮
    }
}
//清空接收区
void RoTcp::on_pushButton_empty_recive_clicked()
{
    ui->listWidget_recive->clear();
    this->_receSize = 0;
    ui->label_dataRecive->setText(QString::number(_receSize));
}
//清空发送区
void RoTcp::on_pushButton_empty_send_clicked()
{
    ui->textEdit_send->clear();
    this->_sendSize = 0;
    ui->label_sendData->setText(QString::number(_sendSize));
}

void RoTcp::on_pushButton_send_clicked()
{
    slotAutoTimeSend();
}
//自动发送数据
void RoTcp::on_checkBox_automatic_clicked(bool checked)
{
    if(checked)
    {
        if(ui->lineEdit_time->text().toInt()<=0)
        {
            ui->checkBox_automatic->setChecked(false);
            return;
        }
        _timer->start(ui->lineEdit_time->text().toInt());//启动定时器
    }
    else {
        _timer->stop();//停止定时器
    }

}
void RoTcp::slotReciveData()
{
    QTcpSocket *obj = (QTcpSocket*)sender();
    //获取发送端的ip
    QString ip = obj->peerAddress().toString();
    ip.remove("::ffff");
    QString msg = obj->readAll();
    ui->listWidget_recive->addItem(ip+":"+msg);
    _receSize+=msg.size();//统计接收到的数据的字节数
    ui->label_dataRecive->setText(QString::number(_receSize));
}
void RoTcp::slotClientDisconnect()
{
    QTcpSocket *obj = (QTcpSocket*)sender();//获取掉线对象
    if(_isServer)
    {
        int row = _Clients.indexOf(obj);//找到掉线对象的内容所在的行
        QListWidgetItem *item = ui->listWidget_onlinelist->takeItem(row);
        delete item;
        _Clients.remove(row);//从容器中删除对象
        //掉线时删除在线数量
        _onLineNum--;
        ui->label_num_online->setText(QString::number(_onLineNum));
    }
    else {
        ui->pushButton_start->setEnabled(true);
    }
}
//客户端连接成功
void RoTcp::slotClientConnectSucess()
{
    ui->pushButton_start->setEnabled(false);//连接成功禁用开始按钮
}
//定时器定时发送数据
void RoTcp::slotAutoTimeSend()
{
    quint64 len = _TcpSocket->write(ui->textEdit_send->toPlainText().toUtf8());
    if(len>0)
    {
         _sendSize +=len;// 统计发送的字节数
         ui->label_sendData->setText(QString::number(_sendSize));//发送的字节数
    }
}
void RoTcp::slotAcceptConnect()
{
    _TcpSocket = _TcpServer->nextPendingConnection();//返回与客户端连接通信的套接字
    //关联接收数据信号
    connect(_TcpSocket,SIGNAL(readyRead()),this,SLOT(slotReciveData()));
    //关联掉线信号
    connect(_TcpSocket,SIGNAL(disconnected()),this,SLOT(slotClientDisconnect()));
    //上线用户添加到客户列表容器
    _Clients.append(_TcpSocket);
    //把用户添加到界面列表中
    QString ip = _TcpSocket->peerAddress().toString().remove("::ffff");//去除客户端中多余的字符
    ui->listWidget_onlinelist->addItem(ip);
    //在线数量添加
    _onLineNum++;
    ui->label_num_online->setText(QString::number(_onLineNum));//显示在线数量
}
void RoTcp::on_onlineUserList_doubleClicked(const QModelIndex &index)
{
    _TcpSocket = _Clients.at(index.row());
}

void RoTcp::on_pushButton_receive_file_clicked()
{
//    _ReceFileForm = new ReceiveFileForm();
//    _ReceFileForm->show();
}
