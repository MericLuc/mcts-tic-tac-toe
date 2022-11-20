/**
 * @file CBackPropagationStrategy.hpp
 * @brief Implementation of the concrete BackPropagation Strategy of the game
 * @note Derived class of mcts::BackPropagationStrategy
 */

#include "CMove.hpp"

#ifndef CBACKPROPAGATIONSTRATEGY_HPP
#define CBACKPROPAGATIONSTRATEGY_HPP

class CBackPropagationStrategy : public mcts::BackPropagationStrategy<CState>
{
public:
    CBackPropagationStrategy(const Player& player) noexcept
      : mcts::BackPropagationStrategy<CState>()
      , _player{ player }
    {}

    float adjust(const CState& state, float eval) noexcept override
    {
        return (state.player() == _player) ? -eval : eval;
    }

private:
    Player _player;
};

#endif // CBACKPROPAGATIONSTRATEGY_HPP
