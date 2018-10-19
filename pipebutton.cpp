#include "pipebutton.h"
#include <QPainter>
#include <QStyleOption>
#include <iostream>
#include <QEvent>
#include <QKeyEvent>

PipeButton::PipeButton(QWidget *parent) : QPushButton(parent), checked(false), existed(true), _color("#ccc")
{
    setFocusPolicy(Qt::NoFocus);
    //setFocusPolicy(Qt::StrongFocus);
}

void PipeButton::toggle()
{
    set_checked(!checked);
}

bool PipeButton::is_checked() const
{
    return checked;
}

void PipeButton::set_checked(bool checked)
{
    this->checked = checked;
    /*QPainter p(this);
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::black);
    p.drawRect(rect());*/
    //p.
    if (checked)
    {
        setStyleSheet(
                    "QPushButton{ "
                            "border:none;"
                            "background-color: #111; }"
                    "QPushButton:pressed {"
                            "border:none;"
                            "background-color: #111; }"
                    "QPushButton:hover:!pressed {"
                            "border:none;"
                            "background-color: #111; }"
        );
        //get_parent_area()->set_selected_item(id);
        //emit selected(existed);
        for (auto pipe : mates)
        {
            if (pipe != this)
            {
                pipe->set_checked(false);
            }
        }
        raise();
        //std::cout <<

        emit btn_selected(id, existed);
    }
    else
    {
        //qDebug("false");
        //setStyleSheet("");
        if (existed)
        {
            setStyleSheet(
                        "QPushButton{ "
                                "border:none;"
                                "background-color: " + _color + "; }"
                        "QPushButton:pressed {"
                                "border:none;"
                                "background-color: #555; }"
                        "QPushButton:hover:!pressed {"
                                "border:none;"
                                "background-color: #aaa; }"
            );
        }
        else
        {
            setStyleSheet(
                        "QPushButton{ "
                                "border:none;"
                                "background-color: #F0F0F0; }"
                        "QPushButton:pressed {"
                                "border:none;"
                                "background-color: #555; }"
                        "QPushButton:hover:!pressed {"
                                "border:none;"
                                "background-color: #aaa; }"
            );
        }

        emit btn_selected(-1, existed);
    }
    //update_style();
}

void PipeButton::set_existed(bool existed)
{
    this->existed = existed;
    if (existed)
    {

    }
    else
    {

    }
}

bool PipeButton::get_existed()
{
    return existed;
}


int PipeButton::get_id() const
{
    return id;
}

void PipeButton::set_id(int _id)
{
    id = _id;
}

void PipeButton::mouseReleaseEvent(QMouseEvent *)
{
    toggle();
}

void PipeButton::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QPushButton::paintEvent(e);
}
/*
void PipeButton::keyPressEvent(QKeyEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_e = static_cast<QKeyEvent *>(e);
        if (key_e->key() == Qt::Key_Delete)
        {
            set_checked(false);
            //return true;
        }
    }
    else
        QPushButton::keyPressEvent(e);
    //return QWidget::event(e);
}*/

Utili::dir_type PipeButton::getDir() const
{
    return dir;
}

void PipeButton::setDir(const Utili::dir_type &value)
{
    dir = value;
}

void PipeButton::set_color(double c, Utili::simulation_mode m)
{
    QColor color;
    if (m == Utili::MODE_WIDTH_ADJUSTABLE)
    {
        c /= 200;
    }
    color.setGreen(255 - c * 255);
    color.setBlue(255 - c * 255);
    color.setRed(255);
    _color = color.name();

    //setStyleSheet("background-color: rgb(" + QString::number(color.red()) + "," + QString::number(color.green()) + "," + QString::number(color.blue()) + ")");
}

vector<PipeButton *> PipeButton::getMates() const
{
    return mates;
}

void PipeButton::setMates(const vector<PipeButton *> &value)
{
    mates = value;
}
