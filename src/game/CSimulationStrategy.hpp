/**
 * @file CSimulationStrategy.hpp
 * @brief Implementation of the concrete Simulation strategy of the game
 * @note Derived class of mcts::SimulationStrategy
 */

#include "CMove.hpp"

#include <random>

#ifndef CSIMULATIONSTRATEGY_HPP
#define CSIMULATIONSTRATEGY_HPP

class CSimulationStrategy : public mcts::SimulationStrategy<CState, CMove>
{
public:
    CSimulationStrategy() noexcept
      : mcts::SimulationStrategy<CState, CMove>()
    {}
    virtual ~CSimulationStrategy() noexcept override = default;

    CMove simulate(const CState& state) noexcept override
    {
        auto idx{ _dist(_gen) };
        while (Player::NONE != state.get(idx)) {
            idx = _dist(_gen);
        }
        return CMove(idx);
    }

private:
    std::random_device                  _dev;
    std::mt19937                        _gen{ std::mt19937(_dev()) };
    std::uniform_int_distribution<uint> _dist{ std::uniform_int_distribution<uint>(0, 8) };
};

#endif // CSIMULATIONSTRATEGY_HPP
