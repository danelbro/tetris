#include "Grid.h"

#include <cstdint>
#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

struct ScoresPacket {
    int score;
    int level;
    int lines;
};

class EndScreen : public utl::Stage {
public:
    EndScreen(utl::Box& screen_space, uint32_t windowID,
              utl::Renderer& renderer, const Grid& grid,
              const ScoresPacket& scoresPacket);

    std::string update(double t, double dt) override;
    std::string
    handle_input(double t, double dt,
                 std::array<bool, utl::KeyFlag::K_TOTAL>& keyState) override;

    void render(double t, double dt) override;

private:
    Grid grid_;
    int score_;
    int level_;
    int lines_;

    utl::Font smallFont_;
    utl::Font largeFont_;

    utl::TextObject gameText_;
    utl::TextObject overText_;
    utl::TextObject scoreTitle_;
    utl::TextObject scoreText_;
    utl::TextObject levelTitle_;
    utl::TextObject levelText_;
    utl::TextObject linesTitle_;
    utl::TextObject linesText_;
};
