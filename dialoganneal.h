#ifndef DIALOGANNEAL_H
#define DIALOGANNEAL_H

#include <QDialog>

namespace Ui {
class DialogAnneal;
}

class DialogAnneal : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAnneal(QWidget *parent = 0);
    ~DialogAnneal();
    void set_default_value(int o1, int o2, int o3);
    int get_expected_1();
    int get_expected_2();
    int get_expected_3();

private:
    Ui::DialogAnneal *ui;
};

#endif // DIALOGANNEAL_H
