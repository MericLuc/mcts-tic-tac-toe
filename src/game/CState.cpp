/**
 * @file CState.cpp
 * @brief Implementation of \a CState.hpp
 * @author lhm
 */

#include "CState.hpp"

/*************************************************************************************************/
Player
CState::player() const noexcept
{
    return static_cast<Player>((_state >> PLAYER_OFFSET) & CELL_MASK);
}

/*************************************************************************************************/
bool
operator==(const CState& lhs, const CState& rhs) noexcept
{
    return lhs._state == rhs._state;
}

/*************************************************************************************************/
bool
operator!=(const CState& lhs, const CState& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

/*************************************************************************************************/
bool
CState::play(int x, int y) noexcept
{
    return play(3 * x + y);
}

/*************************************************************************************************/
bool
CState::play(int x) noexcept
{
    auto curPlayer{ player() },
      nextPlayer{ (Player::CIRCLE == curPlayer) ? Player::CROSS : Player::CIRCLE };
    auto tr{ turn() + 1 };

    if (x >= 9 || Player::NONE != get(x))
        return false;

    _state &= (PLAYER_MASK & TURN_MASK);
    _state |= ((nextPlayer << PLAYER_OFFSET) | (curPlayer << 2 * x) | (tr << TURN_OFFSET));
    return true;
}

/*************************************************************************************************/
Player
CState::get(int x, int y) const noexcept
{
    return get(3 * x + y);
}

/*************************************************************************************************/
Player
CState::get(int x) const noexcept
{
    return (x >= 9) ? Player::NONE : static_cast<Player>((_state >> 2 * x) & CELL_MASK);
}

/*************************************************************************************************/
uint8_t
CState::turn(void) const noexcept
{
    return (_state >> TURN_OFFSET) & 0xFF;
}

/*************************************************************************************************/
Player
CState::winner(void) const noexcept
{
    static const std::map<Player, std::vector<uint32_t>> win_masks{ { Player::CIRCLE,
                                                                      {
                                                                        0x2A,    // row 0
                                                                        0xA80,   // row 1
                                                                        0x2A000, // row 2
                                                                        0x2082,  // col 0
                                                                        0x8208,  // col 1
                                                                        0x20820, // col 2
                                                                        0x20202, // diag G
                                                                        0x2220   // diag D
                                                                      } },
                                                                    { Player::CROSS,
                                                                      {
                                                                        0x15,    // row 0
                                                                        0x540,   // row 1
                                                                        0x15000, // row 2
                                                                        0x1041,  // col 0
                                                                        0x4104,  // col 1
                                                                        0x10410, // col 2
                                                                        0x10101, // diag G
                                                                        0x1110,  // diag D
                                                                      } }

    };

    for (const auto& [pl, moves] : win_masks) {
        for (const auto& mv : moves) {
            if ((_state & mv) == mv)
                return pl;
        }
    }
    return Player::NONE;
}

/*************************************************************************************************/
std::ostream&
CState::print(std::ostream& os) const noexcept
{
    static const auto cvtFn = [](int val) {
        if (val == Player::CROSS)
            return 'x';
        if (val == Player::CIRCLE)
            return 'o';
        return '.';
    };

    os << "----------\n";
    for (int i{ 0 }; i < 3; ++i) {
        for (int j{ 0 }; j < 3; ++j) {
            os << cvtFn(get(i, j));
        }
        os << '\n';
    }
    return os << "Turn : " << (int)turn() << ", Player : " << cvtFn(player()) << "\n----------\n";
}
