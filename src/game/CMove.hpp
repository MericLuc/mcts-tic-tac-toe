/**
 * @file CState.hpp
 * @brief Implementation of the concrete State of the game
 * @note Derived class of mcts::State
 */

#include "CState.hpp"

#ifndef CMOVE_HPP
#define CMOVE_HPP

class CMove : public mcts::Move<CState>
{
public:
    /*!
     * Default constructor creates an empty move (does nothing)
     */
    CMove() noexcept = default;

    /*!
     * \brief CMove Create a move at the required index
     * \param idx the required index
     * \note It will create an empty move in case the provided index is invalid (>8)
     */
    CMove(int idx) noexcept;

    virtual ~CMove() noexcept override = default;

    friend bool operator<(const CMove& lhs, const CMove& rhs) noexcept;
    friend bool operator==(const CMove& lhs, const CMove& rhs) noexcept;
    friend bool operator!=(const CMove& lhs, const CMove& rhs) noexcept;

    [[maybe_unused]] virtual bool apply(CState& state) const noexcept override;

    /*!
     * \brief index retrieves the index of the move
     * \return the index of the move
     */
    int index(void) const noexcept;

    /*!
     * \brief set_index Change the index of the move played.
     * \param x the new 1D index
     * \return true in case of success, false otherwise
     */
    [[maybe_unused]] bool set_index(int x) noexcept;

    /*!
     * \brief set_index Change the index of the move played.
     * \param x the new row index
     * \param y the new col index
     * \return true in case of success, false otherwise
     */
    [[maybe_unused]] bool set_index(int x, int y) noexcept;

    std::ostream& print(std::ostream& os) const noexcept override;

private:
    /**
     * Encodes the move
     *
     * b[x] : the player played the cell c[x] (x in [0,8]) (i.e. c[x/3][x%3])
     */
    uint16_t _data{ 0 };
};

#endif // CMOVE_HPP
