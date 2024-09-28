#include "model.hpp"
#include "opengl.hpp"
#include <bicudo/util/log.hpp>
#include <iostream>

ct::result ct::gpu_create_program(
  ct::program_create_info *p_program_create_info,
  ct::program_t *p_program
) {
  if (p_program_create_info->shader_list.empty()) {
    bicudo::log() << "Error: Invalid shader, empty resources";
    return bicudo::types::FAILED;
  }

  std::string shader_src {};
  std::vector<uint32_t> compiled_shader_list {};
  int32_t status {};
  std::string msg {};

  for (ct::shader_t &module : p_program_create_info->shader_list) {
    uint32_t shader {glCreateShader(module.stage)};
    glShaderSource(shader, 1, &module.p_src, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
      msg.resize(status);
      glGetShaderInfoLog(shader, status, nullptr, msg.data());
      bicudo::log() << "Error: Failed to compile shader '" << module.p_src  << "' stage: \n" << msg;
      break;
    }

    compiled_shader_list.push_back(shader);
  }

  bool keep {compiled_shader_list.size() == p_program_create_info->shader_list.size()};
  p_program->id = glCreateProgram();

  for (uint32_t &shaders : compiled_shader_list) {
    if (keep) {
      glAttachShader(p_program->id, shaders);
    }

    glDeleteShader(shaders);
  }

  if (keep) {
    glLinkProgram(p_program->id);
    glGetProgramiv(p_program->id, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
      glGetProgramiv(p_program->id, GL_INFO_LOG_LENGTH, &status);
      msg.resize(status);
      glGetProgramInfoLog(p_program->id, status, nullptr, msg.data());
      bicudo::log() << "Error: Failed to link program: \n" << msg;
    }
  }

  return ct::results::SUCCESS;
}

uint32_t &ct::program_t::operator[](std::string_view key) {
  return this->program_location_map[key];
}

ct::result ct::gpu_uniform_registry(
  ct::program_t *p_program,
  std::string_view key
) {
  int32_t location {
    glGetUniformLocation(
      p_program->id,
      key.data()
    )
  };

  if (location == -1) {
    return ct::results::FAILED;
  }

  p_program->program_location_map[key] = location;
  return ct::results::SUCCESS;
}

ct::result ct::gpu_dispatch_draw_call(
  ct::draw_call_t *p_draw_call
) {
  if (p_draw_call == nullptr) {
    return ct::results::FAILED;
  }

  switch (p_draw_call->mode) {
  case bicudo::types::INDEXED:
    glDrawElements(
      p_draw_call->polygon_type,
      p_draw_call->size,
      p_draw_call->index_type,
      (void*) p_draw_call->offset
    );
    break;
  case bicudo::types::ARRAYS:
    glDrawArrays(
      p_draw_call->polygon_type,
      p_draw_call->offset,
      p_draw_call->size
    );
    break;
  }

  return ct::results::SUCCESS;
}