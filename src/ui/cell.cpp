/**
 * @file cell.cpp
 * @brief Implementation of \a cell.h
 * @author lhm
 */

#include "cell.h"

// Project's headers
#include <game/CState.hpp>

// Qt headers
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QtSvg>

// External headers

std::map<Player, QSvgRenderer*> Cell::_renderers{
    { Player::CIRCLE, new QSvgRenderer(QString("imgs/circle.svg")) },
    { Player::CROSS, new QSvgRenderer(QString("imgs/cross.svg")) },
    { Player::NONE, new QSvgRenderer(QString("imgs/none.svg")) }
};

/*****************************************************************************/
Cell::Cell(const size_t x, const size_t y, QWidget* parent) noexcept
  : QWidget(parent)
  , _x{ x }
  , _y{ y }
  , _bg{ new QLabel(this) }
  , _lb{ new QLabel(this) }
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setUpdatesEnabled(false);

    auto pSize{ parent->size() };
    auto cSize{ QSize(pSize.width() / _dim, pSize.height() / _dim) };

    // Position
    move(_y * cSize.width(), _x * cSize.height());
    setFixedSize(cSize);

    // Setup label to contain value
    int lbMargin{ cSize.width() / 5 - 5 };
    int lbSize{ cSize.width() - 2 * lbMargin };
    _lb->setAlignment(Qt::AlignCenter);
    _lb->setObjectName("cellLabel");
    _lb->setFixedSize(lbSize, lbSize);
    _lb->move(lbMargin, lbMargin);
    setStyleSheet(
      QString("#cellLabel{border-radius:%1px;background-color:transparent;}").arg(lbSize / 2));

    // Setup background
    QString style("background-color:#f1f2f3;border:2px solid #000000;");
    if (0 == _x || _dim - 1 == _x) {
        style += QString("border-%1:transparent;").arg(_x == 0 ? "top" : "bottom");
    }
    if (0 == _y || _dim - 1 == _y) {
        style += QString("border-%1:transparent;").arg(_y == 0 ? "left" : "right");
    }

    _bg->setFixedSize(cSize);
    _bg->setStyleSheet(style);

    setUpdatesEnabled(true);
}

/*****************************************************************************/
void
Cell::setColorEffect(const QColor& c) noexcept
{
    auto bg_effect{ new QGraphicsColorizeEffect() };
    bg_effect->setStrength(0.8);
    bg_effect->setColor(c);
    _bg->setGraphicsEffect(bg_effect);
}

/*****************************************************************************/
void
Cell::removeColorEffect() noexcept
{
    _bg->setGraphicsEffect(nullptr);
}

/*****************************************************************************/
void
Cell::enterEvent(QEvent*)
{
    setFocus(Qt::FocusReason::OtherFocusReason);

    if (Player::NONE != _player) {
        auto lb_effect{ new QGraphicsDropShadowEffect() };

        // Setup hovering effect on label
        lb_effect->setOffset(.0);
        lb_effect->setBlurRadius(5.0);
        lb_effect->setColor((Player::CROSS == _player) ? Qt::red : Qt::blue);

        _lb->setGraphicsEffect(lb_effect);
    }

    emit hovered(true);
}

/*****************************************************************************/
void
Cell::leaveEvent(QEvent*)
{
    _lb->setGraphicsEffect(nullptr);
    _bg->setGraphicsEffect(nullptr);

    emit hovered(false);
}

/*****************************************************************************/
void
Cell::mouseReleaseEvent(QMouseEvent* e)
{
    if (Qt::LeftButton == e->button())
        emit checked(3 * _x + _y);
}

/*****************************************************************************/
void
Cell::set(const Player& player) noexcept
{
    if (_player == player)
        return;

    _player = player;

    QImage img = QImage(_lb->size(), QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter = QPainter(&img);

    _renderers[_player]->render(&painter);
    _lb->setPixmap(QPixmap::fromImage(img));

    leaveEvent(nullptr);
    update();
}
