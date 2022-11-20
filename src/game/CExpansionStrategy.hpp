/**
 * @file CExpansionStrategy.hpp
 * @brief Implementation of the concrete Expansion strategy of the game
 * @note Derived class of mcts::ExpansionStrategy
 */

#include "CMove.hpp"

#ifndef CEXPANSIONSTRATEGY_HPP
#define CEXPANSIONSTRATEGY_HPP

class CExpansionStrategy : public mcts::ExpansionStrategy<CState, CMove>
{
public:
    CExpansionStrategy() noexcept
      : mcts::ExpansionStrategy<CState, CMove>()
    {}
    virtual ~CExpansionStrategy() noexcept override = default;

    CMove expand(const CState& state) noexcept override
    {
        auto ret{ _idx }, nxtIdx{ MAX_IDX + 1 };
        for (auto i{ _idx + 1 }; i <= MAX_IDX; ++i) {
            if (Player::NONE == state.get(i)) {
                nxtIdx = i;
                break;
            }
        }

        _idx = nxtIdx;

        return ret;
    }

    std::set<CMove> expandAll(const CState& state) noexcept override
    {
        std::set<CMove> ret{};

        for (auto i{ 0 }; i <= MAX_IDX; ++i) {
            if (Player::NONE == state.get(i)) {
                ret.insert(CMove(i));
            }
        }

        return ret;
    }

    bool is_expandable(const CState& state) noexcept override
    {
        if (_reqState != state) {
            _reqState = state;
            _idx = MAX_IDX + 1;
            for (auto i{ 0 }; i <= MAX_IDX; ++i) {
                if (Player::NONE == _reqState.get(i)) {
                    _idx = i;
                    break;
                }
            }
        }

        return _idx <= MAX_IDX;
    }

private:
    CState _reqState{};
    int    _idx{ 0 };

    static constexpr int MAX_IDX{ 8 };
};

#endif // CEXPANSIONSTRATEGY_HPP
