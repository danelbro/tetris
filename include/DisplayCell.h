#pragma once

#include "GridPoint.h"
#include "constants.h"
#include "utl_Stage.hpp"

#include <array>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class DisplayBox;

struct DisplayCell : public utl::Entity {
    DisplayCell(DisplayBox& grid, const GridPoint& coord,
                const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Vec2d& size() const override { return size_; };
    const utl::Stage& stage() const override { return owner_; }

    void setCol(const utl::Colour& newCol);

    utl::Colour col_;
    utl::Colour borderCol_;
    DisplayBox& grid_;
    GridPoint coord_;
    bool renderMe_;
    utl::Rect rect_;
    std::array<utl::Rect, constants::gridWalls> borders_;

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Vec2d size_;
    const utl::Stage& owner_;
};
