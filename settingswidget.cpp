#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    connect(ui->brownRadioButton, SIGNAL(clicked()), SLOT(brownOption()));
    connect(ui->blackRadiokButton, SIGNAL(clicked()), SLOT(blackOption()));
    connect(ui->blueRadioButton, SIGNAL(clicked()), SLOT(blueOption()));
    connect(ui->backButton, SIGNAL(clicked()), SLOT(close()));

    QSettings settings("ZUNP KNT-211", "CHECKERS");
    if (settings.value("blackPawn", QColor(Qt::black)).value<QColor>() == Qt::black)
        ui->blackRadiokButton->setChecked(true);
    else if (settings.value("blackPawn", QColor("#8B4513")).value<QColor>() == "#8B4513")
        ui->brownRadioButton->setChecked(true);
    else
        ui->blueRadioButton->setChecked(true);
}
void SettingsWidget::brownOption()
{
    QSettings settings("ZUNP KNT-211", "CHECKERS");
    settings.setValue("blackPawn", QColor("#8B4513"));
    settings.setValue("whitePawn", QColor("#ffecb3"));
}
void SettingsWidget::blackOption()
{
    QSettings settings("ZUNP KNT-211", "CHECKERS");
    settings.setValue("blackPawn", QColor(Qt::black));
    settings.setValue("whitePawn", QColor(Qt::white));
}
void SettingsWidget::blueOption()
{
    QSettings settings("ZUNP KNT-211", "CHECKERS");
    settings.setValue("blackPawn", QColor("#34e5eb"));
    settings.setValue("whitePawn", QColor(Qt::white));
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
