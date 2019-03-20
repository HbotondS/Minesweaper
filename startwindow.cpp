#include "startwindow.h"
#include "ui_startwindow.h"
#include "mainwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_exitBtn_clicked()
{
    QApplication::quit();
}

void StartWindow::on_startBtn_clicked()
{
    this->close();
    MainWindow *w = new MainWindow(ui->xSpin->value(), ui->ySpin->value());
    w->show();
}
