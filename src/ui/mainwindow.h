#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt headers
#include <QMainWindow>

// Standard headers
#include <map>

// Project headers
#include <game/CState.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QSvgRenderer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
private:
    Ui::MainWindow*                 ui;
    std::map<Player, QSvgRenderer*> _renderers;

    bool _running; /*< A game is being played */
};
#endif // MAINWINDOW_H
