#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void ct::texture_manager::on_pre_init() {
  ct::texture_upload_properties_t upload {
    .p_tag = "cow-loading",
    .p_path = "./cow-loading.png"
  };

  this->upload(
    &upload
  );
}

void ct::texture_manager::on_init() {

}


bicudo::id ct::texture_manager::get_sampler_id_by_tag(const char *p_tag) {
  for (bicudo::id it {}; it < this->sampler_list.size(); it++) {
    ekg::gpu::sampler_t &sampler {
      this->sampler_list.at(it)
    };

    if (strcmp(sampler.p_tag, p_tag) == 0) {
      return it;
    }
  }

  return CT_NO_TEXTURE;
}

ekg::gpu::sampler_t &ct::texture_manager::sampler(bicudo::id id) {
  return this->sampler_list.at(id);
}

bicudo::id ct::texture_manager::upload(
  ct::texture_upload_properties_t *p_texture_upload_properties
) {
  bicudo::id latest_sampler_list_size {this->sampler_list.size()};
  ekg::gpu::sampler_t &sampler {this->sampler_list.emplace_back()};

  ekg::gpu::sampler_allocate_info sampler_alloc_info {
    .p_tag = p_texture_upload_properties->p_path
  };

  int32_t wrap_flag {p_texture_upload_properties->repeated_uv ? GL_REPEAT : GL_CLAMP_TO_EDGE};
  sampler_alloc_info.gl_wrap_modes[0] = wrap_flag;
  sampler_alloc_info.gl_wrap_modes[1] = wrap_flag;

  if (ct::media_create_sampler(&sampler_alloc_info, &sampler) == ct::results::FAILED) {
    return CT_NO_TEXTURE;
  }

  sampler.p_tag = p_texture_upload_properties->p_tag;
  return latest_sampler_list_size;
}

ct::result ct::media_create_sampler(
  ekg::gpu::sampler_allocate_info *p_sampler_alloc_info,
  ekg::gpu::sampler_t *p_sampler
) {
  p_sampler_alloc_info->p_data = {
    static_cast<unsigned char*>(
      stbi_load(
        p_sampler_alloc_info->p_tag,
        &p_sampler_alloc_info->w,
        &p_sampler_alloc_info->h,
        &p_sampler_alloc_info->gl_internal_format,
        0
      )
    )
  };

  if (!p_sampler_alloc_info->p_data) {
    ekg::log() << "Failed to load '" << p_sampler_alloc_info->p_tag << "' sampler ><!";
    return ct::results::FAILED;
  }

  int32_t truly_png_or_no {GL_RGB};
  if (p_sampler_alloc_info->gl_internal_format == 4) {
    truly_png_or_no = GL_RGBA;
  }

  p_sampler_alloc_info->gl_parameter_filter[0] = GL_LINEAR;
  p_sampler_alloc_info->gl_parameter_filter[1] = GL_LINEAR;
  p_sampler_alloc_info->gl_internal_format = truly_png_or_no;
  p_sampler_alloc_info->gl_format = truly_png_or_no;
  p_sampler_alloc_info->gl_type = GL_UNSIGNED_BYTE;

  ekg::allocate_sampler(
    p_sampler_alloc_info,
    p_sampler
  );  

  return ct::results::SUCCESS;
}