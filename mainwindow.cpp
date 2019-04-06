#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    connect(ui->action_New_game, &QAction::triggered, [this]{newGame();});
    connect(ui->action_Restart, &QAction::triggered, [this]{restart();});
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
    connect(ui->action_Restart, &QAction::triggered, [this]{restart();});
    connect(ui->action_Exit, &QAction::triggered, []{QApplication::quit();});
    connect(this, SIGNAL(win()), this, SLOT(winmsg()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {

//    elapsedTime = new QElapsedTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));

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

    newBtns = new QPushButton**[xDimension];
    mines = new int*[xDimension];
    for (int i = 0; i < xDimension; ++i) {
        newBtns[i] = new QPushButton*[yDimension];
        mines[i] = new int[yDimension];
    }
    fillmines();

    generateMines();
    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < yDimension; ++j)
        {
            newBtns[i][j] = new QPushButton();
            newBtns[i][j]->setMinimumSize(40, 40);
            newBtns[i][j]->setText(QString::number(mines[i][j]));
            connect(newBtns[i][j], &QPushButton::clicked, [this, i, j]{btn_action(i, j);});
            connect(newbtns[i][j], &QRightClickButton::rightClicked, [this, i, j]{onRightClicked(i, j);});
            btnLayout->addWidget(newBtns[i][j], i, j);
        }
    }
    ui->centralWidget->setLayout(mainLayout);

//    timer->start();
    //    elapsedTime->start();
}

// for debuging
void MainWindow::printMines() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            qDebug() << mines[i][j] << " ";
        }
        qDebug() << "\n";
    }
}

void MainWindow::newGame()
{
    this->close();
    StartWindow *w = new StartWindow();
    w->show();
}

void MainWindow::generateMines()
{
    srand(time(NULL));
    for (int i = 0; i < xDimension; ++i)
    {
        int j = rand() % yDimension;
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
    if (i-1 >= 0 && j+1 < yDimension)
    {
        incNum(i-1, j+1);
    }
    if (j-1 >= 0)
    {
        incNum(i, j-1);
    }
    if (j+1 < yDimension)
    {
        incNum(i, j+1);
    }
    if (i+1 < xDimension && j-1 >= 0)
    {
        incNum(i+1, j-1);
    }
    if (i+1 < xDimension)
    {
        incNum(i+1, j);
    }
    if (i+1 < xDimension && j+1 < yDimension)
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
                newBtns[i][j]->setText(QString::number(mines[i][j]));
                break;
            }
        }
    }
}

void MainWindow::clearField(int x, int y)
{
    if (mines[x][y] == 0 && newBtns[x][y]->isEnabled())
    {
        newBtns[x][y]->setStyleSheet("border: none;");
        newBtns[x][y]->setDisabled(true);
        if ((x-1) > -1)
            clearField(x-1, y);
        if ((y-1) > -1)
            clearField(x, y-1);
        if ((x+1) < xDimension)
            clearField(x+1, y);
        if ((y+1) < yDimension)
            clearField(x, y+1);
    }
    else
    {
        if (mines[x][y] != 0)
        {
            newBtns[x][y]->setStyleSheet("border: none;");
            newBtns[x][y]->setDisabled(true);
            newBtns[x][y]->setText(QString::number(mines[x][y]));
        }
    }
}

void MainWindow::fillmines()
{
    for (int i = 0; i < xDimension; ++i) {
        for (int j = 0; j < yDimension; ++j) {
            mines[i][j] = 0;
        }
    }
}

void MainWindow::btn_action(int x, int y)
{
    if (btns[x][y]->text() != "B") {
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
                    restart();
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

    if (btnsLeft() == 10) {
        emit win();
    }
}

void MainWindow::update_time()
{
    timeLabel->setText(QString::number(elapsedTime->elapsed()/100));
}

void MainWindow::onRightClicked(int x, int y)
{
    int bombsLeft = bombsLabel->text().toInt();
    if (bombsLeft > 0 && btns[x][y]->text() == "")
    {
        bombsLabel->setText(QString::number(--bombsLeft));
        btns[x][y]->setText("B");
    }
    else
    {
        if (btns[x][y]->text() == "B")
        {
            bombsLabel->setText(QString::number(++bombsLeft));
            btns[x][y]->setText("");
        }
    }
    if (bombsLeft == 0) {
        if (btnsLeft() == 10) {
            emit win();
        }
    }
}

void MainWindow::winmsg()
{
    timer->stop();
    QMessageBox messageBox;
    QMessageBox::StandardButton reply;
    reply = messageBox.critical(this, "Win!", "Your time: " + timeLabel->text(), QMessageBox::Retry | QMessageBox::Close);
    messageBox.setFixedSize(500,200);

    if (reply == QMessageBox::Retry) {
        newGame();
    } else {
        QApplication::quit();
    }
}

void MainWindow::restart() {
    QLayoutItem *item;
    QWidget * widget;
    /*while ((item = btnLayout->takeAt(0))) {
        if ((widget = item->widget()) != nullptr) {
            widget->hide(); delete widget;
        }
        else {
            delete item;
        }
    }*/
    for (int i = 0; i < xDimension; ++i) {
        delete[] mines[i];
    }
    delete[] mines;
    //ldelete btnLayout;
    //delete timer;
    //delete elapsedTime;
    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < yDimension; ++j)
        {
            newBtns[i][j]->setText("R");
        }
    }
}

int MainWindow::btnsLeft() {
    int temp = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (btns[i][j]->isEnabled()) {
                ++temp;
                if (temp > 10) {
                    return temp;
                }
            }
        }
    }

    return temp;
}
