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
    connect(this, SIGNAL(win()), this, SLOT(winmsg()));
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

    btnLayout = new QGridLayout(ui->centralWidget);
    bombsLabel = new QLabel("10");
    bombsLabel->setStyleSheet("padding-bottom: 600px;");
    btnLayout->addWidget(bombsLabel, 0, 0, 0, 4);
    timeLabel = new QLabel("");
    timeLabel->setStyleSheet("padding-bottom: 600px;");
    timeLabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);
    btnLayout->addWidget(timeLabel, 0, 9, 0, -5);


    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            btns[i][j] = new QRightClickButton(this);
            btns[i][j]->setMinimumSize(50, 50);
            btnLayout->addWidget(btns[i][j], i+1, j);
            connect(btns[i][j], &QRightClickButton::clicked, [this, i, j]{btn_action(i, j);});
            connect(btns[i][j], &QRightClickButton::rightClicked, [this, i, j]{onRightClicked(i, j);});
        }
    }
    this->setFixedSize(this->minimumSize());
    ui->centralWidget->setLayout(btnLayout);

    generateMines();

    timer->start();
    elapsedTime->start();

    //printMines();
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
    if (btns[x][y]->text() != "B") {
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
