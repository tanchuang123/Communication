#include "RoTcp.h"
#include "ui_RoTcp.h"

RoTcp::RoTcp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoTcp)
{
    ui->setupUi(this);
}

RoTcp::~RoTcp()
{
    delete ui;
}
