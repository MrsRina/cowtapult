#ifndef CT_GPU_MODEL_HPP
#define CT_GPU_MODEL_HPP

#include <cstdint>
#include "io/trace.hpp"
#include <vector>
#include <unordered_map>
#include <string_view>
#include <bicudo/gpu/types.hpp>

namespace ct {
  struct program_t {
  public:
    const char *p_tag {};
    uint32_t id {};
    std::unordered_map<std::string_view, uint32_t> program_location_map {};
  public:
    uint32_t &operator[](std::string_view uniform);
  };

  struct shader_t {
  public:
    uint32_t stage {};
    const char *p_src {};
  };

  struct program_create_info {
  public:
    const char *p_tag {};
    std::vector<ct::shader_t> shader_list {};
  };

  struct draw_call_t {
  public:
    uint32_t polygon_type {};
    uint32_t index_type {};
    bicudo::types mode {};
    uint32_t vao {};
    uint64_t size {};
    uint64_t offset {};
    std::vector<uint32_t> buffers {};
  };
}

namespace ct {
  ct::result gpu_create_program(
    ct::program_create_info *p_program_create_info,
    ct::program_t *p_program
  );

  ct::result gpu_uniform_registry(
    ct::program_t *p_program,
    std::string_view key
  );

  ct::result gpu_dispatch_draw_call(
    ct::draw_call_t *p_draw_call
  );
}

#endif