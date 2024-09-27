#ifndef CT_GPU_IMMEDIATE_HPP
#define CT_GPU_IMMEDIATE_HPP

#include "opengl.hpp"
#include <bicudo/physics/placement.hpp>
#include "model.hpp"

namespace ct {
  class immediate {
  protected:
    uint32_t program {};
    meow::gpu::uniform uniform {};
    meow::gpu::draw_call_t draw_call {};
    bicudo::mat4 mat4x4_rotate {};
  public:
    bicudo::mat4 mat4x4_projection {};
    bicudo::vec2 latest_pos_clicked {};
    bicudo::vec4 viewport {};
    float current_zoom {1.0f};
  public:
    void create();
    void invoke();
    void revoke();

    /**
     * May be not actually idk idc
     **/
    void draw(
      bicudo::vec4 rect,
      bicudo::vec4 color,
      float angle,
      uint32_t bind_texture = 0
    );
  };
}

#endif