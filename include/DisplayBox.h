#pragma once

#include "DisplayCell.h"
#include "TetrominoShape.h"
#include "constants.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

struct DisplayCell;

class DisplayBox : public utl::Entity {
public:
    DisplayBox(utl::Box& screen, utl::Vec2d pos);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    bool isActivated() { return isActive; }

    void activate();
    void deactivate();
    void updateShape(const TetrominoShape& newShape);
    const TetrominoShape& activeShape() { return displayedShape; }
    const utl::Vec2d& size() const override { return size_; }

private:
    void placeWalls();

    std::array<utl::Rect, constants::displayBoxWalls> walls;
    std::vector<DisplayCell> internalGrid;
    bool isActive;
    TetrominoShape displayedShape;
    utl::Vec2d size_;
};
