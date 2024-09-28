#ifndef CT_WORLD_CAMERA_HPP
#define CT_WORLD_CAMERA_HPP

#include "entity_base.hpp"
#include "io/game_context.hpp"

namespace ct {
  class camera : public ct::entity_base {
  public:
    float zoom {1.0f};
    float previous_zoom {1.0f};
    float interpolated_zoom {1.0f};
    bicudo::vec4 frustum {};
    bicudo::vec2 look_at {};
    float smooth {0.2f};

    bicudo::physics::placement *p_target_placement {};
    ct::camera_state state {};
    bicudo::vec2 prev_target_pos {};
  public:
    camera() = default;

    void set_target(bicudo::physics::placement *p_placement);
    void on_update() override;
  };
}

#endif