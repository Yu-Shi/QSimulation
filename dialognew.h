#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>
#include "utili.h"

namespace Ui {
class DialogNew;
}

class DialogNew : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNew(QWidget *parent = nullptr);
    int number();
    int geti1();
    int geti2();
    int geto1();
    int geto2();
    int geto3();
    void set_default_value(int num, int i1, int i2, int o1, int o2, int o3);
    Utili::simulation_mode getMode() const;

    ~DialogNew() override;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_spin_i1_valueChanged(int arg1);

    void on_spin_i2_valueChanged(int arg1);

    void on_spin_o1_valueChanged(int arg1);

    void on_spin_o2_valueChanged(int arg1);

    void on_spin_o3_valueChanged(int arg1);

private:
    Ui::DialogNew *ui;
};

#endif // DIALOGNEW_H
