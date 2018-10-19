#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialognew.h"
#include "paintarea.h"
#include "pipebutton.h"
#include "core.h"
#include "utili.h"
#include "dialoganneal.h"
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <vector>
#include <iostream>
#include <QSpacerItem>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), have_run(false), _num(5), _i1(0), _i2(1), _o1(0), _o2(1), _o3(2)
{
    ui->setupUi(this);
    window = new QWidget;
    setCentralWidget(window);

    layout = new QHBoxLayout(window);
    area = new PaintArea(window);
    layout->addWidget(area);

    QLabel *right_background = new QLabel(window);
    right_background->setStyleSheet("background-color: #333;"
                                  "color: #eee;"
                                  );
    right_background->setMinimumWidth(150);
    right_layout = new QVBoxLayout(right_background);
    QLabel *labelTitle = new QLabel("控制面板");
    labelTitle->setStyleSheet("background-color: #666;"
                              "color: #eee;"
                              );
    right_layout->setContentsMargins(8, 8, 8, 8);
    QFont font;
    font.setPointSize(12);
    labelTitle->setFont(font);
    labelTitle->setAlignment(Qt::AlignCenter);
    right_layout->addWidget(labelTitle);
    qDebug("1");
    checkbox = new QCheckBox(window);
    checkbox->setText(tr("存在此管道"));
    checkbox->setMinimumHeight(40);
    checkbox->setEnabled(false);
    right_layout->addWidget(checkbox);

    label_con = new QLabel(window);
    label_con->setFont(font);
    label_con->setText("");
    right_layout->addWidget(label_con);

    QLabel *label1 = new QLabel("宽度: ");
    QLabel *label2 = new QLabel("um");
    spin_width = new QSpinBox(window);
    spin_width->setMaximum(1600);
    spin_width->setMinimum(20);
    spin_width->setSingleStep(10);
    spin_width->setEnabled(false);
    qDebug("2");
    //spin_width->set
    width_layout = new QHBoxLayout;
    width_layout->addWidget(label1);
    width_layout->addWidget(spin_width);
    width_layout->addWidget(label2);
    connect(spin_width, SIGNAL(valueChanged(int)), this, SLOT(width_changed(int)));
    qDebug("3");
    right_layout->addLayout(width_layout);
    labelO1 = new QLabel(window);
    labelO2 = new QLabel(window);
    labelO3 = new QLabel(window);
    labelO1->setText("<font size=\"7\"><b>O1</b></font><br />");
    labelO2->setText("<font size=\"7\"><b>O2</b></font><br />");
    labelO3->setText("<font size=\"7\"><b>O3</b></font><br />");
    right_layout->addWidget(labelO1);
    right_layout->addWidget(labelO2);
    right_layout->addWidget(labelO3);

    //QSpacerItem *space = new QSpacerItem()

    layout->addWidget(right_background);
    qDebug("4");
    action_new = new QAction(this);
    action_new->setText(tr("新建"));
    action_new->setShortcut(QKeySequence("Ctrl+N"));
    action_new->setIcon(QIcon(":/images/new"));
    action_new->setStatusTip("创建微流控芯片");
    action_run = new QAction(this);
    action_run->setText(tr("运行"));
    action_run->setShortcut(QKeySequence("Ctrl+R"));
    action_run->setIcon(QIcon(":/images/run"));
    action_run->setStatusTip("运行模拟");
    action_anneal = new QAction(this);
    action_anneal->setText("自动设计芯片");
    action_anneal->setIcon(QIcon(":/images/ai"));
    action_anneal->setStatusTip("输入期望的流速，基于当前设计，自动调整芯片结构");
    ui->mainToolBar->addAction(action_new);
    ui->mainToolBar->addAction(action_run);
    ui->mainToolBar->addAction(action_anneal);
    connect(action_new, SIGNAL(triggered(bool)), this, SLOT(newSimulation()));
    connect(action_run, SIGNAL(triggered(bool)), this, SLOT(runSimulation()));
    connect(action_anneal, SIGNAL(triggered(bool)), this, SLOT(Annealing()));
    qDebug("5");
    area->setFixedSize(6 * (CONNECTOR_SIZE + PIPE_LENGTH), 6 * (CONNECTOR_SIZE + PIPE_LENGTH));
    //setFixedSize(window->minimumSizeHint() + ui->menuBar->minimumSizeHint());
    connect(area, SIGNAL(selected(bool, int, int)), this, SLOT(set_checkbox_status(bool, int, int)));
    connect(area, SIGNAL(unselected()), this, SLOT(on_unselected()));
    connect(checkbox, SIGNAL(toggled(bool)), area, SLOT(set_status(bool)));
    qDebug("6");

    setWindowTitle("QSimulation");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newSimulation()
{
    DialogNew dlg(this);
    dlg.set_default_value(_num, _i1, _i2, _o1, _o2, _o3);
    if (dlg.exec() == QDialog::Accepted)
    {
        int num = dlg.number();
        int i1 = dlg.geti1();
        int i2 = dlg.geti2();
        int o1 = dlg.geto1();
        int o2 = dlg.geto2();
        int o3 = dlg.geto3();
        Utili::simulation_mode mode = dlg.getMode();
        _num = num; _i1 = i1; _i2 = i2; _o1 = o1;
        _o2 = o2; _o3 = o3; _mode = mode;
        area->paint(num, i1, i2, o1, o2, o3, mode);
        label_con->setText("");
        /*if (mode == Utili::MODE_WIDTH_ADJUSTABLE)
        {
            spin_width->setEnabled(true);
        }
        else
            spin_width->setEnabled(false);*/
        have_run = false;
        action_anneal->setEnabled(!(_mode == Utili::MODE_WIDTH_NOT_ADJUSTABLE));
    }
}

void MainWindow::runSimulation()
{
    if (area->getFirst_run())
    {
        int ret = QMessageBox::warning(this, tr("QSimulation"), tr("No chip has been created. Creat one?"),
                                       QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            newSimulation();
        }
    }
    else
    {
        std::vector<int> widths;
        widths = area->getWidths();
        std::vector<double> simu_len;
        int j = 0;
        std::vector<PipeButton *> button;
        button = area->getPipes();
        for (auto i : widths)
        {
            std::cout << i << " ";
            double t = (double) DEFAULT_WIDTH / i;
            //t *= t;
            if (button[j]->get_existed())
                simu_len.push_back(t);
            else
                simu_len.push_back(0);
            j++;
        }
        std::cout << std::endl;
        QDateTime time_a= QDateTime::currentDateTime();
        std::vector<double> ans = caluconspeed(_num, simu_len, _i1, _i2, _o1, _o2, _o3);
        QDateTime time_b= QDateTime::currentDateTime();
        qDebug() << time_a.msecsTo(time_b);

        _ans1 = ans[0];
        _ans2 = ans[1];
        _ans3 = ans[2];

        if (_mode == Utili::MODE_WIDTH_NOT_ADJUSTABLE)
        {
            getCon();
            vector<double> con;
            for (int i = 0; i < EDGESUM; i++)
            {
                con.push_back(edges[i].c);
            }
            area->set_color(con, Utili::MODE_WIDTH_NOT_ADJUSTABLE);
            labelO1->setText(R"(<font size="7"><b>O1</b></font><br /><font size="5">)" + QString::number(ans[0], 'g', 3)
                    + "/" + QString::number(edges[EDGESUM - 3].c, 'g', 3) + "</font>");
            labelO2->setText(R"(<font size="7"><b>O2</b></font><br /><font size="5">)" + QString::number(ans[1], 'g', 3)
                    + "/" + QString::number(edges[EDGESUM - 2].c, 'g', 3) + "</font>");
            labelO3->setText(R"(<font size="7"><b>O3</b></font><br /><font size="5">)" + QString::number(ans[2], 'g', 3)
                    + "/" + QString::number(edges[EDGESUM - 1].c, 'g', 3) + "</font>");
        }
        else
        {
            vector<double> v;
            for (int i = 0; i < EDGESUM; i++)
            {
                v.push_back(edges[i].v);
            }
            area->set_color(v, Utili::MODE_WIDTH_ADJUSTABLE);
            labelO1->setText("<font size=\"7\"><b>O1</b></font><br /><font size=\"5\">" + QString::number(ans[0], 'g', 3) + "</font>");
            labelO2->setText("<font size=\"7\"><b>O2</b></font><br /><font size=\"5\">" + QString::number(ans[1], 'g', 3) + "</font>");
            labelO3->setText("<font size=\"7\"><b>O3</b></font><br /><font size=\"5\">" + QString::number(ans[2], 'g', 3) + "</font>");
        }
        //label_con->setVisible(true);
        label_con->setText("");
        //set_checkbox_status(area->get_selected_item());
        have_run = true;
        area->unselect_all();
    }

}

void MainWindow::set_checkbox_status(bool status, int width, int index)
{
    if (_mode == Utili::MODE_WIDTH_ADJUSTABLE)
    {
        spin_width->setEnabled(true);
        if (have_run)
        {
            label_con->setText("流速: " + QString::number(edges[index].v, 'g', 3));
            label_con->setVisible(true);
        }
    }
    if (_mode == Utili::MODE_WIDTH_NOT_ADJUSTABLE && have_run)
    {
        label_con->setText("浓度: " + QString::number(edges[index].c, 'g', 3) + "\n流速: " + QString::number(edges[index].v, 'g', 3));
        label_con->setVisible(true);
    }
    std::cout << edges[index].leng << endl;
    checkbox->setEnabled(true);
    checkbox->setChecked(status);
    spin_width->setValue(width);
}

void MainWindow::width_changed(int value)
{
    area->set_width(value);
}

void MainWindow::on_unselected()
{
    checkbox->setEnabled(false);
    spin_width->setEnabled(false);
    label_con->setText("");
}

void MainWindow::Annealing()
{
    if (area->getFirst_run())
    {
        int ret = QMessageBox::warning(this, tr("QSimulation"), tr("No chip has been created. Creat one?"),
                                       QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            newSimulation();
        }
        return;
    }
    DialogAnneal dlg(this);
    dlg.set_default_value(static_cast<int>(_ans1), static_cast<int>(_ans2), static_cast<int>(_ans3));
    if (dlg.exec() == QDialog::Accepted)
    {
        //dlg.set_default_value(_ans1, _ans2, _ans3);
        int a1 = dlg.get_expected_1();
        int a2 = dlg.get_expected_2();
        int a3 = dlg.get_expected_3();
        cout << "a1" << a1 << " a2" << a2 << " a3" << a3 << endl;
        vector<int> widths;
        widths = area->getWidths();
        vector<double> simu_len;
        vector<PipeButton *> button;
        button = area->getPipes();
        int j = 0;
        for (auto i : widths)
        {
            double t = (double) DEFAULT_WIDTH / i;
            if (button[j]->get_existed())
                simu_len.push_back(t);
            else
                simu_len.push_back(0);
            j++;
        }
        double _min = 10000;
        vector<double> ans_can;
        for (int i = 0; i < 3; i++)
        {
            vector<double> ans;
            double r;
            r = annealing(_num, a1, a2, a3, _i1, _i2, _o1, _o2, _o3, simu_len, ans);
            if (!ans.empty())
                simu_len = ans;
            cout << "r=" << r << endl;
            if (r < _min)
            {
                _min = r;
                ans_can = ans;
            }
        }
        j = 0;
        cout << "_min " << _min << endl;
        for (auto i : ans_can)
        {
            if (i != 0)
            {
                int w = DEFAULT_WIDTH / i;
                area->set_width(w, j);
            }
            else
            {
                area->set_status(false, j);
            }
            j++;
        }
        runSimulation();
    }
}
