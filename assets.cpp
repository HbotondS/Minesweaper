#include "assets.h"

Assets::Assets()
{
    img1 = new QIcon(":/assets/1.png");
    img2 = new QIcon(":/assets/2.png");
    img3 = new QIcon(":/assets/3.png");
    img4 = new QIcon(":/assets/4.png");
    img5 = new QIcon(":/assets/5.png");
    img6 = new QIcon(":/assets/6.png");
    img7 = new QIcon(":/assets/7.png");
    img8 = new QIcon(":/assets/8.png");
    imgFlag = new QIcon(":/assets/flag.png");
    imgMine = new QIcon(":/assets/mine.png");
    imgMineClicked = new QIcon(":/assets/mine_clicked.png");
}

Assets::~Assets()
{
    delete img1;
    delete img2;
    delete img3;
    delete img4;
    delete img5;
    delete img6;
    delete img7;
    delete img8;
    delete imgFlag;
    delete imgMine;
    delete imgMineClicked;
}

QIcon *Assets::getImg1() const
{
    return img1;
}

QIcon *Assets::getImg2() const
{
    return img2;
}

QIcon *Assets::getImg3() const
{
    return img3;
}

QIcon *Assets::getImg4() const
{
    return img4;
}

QIcon *Assets::getImg5() const
{
    return img5;
}

QIcon *Assets::getImg6() const
{
    return img6;
}

QIcon *Assets::getImg7() const
{
    return img7;
}

QIcon *Assets::getImg8() const
{
    return img8;
}

QIcon *Assets::getImgFlag() const
{
    return imgFlag;
}

QIcon *Assets::getImgMine() const
{
    return imgMine;
}

QIcon *Assets::getImgMineClicked() const
{
    return imgMineClicked;
}
