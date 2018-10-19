#include "paintarea.h"
#include "pipebutton.h"
#include "utili.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent), selected_item(-1), first_run(true), num(0)
{
    setStyleSheet(
        "QPushButton{ "
                "border:none;"
                "background-color: #ccc; }"
        "QPushButton:pressed {"
                "border:none;"
                "background-color: #555; }"
        "QPushButton:hover:!pressed {"
                "border:none;"
                "background-color: #aaa; }"
    );
    emit unselected();
    setFocusPolicy(Qt::ClickFocus);
}

void PaintArea::paint(int num, int i1, int i2, int o1, int o2, int o3, Utili::simulation_mode mode)
{
    if (!first_run)
    {
        for (auto i : pipes)
            delete i;
        pipes.clear();
        widths.clear();
    }
    _mode = mode;
    this->num = num;
    for (int i = 0, x = START_POS; i < num; i++, x += INTERVAL)
    {
        for (int j = 0, p = START_POS + CONNECTOR_SIZE; j < num - 1; j++, p += INTERVAL)
        {
            PipeButton *pipe = new PipeButton(this);
            //pipe->setFixedSize(10, 80);
            pipe->setGeometry(x, p, CONNECTOR_SIZE, PIPE_LENGTH);
            pipe->setDir(Utili::PIPE_VER);
            pipes.push_back(pipe);
            pipe->show();
        }
    }

    for (int i = 0, x = START_POS + CONNECTOR_SIZE; i < num - 1; i++, x += INTERVAL)
    {
        for (int j = 0, p = START_POS; j < num; j++, p += INTERVAL)
        {
            PipeButton *pipe = new PipeButton(this);
            pipe->setGeometry(x, p, PIPE_LENGTH, CONNECTOR_SIZE);
            pipe->setDir(Utili::PIPE_HOR);
            pipes.push_back(pipe);
            pipe->show();
        }
    }


    update();
    PipeButton *pipei1 = new PipeButton(this);
    pipei1->setGeometry(INTERVAL * i1 + START_POS, START_POS - PIPE_LENGTH, CONNECTOR_SIZE, PIPE_LENGTH);
    pipei1->setDir(Utili::PIPE_VER);
    PipeButton *pipei2 = new PipeButton(this);
    pipei2->setGeometry(INTERVAL * i2 + START_POS, START_POS - PIPE_LENGTH, CONNECTOR_SIZE, PIPE_LENGTH);
    pipei2->setDir(Utili::PIPE_VER);
    pipei1->show();
    pipei2->show();

    PipeButton *pipeo1 = new PipeButton(this);
    pipeo1->setGeometry(INTERVAL * o1 + START_POS, (num - 1) * INTERVAL + START_POS + CONNECTOR_SIZE, CONNECTOR_SIZE, PIPE_LENGTH);
    pipeo1->setDir(Utili::PIPE_VER);
    pipeo1->show();
    PipeButton *pipeo2 = new PipeButton(this);
    pipeo2->setGeometry(INTERVAL * o2 + START_POS, (num - 1) * INTERVAL + START_POS + CONNECTOR_SIZE, CONNECTOR_SIZE, PIPE_LENGTH);
    pipeo2->setDir(Utili::PIPE_VER);
    pipeo2->show();
    PipeButton *pipeo3 = new PipeButton(this);
    pipeo3->setGeometry(INTERVAL * o3 + START_POS, (num - 1) * INTERVAL + START_POS + CONNECTOR_SIZE, CONNECTOR_SIZE, PIPE_LENGTH);
    pipeo3->setDir(Utili::PIPE_VER);
    pipeo3->show();

    pipes.push_back(pipei1);
    pipes.push_back(pipei2);
    pipes.push_back(pipeo1);
    pipes.push_back(pipeo2);
    pipes.push_back(pipeo3);

    int i = 0;
    for (auto pipe : pipes)
    {
        pipe->set_id(i);
        connect(pipe, SIGNAL(btn_selected(int, bool)), this, SLOT(set_select_status(int,bool)));
        pipe->setMates(pipes);
        widths.push_back(DEFAULT_WIDTH);
        i++;
    }

    first_run = false;

    setFixedSize((num + 1) * (CONNECTOR_SIZE + PIPE_LENGTH), (num + 2) * (CONNECTOR_SIZE + PIPE_LENGTH));
    emit unselected();
}

int PaintArea::get_selected_item() const
{
    return selected_item;
}

void PaintArea::set_selected_item(int index)
{
    selected_item = index;
    qDebug("hello");
}

void PaintArea::set_width(int width)
{
    widths[selected_item] = width;
    auto p = pipes[selected_item];
    if (p->getDir() == Utili::PIPE_VER) // vertical pipe
    {
        int x = p->x() + p->width() / 2; // center
        int w = width / (DEFAULT_WIDTH / CONNECTOR_SIZE); // scailing
        x -= w / 2;
        p->setGeometry(x, p->y(), w, p->height());
    }
    else // horizontal pipe
    {
        int y = p->y() + p->height() / 2;
        int h = width / (DEFAULT_WIDTH / CONNECTOR_SIZE);
        y -= h / 2;
        p->setGeometry(p->x(), y, p->width(), h);
    }
}

void PaintArea::set_width(int width, int index)
{
    widths[index] = width;
    auto p = pipes[index];
    if (p->getDir() == Utili::PIPE_VER) // vertical pipe
    {
        int x = p->x() + p->width() / 2; // center
        int w = width / (DEFAULT_WIDTH / CONNECTOR_SIZE); // scailing
        x -= w / 2;
        p->setGeometry(x, p->y(), w, p->height());
    }
    else // horizontal pipe
    {
        int y = p->y() + p->height() / 2;
        int h = width / (DEFAULT_WIDTH / CONNECTOR_SIZE);
        y -= h / 2;
        p->setGeometry(p->x(), y, p->width(), h);
    }
}

void PaintArea::set_select_status(int index, bool status)
{
    if (index == -1)
    {
        emit unselected();
        return;
    }
    selected_item = index;

    emit selected(status, widths[index], index);

}

void PaintArea::set_status(bool status)
{
    pipes[selected_item]->set_existed(status);
}

void PaintArea::set_status(bool status, int index)
{
    pipes[index]->set_existed(status);
}

vector<PipeButton *> PaintArea::getPipes() const
{
    return pipes;
}

void PaintArea::set_color(vector<double> &_con, Utili::simulation_mode m)
{
    con = _con;
    int i = 0;
    for (auto pipe : pipes)
    {
        pipe->set_color(con[i], m);
        i++;
    }
    refresh();
}

void PaintArea::refresh()
{
    for (auto pipe : pipes)
    {
        pipe->set_checked(pipe->is_checked());
    }
}

void PaintArea::unselect_all()
{
    for (auto pipe : pipes)
    {
        pipe->set_checked(false);
    }
}

bool PaintArea::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        auto *key_e = dynamic_cast<QKeyEvent *>(event);
        if (key_e->key() == Qt::Key_Delete)
        {
            //std::cout << "hellodel!" << std::endl;
            if (selected_item != -1)
            {
                pipes[selected_item]->set_existed(false);
                emit selected(false, widths[selected_item], selected_item);
            }
            return true;
        }
        else if (key_e->key() == Qt::Key_Up)
        {
            std::cout << "helloup!" << std::endl;
            if (selected_item > 0)
            {
                pipes[--selected_item]->set_checked(true);
            }
            return true;
        }
        else if ((key_e->key() == Qt::Key_Down))
        {
            if (selected_item < 2 * num * num - 2 * num + 4)
            {
                pipes[++selected_item]->set_checked(true);
            }
            return true;
        }
        else if (key_e->key() == Qt::Key_Left)
        {
            if (pipes[selected_item]->getDir() == Utili::PIPE_HOR)
            {
                if (selected_item - num >= 0)
                {
                    selected_item -= num;
                    pipes[selected_item]->set_checked(true);
                }
            }
            else
            {
                if (selected_item - num + 1 >= 0)
                {
                    selected_item -= (num - 1);
                    pipes[selected_item]->set_checked(true);
                }
            }
            return true;
        }
        else if (key_e->key() == Qt::Key_Right)
        {
            if (pipes[selected_item]->getDir() == Utili::PIPE_HOR)
            {
                if (selected_item + num <= 2 * num * num - 2 * num + 4)
                {
                    selected_item += num;
                    pipes[selected_item]->set_checked(true);
                }
            }
            else
            {
                if (selected_item + num - 1 <= 2 * num * num - 2 * num + 4)
                {
                    selected_item += (num - 1);
                    pipes[selected_item]->set_checked(true);
                }
            }
            return true;
        }
    }
    return QWidget::event(event);
    //return QWidget::event(e);
}

vector<int> PaintArea::getWidths() const
{
    return widths;
}

bool PaintArea::eventFilter(QObject *watched, QEvent *event)
{
    for (auto pipe : pipes)
    {
        if (pipe != watched)
        {
            if (event->type() == QEvent::MouseButtonRelease)
            {
                qDebug("hello");
                pipe->setChecked(false);
            }
        }
        //return QWidget::eventFilter(watched, event);
    }
    return QWidget::eventFilter(watched, event);
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPen pen(Qt::NoPen);
    //QBrush brush()
    p.setPen(pen);
    QColor color;
    color.setNamedColor("#ccc");
    QBrush brush(color);
    p.setBrush(brush);

    for (int i = 0, x = START_POS; i < num; i++, x += INTERVAL)
    {
        for (int j = 0, y = START_POS; j < num; j++, y += INTERVAL)
        {
            p.drawRect(x, y, CONNECTOR_SIZE, CONNECTOR_SIZE);
        }
    }
}

bool PaintArea::getFirst_run() const
{
    return first_run;
}
