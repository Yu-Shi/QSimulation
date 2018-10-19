#include "dialoganneal.h"
#include "ui_dialoganneal.h"

DialogAnneal::DialogAnneal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAnneal)
{
    ui->setupUi(this);
    setWindowTitle("输入期望的流速");
}

DialogAnneal::~DialogAnneal()
{
    delete ui;
}

void DialogAnneal::set_default_value(int o1, int o2, int o3)
{
    ui->spinBoxO1->setValue(o1);
    ui->spinBoxO2->setValue(o2);
    ui->spinBoxO3->setValue(o3);
}

int DialogAnneal::get_expected_1()
{
    return ui->spinBoxO1->value();
}

int DialogAnneal::get_expected_2()
{
    return ui->spinBoxO2->value();
}

int DialogAnneal::get_expected_3()
{
    return ui->spinBoxO3->value();
}
