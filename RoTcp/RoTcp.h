#ifndef ROTCP_H
#define ROTCP_H

#include <QWidget>
//#include <QtNetwork/QTcpServer>
//#include <QtNetwork/QTcpSocket>
//namespace Ui {
//class RoTcp;
//}

//class RoTcp : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit RoTcp(QWidget *parent = 0);
//    ~RoTcp();
//private slots:
//    void slotNewClient();
//    void slotReadClientData();
//    void slotClientDisConnect();
//private:
//    Ui::RoTcp *ui;
//    QTcpServer *_TcpServer;
//    QTcpSocket *_TcpSocket;
//    int _port = 2222;

//};
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QVector>
namespace Ui {
class RoTcp;
}

class RoTcp : public QWidget
{
    Q_OBJECT

public:

    explicit RoTcp(QWidget *parent = 0);
    ~RoTcp();
    static RoTcp * init();
private slots:
    void on_radioButton_server_clicked();

    void on_radioButton_client_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_empty_recive_clicked();

    void on_pushButton_empty_send_clicked();

    void on_pushButton_send_clicked();
    void slotAcceptConnect();//接受连接
    void slotReciveData();//接受数据
    void slotAutoTimeSend();//定时器发送数据
    void slotClientDisconnect();//关联掉线信号
    void slotClientConnectSucess();//检测客户端连接成功信号
    void on_onlineUserList_doubleClicked(const QModelIndex &index);//选择给哪个客户端发送数据

    void on_checkBox_automatic_clicked(bool checked);//选择自动发还是手动发

    void on_pushButton_receive_file_clicked();

private:
    static RoTcp * pRoTcp;
    Ui::RoTcp *ui;
    QTimer *_timer;
    QTcpServer *_TcpServer;
    QTcpSocket *_TcpSocket;
    QVector<QTcpSocket*> _Clients;//存储所有在线客户端
    bool _isServer;//标志位,true为服务器，false为客户端
    //保存接收和发送数据的字节数
    quint64 _receSize = 0;
    quint64 _sendSize = 0;
    qint16  _onLineNum = 0;
    bool _isCheckServer = false;//选择服务端.
    bool _isCheckClient = false;//选择客户端.

};

#endif // ROTCP_H
