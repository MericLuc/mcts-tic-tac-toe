#ifndef CELL_H
#define CELL_H

// Qt headers
#include <QSvgRenderer>
#include <QWidget>

// Project's headers
#include <game/CState.hpp>

// Standard headers
#include <map>

class QLabel;

class Cell : public QWidget
{
    Q_OBJECT
    friend class Grid;

public:
    Cell(const size_t x, const size_t y, QWidget* parent = nullptr) noexcept;
    virtual ~Cell() noexcept = default;

    void   set(const Player& player) noexcept;
    Player get(void) const noexcept { return _player; }

public slots:
    void setColorEffect(const QColor& c = QColor(255, 20, 20)) noexcept;
    void removeColorEffect() noexcept;

signals:
    void checked(int);
    void hovered(bool);

protected:
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    static constexpr size_t _dim{ 3 };

    const size_t _x, _y;
    Player       _player{ Player::NONE };

    // UI related members
    QLabel* _bg{ nullptr };
    QLabel* _lb{ nullptr };

    static std::map<Player, QSvgRenderer*> _renderers;
};

#endif // CELL_H
