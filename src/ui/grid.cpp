/**
 * @file grid.cpp
 * @brief Implementation of \a grid.h
 * @author lhm
 */

// Project's headers
#include "grid.h"
#include "cell.h"

// External headers

/*****************************************************************************/
Grid::Grid(QWidget* parent) noexcept
  : QWidget(parent)
{
    setFixedSize(500, 500);

    for (size_t i{ 0 }, k{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j, ++k)
            _cells[k] = new Cell(i, j, this);

    for (auto& c : _cells) {
        c->set(Player::NONE);
        c->show();
        connect(c, &Cell::checked, this, [this](int idx) {
            if (!_running)
                return;

            auto curPlayer{ _state.player() };
            if (PlayerType::HUMAN != _players[curPlayer])
                return;

            advance(CMove(idx));
        });
        connect(c, &Cell::hovered, this, [c, this](bool hovered) {
            if (!_running)
                return;

            if (Player::NONE != c->get())
                return;

            if (hovered)
                c->setColorEffect((Player::CROSS == _state.player()) ? Qt::red : Qt::blue);
            else
                c->removeColorEffect();
        });
    }
}

/*****************************************************************************/
CState
Grid::getState() const noexcept
{
    return _state;
}

/*****************************************************************************/
void
Grid::setState(const CState& data) noexcept
{
    _state = data;

    for (size_t i{ 0 }, k{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j, ++k)
            _cells[k]->set(_state.get(k));
}

/*****************************************************************************/
bool
Grid::advance(const CMove& move) noexcept
{
    bool ret{ true };

    if (CMove() != move) {
        ret = move.apply(_state);
        int idx{ move.index() };

        _cells[idx]->set(_state.get(idx));
        repaint();
        if (auto winner{ _state.winner() }; Player::NONE != winner) {
            _running = false;
            emit finished(winner);
            return ret;
        }

        if (9 <= _state.turn()) {
            _running = false;
            emit finished(Player::NONE);
            return ret;
        }

        emit newTurn(_state.player());
    }

    return ret;
}

/*****************************************************************************/
void
Grid::stop(void) noexcept
{
    _running = false;
    _state = CState();
    _players.clear();
    setState(_state);

    emit stopped();
}

/*****************************************************************************/
void
Grid::start(const PlayerType& cross, const PlayerType& circle) noexcept
{
    if (_running) // shoudl not happen
        stop();

    _players = { { Player::CROSS, cross }, { Player::CIRCLE, circle } };
    _running = true;

    emit started();

    advance(CMove());
}
