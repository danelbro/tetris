#pragma once

#include "GridPoint.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <string>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class TetrisGame;
class DisplayBox;

struct DisplayCell : public utl::Entity {
    DisplayCell() = default;
    DisplayCell(TetrisGame* owner, const DisplayBox& displayBox,
                const GridPoint& coord, const utl::Colour& colour);

    void update(double t, double dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;
    void set_pos(const utl::Vec2d& newPos) override final;

    void setCol(const utl::Colour& newCol);

private:
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)};
    TetrisGame* owner_{nullptr};
    utl::Vec2d pos_{};
    utl::Size size_{constants::displayCellWidth, constants::displayCellHeight};

public:
    utl::Colour col_{colours::Z_tetrominoCol};
    utl::Colour borderCol_{colours::Z_tetrominoCol};
    GridPoint coord_{};
    bool renderMe_{false};
    utl::Rect rect_{};
    std::array<utl::Rect, constants::gridWalls> borders_{};
};
