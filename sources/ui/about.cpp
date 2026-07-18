#include "ui/about.hpp"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::About) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &AboutWidget::close);
}

AboutWidget::~AboutWidget() {
    delete ui;
}
