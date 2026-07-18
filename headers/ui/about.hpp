#ifndef KHAUPHANDINHDUONG_ABOUT_HPP
#define KHAUPHANDINHDUONG_ABOUT_HPP

#include "ui_about.h"

class AboutWidget : public QWidget {
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = nullptr);
    ~AboutWidget() override;
private slots:
private:
    Ui::About* ui;
};

#endif //KHAUPHANDINHDUONG_ABOUT_HPP
