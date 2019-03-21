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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLabel* bombsLabel;
    QLabel* timeLabel;

    QTimer* timer;
    QElapsedTimer* elapsedTime;

    QGridLayout *btnLayout;
    QRightClickButton* btns[10][10];

    /*
     * -1 <= mines
     *  0 <= no mines in neighbour
     *  1-6 <= no. of mines located in neighbour
     */
    int mines[10][10];

    void init();
    void newGame();
    void generateMines();
    void countMines(int, int);
    void incNum(int, int);
    void showMines();
    void clearField(int, int);

private slots:
    void btn_action(int, int);
    void update_time();

    void onRightClicked(int, int);
};

#endif // MAINWINDOW_H
