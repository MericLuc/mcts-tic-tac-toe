#ifndef GRID_H
#define GRID_H

// Qt headers
#include <QWidget>

// Standard headers
#include <array>

// Project's headers
#include <game/CMove.hpp>
#include <game/CState.hpp>

class Cell;

typedef enum
{
    HUMAN,
    BOT
} PlayerType;

/*!
 * \brief The Grid class exposes a N x N Grid (LatinSquare)
 */
class Grid : public QWidget
{
    Q_OBJECT

public:
    explicit Grid(QWidget* parent = nullptr) noexcept;
    virtual ~Grid() noexcept = default;

    /*!
     * \brief getState Get the state of the grid
     */
    CState getState() const noexcept;

    PlayerType curPlayer() const noexcept { return _players.at(_state.player()); }

    /*!
     * \brief advance
     */
    [[maybe_unused]] bool advance(const CMove& move) noexcept;

signals:
    /*!
     * \brief started emitted when a game starts
     */
    void started();

    /*!
     * \brief stopped emitted when a game is stopped before the end
     */
    void stopped();

    /*!
     * \brief finished emitted when a game is finished
     * \param winner the winner of the game (or Player::NONE in case of a draw)
     */
    void finished(const Player& winner);

    /*!
     * \brief newTurn emitted when the player whose turn is changes
     * \param curPlayer the current player
     */
    void newTurn(const Player& curPlayer);

public slots:

    /*!
     * \brief stop the game (clear the grid)
     */
    void stop(void) noexcept;

    /*!
     * \brief start the game with specified players
     */
    void start(const PlayerType& cross, const PlayerType& circle) noexcept;

protected:
    /*!
     * \brief setState fills the grid using custom state
     * \param state the state of the game to use
     */
    void setState(const CState& state) noexcept;

private:
    static constexpr size_t _size{ 3 };

    std::array<Cell*, _size * _size> _cells;   /*!< Cells of the Grid */
    CState                           _state{}; /*!< The state of the game */
    bool                             _running{ false };
    std::map<Player, PlayerType>     _players{};
};

#endif // GRID_H
