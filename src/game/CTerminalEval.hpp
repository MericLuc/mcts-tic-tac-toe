/**
 * @file CTerminalEval.hpp
 * @brief Implementation of the concrete TerminalEval of the game
 * @note Derived class of mcts::TerminalEval
 */

#include "CMove.hpp"

#ifndef CTERMINALEVAL_HPP
#define CTERMINALEVAL_HPP

class CTerminalEval : public mcts::TerminalEval<CState>
{
public:
    CTerminalEval(const Player& player) noexcept
      : mcts::TerminalEval<CState>()
      , _player{ player }
    {}

    float eval(const CState& state) noexcept override
    {
        auto win_player{ state.winner() };
        return (win_player == _player) ? 1 : ((win_player == Player::NONE) ? 0.6 : -1);
    }

private:
    Player _player;
};

#endif // CTERMINALEVAL_HPP
