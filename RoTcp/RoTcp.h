#ifndef ROTCP_H
#define ROTCP_H

#include <QWidget>

namespace Ui {
class RoTcp;
}

class RoTcp : public QWidget
{
    Q_OBJECT

public:
    explicit RoTcp(QWidget *parent = 0);
    ~RoTcp();

private:
    Ui::RoTcp *ui;
};

#endif // ROTCP_H
