#ifndef RULESWIDGET_H
#define RULESWIDGET_H

#include <QWidget>
#include <QObject>

namespace Ui
{
    class RulesWidget;
}

class RulesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RulesWidget(QWidget *parent = nullptr);
    ~RulesWidget();

private:
    Ui::RulesWidget *ui;
};

#endif // RULESWIDGET_H
