#ifndef COMMUNICATIONMAIN_H
#define COMMUNICATIONMAIN_H

#include <QMainWindow>

namespace Ui {
class CommunicationMain;
}

class CommunicationMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommunicationMain(QWidget *parent = 0);
    ~CommunicationMain();

private:
    Ui::CommunicationMain *ui;
};

#endif // COMMUNICATIONMAIN_H
