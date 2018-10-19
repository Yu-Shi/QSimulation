#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QAction>
#include "paintarea.h"
#include "dialognew.h"
#include "utili.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //void paintEvent(QPaintEvent *);

private slots:
    void newSimulation();
    void runSimulation();
    void set_checkbox_status(bool status, int width, int index);
    void width_changed(int value);
    void on_unselected();
    void Annealing();

private:
    Ui::MainWindow *ui;
    QWidget *window;
    QHBoxLayout *layout;
    PaintArea *area;
    QVBoxLayout *right_layout;
    QCheckBox *checkbox;
    QSpinBox *spin_width;
    QHBoxLayout *width_layout;
    QLabel *labelO1;
    QLabel *labelO2;
    QLabel *labelO3;
    QLabel *label_con;
    QAction *action_new;
    QAction *action_run;
    QAction *action_anneal;
    //DialogNew *newdlg;
    int _num;
    int _i1;
    int _i2;
    int _o1;
    int _o2;
    int _o3;
    double _ans1;
    double _ans2;
    double _ans3;
    Utili::simulation_mode _mode;
    bool have_run;
};

#endif // MAINWINDOW_H
