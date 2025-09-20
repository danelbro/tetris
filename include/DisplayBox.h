// -*- C++ -*
#pragma once

#include "DisplayCell.h"
#include "TetrominoShape.h"

#include <chrono>
#include <string>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

class TetrisGame;

class DisplayBox : public utl::Entity {
public:
    DisplayBox() = default;
    DisplayBox(TetrisGame* owner);
    DisplayBox(TetrisGame* owner, utl::Vec2d pos);

    void update(std::chrono::milliseconds t,
                std::chrono::milliseconds dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;

    void set_pos(const utl::Vec2d& new_pos) override final;

    bool isActivated();

    void activate();
    void deactivate();
    void updateShape(const TetrominoShape& newShape);
    const TetrominoShape& activeShape();

private:
    void populateGrid();
    void placeWalls();

private:
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::DISPLAYBOX)};
    TetrisGame* owner_{nullptr};
    utl::Vec2d pos_{0.0, 0.0};
    utl::Size size_{
        (constants::displayBoxWallsThickness * 2)
            + (constants::displayBoxGridWidth * constants::displayCellWidth),
        (constants::displayBoxWallsThickness * 2)
            + (constants::displayBoxGridHeight * constants::displayCellHeight)};
    std::array<utl::Rect, constants::displayBoxWalls> walls{};
    std::vector<DisplayCell> internalGrid{};
    bool isActive{false};
    TetrominoShape displayedShape{Z_tetromino};
};
