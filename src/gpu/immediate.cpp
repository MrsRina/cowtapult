#include "immediate.hpp"
#include "model.hpp"

void ct::immmediate::create() {
  ct::program_create_info program_create_info {
    .p_tag = "immediate",
    .shader_list =
    {
      {
        .stage = GL_VERTEX_SHADER,
        .p_src = R"(
        #version 450 core

        layout (location = 0) in vec2 aPos; 

        uniform mat4 uRotate;
        uniform vec4 uRect;
        uniform mat4 uProjection;

        out vec2 vUV;
        out vec4 vRect;

        void main() {
          gl_Position = uProjection * (uRotate * vec4((aPos * uRect.zw) + uRect.xy, 0.0f, 1.0f));
          vRect = uRect;
          vUV = aPos;
        }
        )"
      },
      {
        .stage = GL_FRAGMENT_SHADER,
        .p_src = R"(
        #version 450 core

        layout (location = 0) out vec4 vFragColor;
        layout (binding = 0) uniform sampler2D uSampler;

        uniform bool uSamplerEnabled;
        uniform vec4 uColor;

        in vec2 vUV;
        in vec4 vRect;

        void main() {
          vec4 outColor = uColor;
          if (uSamplerEnabled) {
            outColor = texture(uSampler, vUV);
          }

          vFragColor = outColor;
        })"
      }
    }
  };

  ct::gpu_create_program(
    &program_create_info,
    &this->program
  );

  ct::gpu_uniform_registry(&this->program, "uSamplerEnabled");
  ct::gpu_uniform_registry(&this->program, "uColor");
  ct::gpu_uniform_registry(&this->program, "uRect");
  ct::gpu_uniform_registry(&this->program, "uRotate");
  ct::gpu_uniform_registry(&this->program, "uProjection");

  this->draw_call.polygon_type = GL_TRIANGLES;
  this->draw_call.index_type = GL_UNSIGNED_BYTE;
  this->draw_call.mode = bicudo::types::INDEXED;
  this->draw_call.offset = 0;
  this->draw_call.size = 6;
  this->draw_call.buffers.resize(2);

  glCreateVertexArrays(1, &this->draw_call.vao);
  glCreateBuffers(2, this->draw_call.buffers.data());

  glBindVertexArray(this->draw_call.vao);

  float vertices[8] {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };

  glBindBuffer(GL_ARRAY_BUFFER, this->draw_call.buffers.at(0));
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  uint8_t indices[6] {
    0, 1, 3,
    3, 2, 0
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->draw_call.buffers.at(1));
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void ct::immmediate::update_viewport() {
  float w {static_cast<float>(ct::p_app->window.w)};
  float h {static_cast<float>(ct::p_app->window.h)};

  this->mat4x4_projection = bicudo::ortho(
    0.0f,
    w,
    h,
    0.0f
  );

  ct::camera &camera {ct::p_app->camera};
  bicudo::vec2 center {w / 2, h / 2}; // RINA PLEASE ADD DYNAMIC ZOOM FOR TARGET ENTITIES

  bicudo::vec2 delta {(center / this->current_zoom) + camera.placement.pos};
  this->current_zoom = camera.zoom;
  camera.placement.pos = delta - (center / this->current_zoom);

  this->mat4x4_projection = bicudo::scale(this->mat4x4_projection, {this->current_zoom, this->current_zoom, 1.0f});

  glProgramUniformMatrix4fv(
    this->program.id,
    this->program["uProjection"],
    1,
    GL_FALSE,
    this->mat4x4_projection.data()
  );
}

void ct::immmediate::invoke() {
  glUseProgram(this->program.id);
  glBindVertexArray(this->draw_call.vao);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ct::immmediate::draw(
  bicudo::vec4 rect,
  bicudo::vec4 color,
  float angle,
  uint32_t bind_texture
) {
  this->mat4x4_rotate = bicudo::mat4(1.0f);

  if (!bicudo::assert_float(angle, 0.0f)) {
    bicudo::vec2 center {
      rect.x + (rect.z / 2), rect.y + (rect.w / 2)
    };

    this->mat4x4_rotate = bicudo::translate(this->mat4x4_rotate, center);
    this->mat4x4_rotate = bicudo::rotate(this->mat4x4_rotate, {0.0f, 0.0f, 1.0f}, -angle);
    this->mat4x4_rotate = bicudo::translate(this->mat4x4_rotate, -center);
  }

  glUniformMatrix4fv(
    this->program["uRotate"],
    1,
    GL_FALSE,
    this->mat4x4_rotate.data()
  );

  glUniform4fv(
    this->program["uRect"],
    1,
    rect.data()
  );

  glUniform4fv(
    this->program["uColor"],
    1,
    color.data()
  );

  if (bind_texture > 0) {
    glUniform1i(
      this->program["uSamplerEnabled"],
      true
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bind_texture);
  }

  ct::gpu_dispatch_draw_call(&this->draw_call);
}

void ct::immmediate::revoke() {
  glUseProgram(0);
  glBindVertexArray(0);
}