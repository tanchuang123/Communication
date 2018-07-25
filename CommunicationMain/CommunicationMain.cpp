#include "CommunicationMain.h"
#include "ui_CommunicationMain.h"

CommunicationMain::CommunicationMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommunicationMain)
{
    ui->setupUi(this);
}

CommunicationMain::~CommunicationMain()
{
    delete ui;
}
