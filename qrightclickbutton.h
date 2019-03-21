#ifndef QRIGHTCLICKBUTTON_H
#define QRIGHTCLICKBUTTON_H

#include "QPushButton"
#include "QMouseEvent"

class QRightClickButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QRightClickButton(QWidget *parent = nullptr);

signals:

public slots:
    void mousePressEvent(QMouseEvent* e);

signals:
    void rightClicked();
};

#endif // QRIGHTCLICKBUTTON_H
