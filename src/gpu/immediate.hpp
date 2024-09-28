#ifndef CT_GPU_IMMEDIATE_HPP
#define CT_GPU_IMMEDIATE_HPP

#include "opengl.hpp"
#include <bicudo/physics/placement.hpp>
#include "model.hpp"

namespace ct {
  class immediate {
  protected:
    ct::program_t program {};
    ct::draw_call_t draw_call {};
    bicudo::mat4 mat4x4_rotate {};
  public:
    bicudo::mat4 mat4x4_projection {};
    bicudo::vec2 latest_pos_clicked {};
    float current_zoom {1.0f};
  public:
    void create();
    void invoke();
    void revoke();

    void draw(
      bicudo::physics::placement &placement,
      bicudo::vec4 color,
      uint32_t bind_texture = 0
    );
  };
}

#endif