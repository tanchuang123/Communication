#ifndef ROUDP_H
#define ROUDP_H

#include <QWidget>

namespace Ui {
class RoUdp;
}

class RoUdp : public QWidget
{
    Q_OBJECT

public:
    explicit RoUdp(QWidget *parent = 0);
    ~RoUdp();

private:
    Ui::RoUdp *ui;
};

#endif // ROUDP_H
