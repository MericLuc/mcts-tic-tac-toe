/**
 * @file CState.hpp
 * @brief Implementation of the concrete State of the game
 * @note Derived class of mcts::State
 */

#include <3rd/mcts/mcts.hpp>

#include <map>
#include <vector>

#ifndef CSTATE_HPP
#define CSTATE_HPP

typedef enum
{
    NONE = 0,
    CROSS = 1 << 0,
    CIRCLE = 1 << 1
} Player;

class CState : public mcts::State
{
public:
    /*!
     * Default constructor creates an empty board where its Player::CROSS to play
     */
    CState() noexcept = default;
    virtual ~CState() noexcept override = default;

    friend bool operator==(const CState& lhs, const CState& rhs) noexcept;
    friend bool operator!=(const CState& lhs, const CState& rhs) noexcept;

    /*!
     * \brief play The current player places a mark on the cell c(x,y)
     * \param x the row
     * \param y the col
     * \return true in case of success, false otherwise
     */
    [[maybe_unused]] bool play(int x, int y) noexcept;

    /*!
     * \brief play The current player places a mark on the cell c(x)
     * \param x the 1D index
     * \return true in case of success, false otherwise
     */
    [[maybe_unused]] bool play(int x) noexcept;

    /*!
     * \brief player Get the current \a Player
     * \return the player it is the turn to play to.
     */
    Player player() const noexcept;

    /*!
     * \brief get Retrieves the player that owns the cell at position (x,y)
     * \param x the row
     * \param y the column
     * \return The player who owns the cell (or Player::NONE if it is free)
     */
    Player get(int x, int y) const noexcept;

    /*!
     * \brief get Retrieves the player that owns the cell at position (x)
     * \param x the 1D index
     * \return The player who owns the cell (or Player::NONE if it is free)
     */
    Player get(int x) const noexcept;

    /*!
     * \brief turn retrieves the current turn number
     * \return the turn number
     */
    uint8_t turn(void) const noexcept;

    /*!
     * \brief winner Returns the player that won the game (if the state is a termination state,
     * Player::NONE otherwise)
     * \return The player who won the game
     */
    Player winner(void) const noexcept;

    std::ostream& print(std::ostream& os) const noexcept override;

private:
    /**
     * Encodes the state of the game
     *
     * b[31-30]     : the player it is the turn to play
     * b[18-25]     : the number of turns played (should never be > 9)
     * b[1 - 0]     : the state of the first cell (i.e. c[0][0])
     * b[2x+1 - 2x] : the state of the xth cell (x in [0,8]) (i.e. c[x/3][x%3])
     */
    uint32_t _state{ CROSS << 30 };

    static constexpr uint32_t CELL_MASK{ 0x3 };
    static constexpr uint32_t PLAYER_MASK{ 0x3FFFFFFF };
    static constexpr uint32_t TURN_MASK{ 0xFC03FFFF };

    static constexpr uint32_t PLAYER_OFFSET{ 30 };
    static constexpr uint32_t TURN_OFFSET{ 18 };
};

#endif // CSTATE_HPP
