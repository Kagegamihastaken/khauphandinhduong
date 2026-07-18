#include "ui/home.hpp"

#include "common/database.hpp"
#include "common/logging.hpp"
#include "common/solver.hpp"
#include <magic_enum/magic_enum.hpp>
#include <model/HighsModel.h>

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Home) {
    ui->setupUi(this);
    connect(ui->calories_find_dd, &QPushButton::clicked, this, &HomeWidget::calculate_dinhduong);
    connect(ui->calories_reset, &QPushButton::clicked, this, &HomeWidget::reset_value);
    connect(ui->tim_phuong_an_toi_uu, &QPushButton::clicked, this, &HomeWidget::tim_phuong_an);
    //Init window
    reset_value();
    p_min = 0.0;
    p_max = 0.0;
    l_min = 0.0;
    l_max = 0.0;
    c_min = 0.0;
    c_max = 0.0;
}

void HomeWidget::reset_value() {
    MFCPP::Log::InfoPrint("Resetting values...");
    ui->calories_num_min->setValue(2000.0);
    ui->calories_num_max->setValue(2500.0);
    ui->c_min->setText("NAN");
    ui->c_max->setText("NAN");
    ui->l_min->setText("NAN");
    ui->l_max->setText("NAN");
    ui->p_max->setText("NAN");
    ui->p_min->setText("NAN");
    ui->tim_phuong_an_toi_uu->setEnabled(false);
    ui->calories_reset->setEnabled(false);
    ui->calories_find_dd->setEnabled(true);
    ui->calories_num_min->setEnabled(true);
    ui->calories_num_max->setEnabled(true);
}

HomeWidget::~HomeWidget() {
    delete ui;
}

void HomeWidget::calculate_dinhduong() {
    MFCPP::Log::InfoPrint("Calculating dinh duong...");
    p_min = ui->calories_num_min->value() * 0.1 / 4.0;
    p_max = ui->calories_num_max->value() * 0.35 / 4.0;
    l_min = ui->calories_num_min->value() * 0.2 / 9.0;
    l_max = ui->calories_num_max->value() * 0.3 / 9.0;
    c_min = ui->calories_num_min->value() * 0.45 / 4.0;
    c_max = ui->calories_num_max->value() * 0.65 / 4.0;
    //Change state
    if (!ui->tim_phuong_an_toi_uu->isEnabled()) ui->tim_phuong_an_toi_uu->setEnabled(true);
    ui->calories_reset->setEnabled(true);
    ui->calories_find_dd->setEnabled(false);
    ui->calories_num_min->setEnabled(false);
    ui->calories_num_max->setEnabled(false);
    //Set text
    ui->p_min->setText(QString::number(p_min, 'f', 2));
    ui->p_max->setText(QString::number(p_max, 'f', 2));
    ui->l_min->setText(QString::number(l_min, 'f', 2));
    ui->l_max->setText(QString::number(l_max, 'f', 2));
    ui->c_min->setText(QString::number(c_min, 'f', 2));
    ui->c_max->setText(QString::number(c_max, 'f', 2));
}

void HomeWidget::tim_phuong_an() {
    std::vector<int64_t> ans;
    HighsModel model;
    MFCPP::Database::setAllFoodSelected(true);
    MFCPP::Solver::Generate(model, MFCPP::Solver::Boundary{ui->calories_num_min->value(), ui->calories_num_max->value(), p_min, p_max, l_min, l_max, c_min, c_max});
    int cnt = 0;
    double cost = 0.0;
    if (MFCPP::Solver::Solve(ans, model)) {
        constexpr auto food_entries = magic_enum::enum_entries<FoodID>();
        for (int i = 0; i < food_entries.size(); ++i) {
            if (food_entries[i].first == FoodID::FOOD_NULL) continue;
            if (ans[cnt] == 0) {
                ++cnt;
                continue;
            }
            MFCPP::Log::InfoPrint(fmt::format("{}x {}", ans[cnt], MFCPP::Database::getFood(food_entries[i].first).name));
            cost += ans[cnt] * MFCPP::Database::getFood(food_entries[i].first).price;
            ++cnt;
        }
        MFCPP::Log::InfoPrint(fmt::format("Price: {}k", cost));
    }

    reset_value();
}
