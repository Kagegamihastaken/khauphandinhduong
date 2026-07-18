#ifndef KHAUPHANDINHDUONG_HOME_HPP
#define KHAUPHANDINHDUONG_HOME_HPP

#include "ui_home.h"

class HomeWidget : public QWidget {
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();
private slots:
    void calculate_dinhduong();
    void reset_value();
    void tim_phuong_an();
private:
    Ui::Home* ui;
    //for class numeric
    double p_min, p_max, l_min, l_max, c_min, c_max;
};

#endif //KHAUPHANDINHDUONG_HOME_HPP
