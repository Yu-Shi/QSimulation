#ifndef PIPEBUTTON_H
#define PIPEBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "utili.h"

using namespace std;

class PipeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PipeButton(QWidget *parent = nullptr);
    void update_font();
    void toggle();
    bool is_checked() const;
    void set_checked(bool);
    void set_existed(bool);
    bool get_existed();
    //void set_parent_area(PaintArea *area);
    //PaintArea *get_parent_area() const;
    int get_id() const;
    void set_id(int _id);

    vector<PipeButton *> getMates() const;
    void setMates(const vector<PipeButton *> &value);

    Utili::dir_type getDir() const;
    void setDir(const Utili::dir_type &value);

    void set_color(double c, Utili::simulation_mode m);

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *e);
    //void keyPressEvent(QKeyEvent *e);

signals:
    //void selected(bool existed);
    void btn_selected(int index, bool existed);

public slots:

private:
    bool checked;
    bool existed;
    Utili::dir_type dir;
    vector<PipeButton *> mates;
    //PaintArea *parent_area;
    int id;
    QString _color;
};

#endif // PIPEBUTTON_H
