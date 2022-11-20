/**
 * @file CTerminalCriteria.hpp
 * @brief Implementation of the concrete TerminalCriteria of the game
 * @note Derived class of mcts::TerminalCriteria
 */

#include "CMove.hpp"

#ifndef CTERMINALCRITERIA_HPP
#define CTERMINALCRITERIA_HPP

class CTerminalCriteria : public mcts::TerminalCriteria<CState>
{
public:
    bool finished(const CState& state) noexcept override
    {
        return (Player::NONE != state.winner()) || (9 <= state.turn());
    }
};

#endif // CTERMINALCRITERIA_HPP
