#include "infomenu.h"
#include "ui_infomenu.h"

InfoMenu::InfoMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoMenu)
{
    ui->setupUi(this);
}

InfoMenu::~InfoMenu()
{
    delete ui;
}
