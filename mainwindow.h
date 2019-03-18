#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include "QPushButton"
#include "QDebug"
#include "QGridLayout"
#include "QMessageBox"

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

    QPushButton* btns[10][10];
    /*
     * -1 <= mines
     *  0 <= no mines in neighbour
     *  1-6 <= no. of mines located in neighbour
     */
    int mines[10][10] = {{0}};

    void generateMines();
    void countMines();
    void showMines();
    void clearField(int, int);

private slots:
    void btn_action(int, int);
};

#endif // MAINWINDOW_H
