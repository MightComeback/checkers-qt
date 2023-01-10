#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "game.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    connect(ui->toolBar, SIGNAL(actionTriggered(QAction *)), this, SLOT(toolBarAction(QAction *)));
    ui->boardView->viewport()->installEventFilter(this);
    setWindowTitle("Шашки КНТ-211");

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::gray);
    ui->boardView->setScene(scene);
    ui->boardView->rotate(270);
    ui->boardView->show();

    scene->installEventFilter(this);

    QSettings settings("ZUNP KNT-211", "CHECKERS");
    settings.setValue("blackPawn", QColor(Qt::black));
    settings.setValue("whitePawn", QColor(Qt::white));

    game = new Game(scene);
}

GameWindow::~GameWindow()
{
    delete ui;
    delete game;
    delete scene;
}

void GameWindow::toolBarAction(QAction *action)
{
    QString name = action->objectName();
    if (name == "actionNewGame")
        newGame();
    else if (name == "actionSettings")
    {
        SettingsWidget *window = new SettingsWidget();
        window->setWindowTitle("Налаштування");
        window->show();
    }
    else if (name == "actionRulesMenu")
    {
        RulesWidget *window = new RulesWidget();
        window->setWindowTitle("Правила");
        window->show();
    }
    else if (name == "actionInfoMenu")
    {
        InfoWidget *window = new InfoWidget();
        window->setWindowTitle("Інформація");
        window->show();
    }
}

void GameWindow::newGame()
{
    delete game;
    delete scene;
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::gray);
    ui->boardView->setScene(scene);
    game = new Game(scene);
}

bool GameWindow::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::Resize)
    {
        ui->boardView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        return true;
    }
    else if (e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
        int x = ui->boardView->mapToScene(mouseEvent->pos()).x();
        int y = ui->boardView->mapToScene(mouseEvent->pos()).y();
        int square = std::min(scene->width(), scene->height());

        if (x >= 0 && y >= 0 && x <= square && y <= square)
        {
            game->squareClicked(x, y);
        }
        if (game->getGameOverFlag())
        {
            QMessageBox msgBox;
            msgBox.setText("Гра закінчена");
            msgBox.setInformativeText("Хочете зіграти знову?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setButtonText(QMessageBox::Yes, tr("Так"));
            msgBox.setButtonText(QMessageBox::No, tr("Ні"));
            QSpacerItem *horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Maximum);
            QGridLayout *layout = (QGridLayout *)msgBox.layout();
            layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
            int ret = msgBox.exec();
            switch (ret)
            {
            case QMessageBox::Yes:
                newGame();
                break;
            case QMessageBox::No:
                this->close();
                break;
            default:
                break;
            }
        }

        ui->boardView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        return true;
    }
    else
        return QObject::eventFilter(o, e);
}
