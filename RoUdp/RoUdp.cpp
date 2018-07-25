#include "RoUdp.h"
#include "ui_RoUdp.h"

RoUdp::RoUdp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoUdp)
{
    ui->setupUi(this);
}

RoUdp::~RoUdp()
{
    delete ui;
}
