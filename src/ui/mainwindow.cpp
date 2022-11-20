/**
 * @file mainwindow.cpp
 * @brief Implementation of \a mainwindow.h
 * @author lhm
 */

// Standard headers

// Qt headers
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QSvgRenderer>

// Project's headers
#include "./ui_mainwindow.h"
#include "mainwindow.h"

#include <game/CBackPropagationStrategy.hpp>
#include <game/CExpansionStrategy.hpp>
#include <game/CMove.hpp>
#include <game/CSimulationStrategy.hpp>
#include <game/CTerminalCriteria.hpp>
#include <game/CTerminalEval.hpp>

// External headers

/*****************************************************************************/
void
setupWidget(QWidget* w, const QString& name = "setupWidget")
{
    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setOffset(1, 1);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(2);

    w->setObjectName(name);
    w->setGraphicsEffect(shadow_effect);
}

/*****************************************************************************/
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , _renderers{ { Player::CROSS, new QSvgRenderer(QString("imgs/cross.svg"), this) },
                { Player::CIRCLE, new QSvgRenderer(QString("imgs/circle.svg"), this) },
                { Player::NONE, new QSvgRenderer(QString("imgs/none.svg"), this) } }
{
    ui->setupUi(this);

    setupWidget(ui->pb_start);
    setupWidget(ui->cb_p1);
    setupWidget(ui->cb_p2);

    setStyleSheet("QLineEdit#setupWidget{background-color:#ffffff;color:#5F5F5F;}"
                  "QLineEdit#setupWidget:hover{background-color:#daffb3;}"
                  "QPushButton#setupWidget{background-color:#ffffff;color:#5F5F5F;}"
                  "QPushButton#setupWidget:hover{background-color:#daffb3;}"
                  "QPushButton#setupWidget:pressed{background-color:#5cb300;}"
                  "QPushButton#setupWidget:!enabled{background-color:rgb(200, 200, 200);}"
                  "QComboBox#setupWidget{border-radius:3px;background-color:#ffffff;color:#"
                  "5F5F5F;}"
                  "QComboBox#setupWidget:hover{background-color:#daffb3;}"
                  "QComboBox#setupWidget:pressed{background-color:#5cb300;}"
                  "QComboBox#setupWidget:!enabled{background-color:rgb(200, 200, 200);}"
                  "QCheckBox#setupWidget{background-color:transparent;color:#5F5F5F;}"
                  "QCheckBox#setupWidget:hover{background-color:#daffb3;}"
                  "QCheckBox#setupWidget:pressed{background-color:#5cb300;}"
                  "QCheckBox#setupWidget:!enabled{background-color:rgb(200, 200, 200);}");

    for (auto& btn : { ui->cb_p1, ui->cb_p2 }) {
        btn->setStyleSheet(
          QString("border-top-right-radius:%1px;border-bottom-right-radius:%1px;").arg(3));
    }
    QImage img1{ QImage(ui->lb_p1->height(), ui->lb_p1->height(), QImage::Format_ARGB32) },
      img2{ QImage(ui->lb_p1->height(), ui->lb_p1->height(), QImage::Format_ARGB32) };

    img1.fill(Qt::transparent);
    img2.fill(Qt::transparent);

    QPainter pt1{ QPainter(&img1) }, pt2{ QPainter(&img2) };

    _renderers[Player::CROSS]->render(&pt1);
    _renderers[Player::CIRCLE]->render(&pt2);

    ui->lb_p1->setPixmap(QPixmap::fromImage(img1));
    ui->lb_p2->setPixmap(QPixmap::fromImage(img2));
    ui->lb_curPlayer->clear();

    // Connect buttons to actions
    connect(ui->pb_start, &QPushButton::clicked, this, [this]() {
        static bool started{ false };

        ui->lb_infos->clear();

        if (!started) {
            ui->cb_p1->setEnabled(false);
            ui->cb_p2->setEnabled(false);
            ui->pb_start->setText("Stop");
            PlayerType p1{ ui->cb_p1->currentIndex() == 0 ? PlayerType::HUMAN : PlayerType::BOT };
            PlayerType p2{ ui->cb_p2->currentIndex() == 0 ? PlayerType::HUMAN : PlayerType::BOT };

            ui->w_grid->start(p1, p2);
        } else {
            ui->cb_p1->setEnabled(true);
            ui->cb_p2->setEnabled(true);
            ui->pb_start->setText("Start");
            ui->w_grid->stop();
        }
        started = !started;
    });

    // Connect to board (grid)
    connect(ui->w_grid, &Grid::newTurn, this, [this](Player curPlayer) {
        QImage img{ QImage(ui->lb_curPlayer->size(), QImage::Format_ARGB32) };
        img.fill(Qt::transparent);
        QPainter pt{ QPainter(&img) };
        _renderers[curPlayer]->render(&pt);

        ui->lb_curPlayer->setPixmap(QPixmap::fromImage(img));
        repaint();

        if (PlayerType::BOT == ui->w_grid->curPlayer()) {
            auto nxtMove{ mcts::MCTS<CState, CMove>(
                            ui->w_grid->getState(),
                            std::make_shared<CSimulationStrategy>(),
                            std::make_shared<CExpansionStrategy>(),
                            std::make_shared<CTerminalCriteria>(),
                            std::make_shared<CTerminalEval>(curPlayer),
                            std::make_shared<CBackPropagationStrategy>(curPlayer))
                            .compute() };

            ui->w_grid->advance(nxtMove);
        }
    });
    connect(ui->w_grid, &Grid::started, this, [this]() {
        ui->lb_infos->clear();
        ui->lb_infos->setStyleSheet(QString("color: black;"));

        QImage img{ QImage(ui->lb_curPlayer->size(), QImage::Format_ARGB32) };
        img.fill(Qt::transparent);
        QPainter pt{ QPainter(&img) };
        _renderers[Player::CROSS]->render(&pt);

        ui->lb_curPlayer->setPixmap(QPixmap::fromImage(img));
        repaint();

        if (PlayerType::BOT == ui->w_grid->curPlayer()) {
            auto cp{ ui->w_grid->getState().player() };
            auto nxtMove{ mcts::MCTS<CState, CMove>(ui->w_grid->getState(),
                                                    std::make_shared<CSimulationStrategy>(),
                                                    std::make_shared<CExpansionStrategy>(),
                                                    std::make_shared<CTerminalCriteria>(),
                                                    std::make_shared<CTerminalEval>(cp),
                                                    std::make_shared<CBackPropagationStrategy>(cp))
                            .compute() };

            ui->w_grid->advance(nxtMove);
        }
    });
    connect(ui->w_grid, &Grid::finished, this, [this](Player winner) {
        ui->lb_infos->setText(QString("%1").arg((CROSS == winner)    ? "Cross wins !"
                                                : (CIRCLE == winner) ? "Circle wins !"
                                                                     : "It's a draw !"));
        ui->lb_infos->setStyleSheet(QString("color: %1;")
                                      .arg((CROSS == winner)    ? "rgb(255,0,0)"
                                           : (CIRCLE == winner) ? "rgb(0,0,255)"
                                                                : "rgb(120,120,120)"));

        QImage img{ QImage(ui->lb_curPlayer->size(), QImage::Format_ARGB32) };
        img.fill(Qt::transparent);
        QPainter pt{ QPainter(&img) };
        _renderers[NONE]->render(&pt);

        ui->lb_curPlayer->setPixmap(QPixmap::fromImage(img));
        repaint();
    });
    connect(ui->w_grid, &Grid::stopped, this, [this]() {
        QImage img{ QImage(ui->lb_curPlayer->size(), QImage::Format_ARGB32) };
        img.fill(Qt::transparent);
        QPainter pt{ QPainter(&img) };
        _renderers[NONE]->render(&pt);

        ui->lb_curPlayer->setPixmap(QPixmap::fromImage(img));
        ui->lb_infos->clear();
        repaint();
    });
}

/*****************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}
