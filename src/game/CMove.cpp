/**
 * @file CState.cpp
 * @brief Implementation of \a CState.hpp
 * @author lhm
 */

#include "CMove.hpp"

/*************************************************************************************************/
CMove::CMove(int idx) noexcept
  : _data{ idx >= 9 ? static_cast<uint16_t>(0) : static_cast<uint16_t>(1 << (idx & 0xFF)) }
{}

/*************************************************************************************************/
bool
operator<(const CMove& lhs, const CMove& rhs) noexcept
{
    return lhs._data < rhs._data;
}

/*************************************************************************************************/
bool
operator==(const CMove& lhs, const CMove& rhs) noexcept
{
    return lhs._data == rhs._data;
}

/*************************************************************************************************/
bool
operator!=(const CMove& lhs, const CMove& rhs) noexcept
{
    return !(operator==(lhs, rhs));
}

/*************************************************************************************************/
bool
CMove::apply(CState& state) const noexcept
{
    return state.play(index());
}

/*************************************************************************************************/
int
CMove::index(void) const noexcept
{
    auto x{ 0 };
    for (auto i{ 0 }; i < 9; ++i) {
        if ((_data >> i) & 1) {
            x = i;
            break;
        }
    }

    return x;
}

/*************************************************************************************************/
bool
CMove::set_index(int x) noexcept
{
    if (x >= 9)
        return false;

    return (_data = (1 << x));
}

/*************************************************************************************************/
bool
CMove::set_index(int x, int y) noexcept
{
    return set_index(3 * x + y);
}

/*************************************************************************************************/
std::ostream&
CMove::print(std::ostream& os) const noexcept
{
    auto x{ index() };
    return os << "move[" << x << "] (" << (x / 3) << ", " << (x % 3) << ")\n";
}
