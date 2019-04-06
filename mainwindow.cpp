#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startwindow.h"

MainWindow::MainWindow(int xDimension, int yDimension):
    ui(new Ui::MainWindow),
    xDimension(xDimension),
    yDimension(yDimension)
{
    ui->setupUi(this);
    init();
    connectActions();
}

MainWindow::MainWindow(int xDimension, int yDimension, int numberOfBombs):
    ui(new Ui::MainWindow),
    xDimension(xDimension),
    yDimension(yDimension),
    numberOfBombs(numberOfBombs)
{
    ui->setupUi(this);
    init();
    connectActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->action_New_game, &QAction::triggered, [this]{newGame();});
    connect(ui->action_Restart, &QAction::triggered, [this]{restart();});
    connect(ui->action_Exit, &QAction::triggered, []{QApplication::quit();});
    connect(this, SIGNAL(win()), this, SLOT(winmsg()));
}

void MainWindow::init() {
    timer = new QTimer();
    elapsedTime = new QElapsedTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));

    QVBoxLayout* mainLayout = new QVBoxLayout();
    btnLayout = new QGridLayout();
    btnLayout->setSpacing(1);
    QHBoxLayout* labelLayout = new QHBoxLayout();

    bombsLabel = new QLabel(QString::number(numberOfBombs));
    labelLayout->addWidget(bombsLabel);
    timeLabel = new QLabel("");
    timeLabel->setAlignment(Qt::AlignRight);
    labelLayout->addWidget(timeLabel);

    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(btnLayout);

    newBtns = new QRightClickButton**[xDimension];
    mines = new int*[xDimension];
    for (int i = 0; i < xDimension; ++i) {
        newBtns[i] = new QRightClickButton*[yDimension];
        mines[i] = new int[yDimension];
    }
    fillmines();

    generateMines();
    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < yDimension; ++j)
        {
            newBtns[i][j] = new QRightClickButton();
            newBtns[i][j]->setMinimumSize(40, 40);
            connect(newBtns[i][j], &QRightClickButton::clicked, [this, i, j]{btn_action(i, j);});
            connect(newBtns[i][j], &QRightClickButton::rightClicked, [this, i, j]{onRightClicked(i, j);});
            btnLayout->addWidget(newBtns[i][j], i, j);
        }
    }
    ui->centralWidget->setLayout(mainLayout);

    timer->start();
    elapsedTime->start();
}

/**
 * for debuging
 */
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
    int x, y, currentMines = 0;
    while (currentMines != numberOfBombs)
    {
        x = rand() % xDimension;
        y = rand() % yDimension;

        if (mines[x][y] != -1)
        {
            mines[x][y] = -1;
            ++currentMines;
            countMines(x, y);
        }
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
    if (newBtns[x][y]->text() != "B") {
        auto isMine = mines[x][y];
        switch (isMine)
        {
            case -1:
            {
                newBtns[x][y]->setStyleSheet("color: red;");
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
                newBtns[x][y]->setStyleSheet("border: none;");
                newBtns[x][y]->setText(QString::number(isMine));
                newBtns[x][y]->setDisabled(true);
            }
        }
    }

    if (btnsLeft() == numberOfBombs) {
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
    if (bombsLeft > 0 && newBtns[x][y]->text() == "")
    {
        bombsLabel->setText(QString::number(--bombsLeft));
        newBtns[x][y]->setText("B");
    }
    else
    {
        if (newBtns[x][y]->text() == "B")
        {
            bombsLabel->setText(QString::number(++bombsLeft));
            newBtns[x][y]->setText("");
        }
    }
    if (bombsLeft == 0) {
        if (btnsLeft() == numberOfBombs) {
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
    for (int i = 0; i < xDimension; ++i) {
        delete[] mines[i];
    }
    delete[] mines;
    for (int i = 0; i < xDimension; ++i)
    {
        for (int j = 0; j < yDimension; ++j)
        {
            newBtns[i][j]->setText("");
            newBtns[i][j]->setEnabled(true);
            newBtns[i][j]->setStyleSheet("border-bottom: 2px solid #7B7B7B; \
                                         border-left: 2px solid #ffffff;    \
                                         border-top: 2px solid #ffffff;     \
                                         border-right: 2px solid #7B7B7B;");
        }
    }
    mines = new int*[xDimension];
    for (int i = 0; i < xDimension; ++i) {
        mines[i] = new int[yDimension];
    }
    fillmines();

    generateMines();
    timer->start();
    elapsedTime->start();
}

int MainWindow::btnsLeft() {
    int temp = 0;
    for (int i = 0; i < xDimension; ++i) {
        for (int j = 0; j < yDimension; ++j) {
            if (newBtns[i][j]->isEnabled()) {
                ++temp;
                if (temp > numberOfBombs) {
                    return temp;
                }
            }
        }
    }

    return temp;
}
