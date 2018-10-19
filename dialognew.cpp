#include "dialognew.h"
#include "ui_dialognew.h"
#include <QPushButton>

DialogNew::DialogNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNew)
{
    ui->setupUi(this);
    ui->lblWarning->setStyleSheet("color: red;");
    ui->lblWarning->setText("输入/输出管道不可在同一列，请调整");
    ui->lblWarning->setVisible(false);
    setWindowTitle("新建芯片");
}

int DialogNew::number()
{
    return ui->spinBox->value();
}

int DialogNew::geti1()
{
    return ui->spin_i1->value();
}

int DialogNew::geti2()
{
    return ui->spin_i2->value();
}

int DialogNew::geto1()
{
    return ui->spin_o1->value();
}

int DialogNew::geto2()
{
    return ui->spin_o2->value();
}

int DialogNew::geto3()
{
    return ui->spin_o3->value();
}

void DialogNew::set_default_value(int num, int i1, int i2, int o1, int o2, int o3)
{
    ui->spinBox->setValue(num);
    ui->spin_i1->setValue(i1);
    ui->spin_i2->setValue(i2);
    ui->spin_o1->setValue(o1);
    ui->spin_o2->setValue(o2);
    ui->spin_o3->setValue(o3);
    on_spinBox_valueChanged(num);
}

Utili::simulation_mode DialogNew::getMode() const
{
    if (ui->radioSpeed->isChecked())
    {
        return Utili::MODE_WIDTH_ADJUSTABLE;
    }
    else
    {
        return Utili::MODE_WIDTH_NOT_ADJUSTABLE;
    }
}

DialogNew::~DialogNew()
{
    delete ui;
}

void DialogNew::on_spinBox_valueChanged(int arg1)
{
    ui->spin_i1->setMaximum(arg1 - 1);
    ui->spin_i2->setMaximum(arg1 - 1);

    ui->spin_o1->setMaximum(arg1 - 1);
    ui->spin_o2->setMaximum(arg1 - 1);
    ui->spin_o3->setMaximum(arg1 - 1);
}

void DialogNew::on_spin_i1_valueChanged(int arg1)
{
    if (arg1 == ui->spin_i2->value())
    {
        ui->lblWarning->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->lblWarning->setVisible(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void DialogNew::on_spin_i2_valueChanged(int arg1)
{
    if (arg1 == ui->spin_i1->value())
    {
        ui->lblWarning->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->lblWarning->setVisible(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void DialogNew::on_spin_o1_valueChanged(int arg1)
{
    if (arg1 == ui->spin_o2->value() || arg1 == ui->spin_o3->value())
    {
        ui->lblWarning->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->lblWarning->setVisible(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void DialogNew::on_spin_o2_valueChanged(int arg1)
{
    if (arg1 == ui->spin_o1->value() || arg1 == ui->spin_o3->value())
    {
        ui->lblWarning->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->lblWarning->setVisible(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void DialogNew::on_spin_o3_valueChanged(int arg1)
{
    if (arg1 == ui->spin_o1->value() || arg1 == ui->spin_o2->value())
    {
        ui->lblWarning->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        ui->lblWarning->setVisible(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
