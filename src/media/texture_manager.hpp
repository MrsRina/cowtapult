#ifndef CT_MEDIA_TEXTURE_MANAGER_HPP
#define CT_MEDIA_TEXTURE_MANAGER_HPP

#include "io/trace.hpp"
#include <bicudo/physics/types.hpp>
#include <ekg/ekg.hpp>
#include <ekg/os/ekg_opengl.hpp>

#define CT_NO_TEXTURE 969669

namespace ct {
  class texture_manager {
  public:
    std::vector<ekg::gpu::sampler_t> sampler_list {};
  public:
    ekg::gpu::sampler_t &sampler(bicudo::id id);
    bicudo::id upload(std::string_view tag, std::string_view path);
    ct::result read_file(std::string_view path, ekg::gpu::sampler_t *p_sampler);
  };
}

#endif