#ifndef CT_MEDIA_TEXTURE_MANAGER_HPP
#define CT_MEDIA_TEXTURE_MANAGER_HPP

#include "io/trace.hpp"
#include <bicudo/physics/types.hpp>
#include <ekg/ekg.hpp>
#include <ekg/os/ekg_opengl.hpp>

#define CT_NO_TEXTURE 969669

namespace ct {
  struct texture_upload_properties_t  {
  public:
    const char *p_tag {};
    const char *p_path {};
    bool repeated_uv {};
  };

  class texture_manager {
  public:
    std::vector<ekg::gpu::sampler_t> sampler_list {};
  public:
    bicudo::id get_sampler_id_by_tag(const char *p_tag);
    ekg::gpu::sampler_t &sampler(bicudo::id id);
    bicudo::id upload(ct::texture_upload_properties_t *p_texture_upload_properties);
  };
}

namespace ct {
  ct::result media_create_sampler(
    ekg::gpu::sampler_allocate_info *p_sampler_alloc_info,
    ekg::gpu::sampler_t *p_sampler
  );
}

#endif