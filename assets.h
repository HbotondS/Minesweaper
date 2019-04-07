#ifndef ASSETS_H
#define ASSETS_H

#include "QIcon"


class Assets
{
private:
    QIcon* img1;
    QIcon* img2;
    QIcon* img3;
    QIcon* img4;
    QIcon* img5;
    QIcon* img6;
    QIcon* img7;
    QIcon* img8;
    QIcon* imgFlag;
    QIcon* imgMine;
    QIcon* imgMineClicked;
public:
    Assets();
    ~Assets();
    QIcon *getImg1() const;
    QIcon *getImg2() const;
    QIcon *getImg3() const;
    QIcon *getImg4() const;
    QIcon *getImg5() const;
    QIcon *getImg6() const;
    QIcon *getImg7() const;
    QIcon *getImg8() const;
    QIcon *getImgFlag() const;
    QIcon *getImgMine() const;
    QIcon *getImgMineClicked() const;
};

#endif // ASSETS_H
