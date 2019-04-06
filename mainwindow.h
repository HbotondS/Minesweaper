#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include "qrightclickbutton.h"
#include "QDebug"
#include "QGridLayout"
#include "QMessageBox"
#include "QLabel"
#include "QElapsedTimer"
#include "QTimer"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "qrightclickbutton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(int, int);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int xDimension = 10;
    int yDimension = 10;

    QLabel* bombsLabel;
    QLabel* timeLabel;

    QTimer* timer;
    QElapsedTimer* elapsedTime;

    QGridLayout* btnLayout;
    QRightClickButton*** newBtns;

    /*
     * -1 <= mines
     *  0 <= no mines in neighbour
     *  1-6 <= no. of mines located in neighbour
     */
    int **mines;

    void init();
    void newGame();
    void generateMines();
    void countMines(int, int);
    void incNum(int, int);
    void showMines();
    void clearField(int, int);
    int btnsLeft();
    void printMines();
    void fillmines();
    void restart();

private slots:
    void btn_action(int, int);
    void update_time();

    void onRightClicked(int, int);

    void winmsg();

signals:
    void win();
};

#endif // MAINWINDOW_H
