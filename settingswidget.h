#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QObject>
#include <QSettings>

namespace Ui
{
    class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private slots:
    void brownOption();
    void blackOption();
    void blueOption();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
