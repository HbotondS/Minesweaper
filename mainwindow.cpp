#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    connect(ui->action_New_game, &QAction::triggered, [this]{newGame();});
    connect(ui->action_Exit, &QAction::triggered, []{QApplication::quit();});
}

MainWindow::MainWindow(int x, int y):
    ui(new Ui::MainWindow),
    xDimension(x),
    yDimension(y)
{    
    ui->setupUi(this);
    init();
    connect(ui->action_New_game, &QAction::triggered, [this]{newGame();});
    connect(ui->action_Exit, &QAction::triggered, []{QApplication::quit();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    timer = new QTimer();
    elapsedTime = new QElapsedTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));

    memset(mines, 0, sizeof(mines));

    QVBoxLayout* mainLayout = new QVBoxLayout();
    btnLayout = new QGridLayout();
    QHBoxLayout* labelLayout = new QHBoxLayout();

    bombsLabel = new QLabel("10");
    labelLayout->addWidget(bombsLabel);
    timeLabel = new QLabel("");
    timeLabel->setAlignment(Qt::AlignRight);
    labelLayout->addWidget(timeLabel);

    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(btnLayout);


    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < xDimension; ++j)
        {
            btns[i][j] = new QPushButton();
            btns[i][j]->setMinimumSize(40, 40);
            btnLayout->addWidget(btns[i][j], i, j);
            connect(btns[i][j], &QPushButton::clicked, [this, i, j]{btn_action(i, j);});
        }
    }
    ui->centralWidget->setLayout(mainLayout);

    generateMines();

    timer->start();
    elapsedTime->start();
}

void MainWindow::generateMines()
{
    srand(time(NULL));
    for (int i = 0; i < 10; ++i)
    {
        int j = rand() % 10;
        mines[i][j] = -1;
        countMines(i, j);
    }
}

void MainWindow::incNum(int i, int j) {
    if (mines[i][j] != -1) {
        ++mines[i][j];
    }
}

void MainWindow::countMines(int i, int j) {
    if (i-1 >= 0 && j-1 >= 0)
    {
        incNum(i-1, j-1);
    }
    if (i-1 >= 0)
    {
        incNum(i-1, j);
    }
    if (i-1 >= 0 && j+1 <= 9)
    {
        incNum(i-1, j+1);
    }
    if (j-1 >= 0)
    {
        incNum(i, j-1);
    }
    if (j+1 <= 9)
    {
        incNum(i, j+1);
    }
    if (i+1 <= 9 && j-1 >= 0)
    {
        incNum(i+1, j-1);
    }
    if (i+1 <= 9)
    {
        incNum(i+1, j);
    }
    if (i+1 <= 9 && j+1 <= 9)
    {
        incNum(i+1, j+1);
    }
}

void MainWindow::showMines()
{
    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < yDimension; ++j)
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
        btns[x][y]->setStyleSheet("border: none;");
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
            btns[x][y]->setStyleSheet("border: none;");
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
            btns[x][y]->setStyleSheet("color: red;");
            timer->stop();
            showMines();
            QMessageBox messageBox;
            QMessageBox::StandardButton reply;
            reply = messageBox.critical(this, "Game over", "You lost!", QMessageBox::Retry | QMessageBox::Close);
            messageBox.setFixedSize(500,200);

            if (reply == QMessageBox::Retry) {
                newGame();
            } else {
                QApplication::quit();
            }
            break;
        }
        case 0:
        {
            clearField(x, y);
            break;
        }
        default:
        {
            btns[x][y]->setStyleSheet("border: none;");
            btns[x][y]->setText(QString::number(isMine));
            btns[x][y]->setDisabled(true);
        }
    }
}

void MainWindow::update_time()
{
    timeLabel->setText(QString::number(elapsedTime->elapsed()/100));
}

void MainWindow::newGame() {
    QLayoutItem *item;
    QWidget * widget;
    while ((item = btnLayout->takeAt(0))) {
        if ((widget = item->widget()) != nullptr) {
            widget->hide(); delete widget;
        }
        else {
            delete item;
        }
    }
    delete btnLayout;
    delete timer;
    delete elapsedTime;
    init();
}
