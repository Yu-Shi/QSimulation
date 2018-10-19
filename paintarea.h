#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "utili.h"

using namespace std;

class PipeButton;

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);
    void paint(int num, int i1, int i2, int o1, int o2, int o3, Utili::simulation_mode);
    int get_selected_item() const;
    void set_selected_item(int index);
    void set_width(int width);
    void set_width(int width, int index);

    bool getFirst_run() const;

    vector<int> getWidths() const;

    vector<PipeButton *> getPipes() const;

    void set_color(vector<double> &_con, Utili::simulation_mode m);

    void refresh();

    void unselect_all();
protected:
    bool event(QEvent *event);

signals:
    void selected(bool, int, int);
    void unselected();
    void con_got(double);

public slots:
    void set_select_status(int, bool);
    void set_status(bool);
    void set_status(bool, int);

private:
    //QPushButton *btn2;
    vector<PipeButton *> pipes;
    vector<int> widths;
    vector<double> con;
    int selected_item;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    int num;
    bool first_run;
    Utili::simulation_mode _mode;
};

#endif // PAINTAREA_H
