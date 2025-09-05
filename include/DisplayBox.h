#pragma once

#include "DisplayCell.h"
#include "TetrominoShape.h"
#include "TetrisGame.h"
#include "constants.h"

#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

struct DisplayCell;

class DisplayBox : public utl::Entity {
public:
    DisplayBox(TetrisGame& owner);
    DisplayBox(TetrisGame& owner, utl::Vec2d pos);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Vec2d& size() const override { return size_; }
    const utl::Stage& stage() const override { return owner_; }

    void set_pos(double x, double y) override;
    void set_pos(const utl::Vec2d& new_pos) override;

    bool isActivated() { return isActive; }

    void activate();
    void deactivate();
    void updateShape(const TetrominoShape& newShape);
    const TetrominoShape& activeShape() { return displayedShape; }

private:
    void populateGrid();
    void placeWalls();

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Vec2d size_;
    TetrisGame& owner_;
    std::array<utl::Rect, constants::displayBoxWalls> walls;
    std::vector<DisplayCell> internalGrid;
    bool isActive;
    TetrominoShape displayedShape;
};
