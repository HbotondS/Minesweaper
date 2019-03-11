#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGridLayout *btnLayout = new QGridLayout(ui->centralWidget);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            btns[i][j] = new QPushButton();
            btns[i][j]->setMinimumSize(50, 50);
            btnLayout->addWidget(btns[i][j], i, j);
            connect(btns[i][j], &QPushButton::clicked, [this, i, j]{btn_action(i, j);});
        }
    }
    this->setFixedSize(this->minimumSize());
    ui->centralWidget->setLayout(btnLayout);

    generateMines();
    countMines();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateMines()
{
    srand(time(NULL));
    for (int i = 0; i < 10; ++i)
    {
        int j = rand() % 10;
        mines[i][j] = -1;
    }
}

void MainWindow::countMines()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (mines[i][j] != -1)
            {
                if (mines[i-1][j-1] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i-1][j] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i-1][j+1] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i][j-1] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i][j+1] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i+1][j-1] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i+1][j] == -1)
                {
                    ++mines[i][j];
                }
                if (mines[i+1][j+1] == -1)
                {
                    ++mines[i][j];
                }
            }
        }
    }
}

void MainWindow::showMines()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (mines[i][j] == -1)
            {
                btns[i][j]->setText(QString::number(mines[i][j]));
                break;
            }
        }
    }
}

void MainWindow::clearField(int x, int y)
{
    if (mines[x][y] == 0 && btns[x][y]->isEnabled())
    {
        btns[x][y]->setDisabled(true);
        if ((x-1) > -1)
            clearField(x-1, y);
        if ((y-1) > -1)
            clearField(x, y-1);
        if ((x+1) < 10)
            clearField(x+1, y);
        if ((y+1) < 10)
            clearField(x, y+1);
    }
    else
    {
        if (mines[x][y] != 0)
        {
            btns[x][y]->setDisabled(true);
            btns[x][y]->setText(QString::number(mines[x][y]));
        }
    }
}

void MainWindow::btn_action(int x, int y)
{
    auto isMine = mines[x][y];
    switch (isMine)
    {
        case -1:
        {
            showMines();
            break;
        }
        case 0:
        {
            clearField(x, y);
            break;
        }
        default:
        {
            btns[x][y]->setText(QString::number(isMine));
            btns[x][y]->setDisabled(true);
        }
    }
}
