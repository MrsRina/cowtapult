#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ekg::gpu::sampler_t &ct::texture_manager::sampler(bicudo::id id) {
  return this->sampler_list.at(id);
}

bicudo::id ct::texture_manager::upload(
  std::string_view tag,
  std::string_view path
) {
  bicudo::id latest_sampler_list_size {this->sampler_list.size()};
  ekg::gpu::sampler_t &sampler {this->sampler_list.emplace_back()};

  if (this->read_file(path, &sampler) == ct::results::FAILED) {
    return CT_NO_TEXTURE;
  }

  sampler.p_tag = tag.data();
  return latest_sampler_list_size;
}

ct::result ct::texture_manager::read_file(
  std::string_view path,
  ekg::gpu::sampler_t *p_sampler
) {
  ekg::gpu::sampler_allocate_info sampler_alloc_info {
    .p_tag = "oi"
  };

  sampler_alloc_info.p_data = {
    static_cast<unsigned char*>(
      stbi_load(
        path.data(),
        &sampler_alloc_info.w,
        &sampler_alloc_info.h,
        &sampler_alloc_info.gl_internal_format,
        0
      )
    )
  };

  if (!sampler_alloc_info.p_data) {
    ekg::log() << "Failed to load '" << path << "' sampler ><!";
    return ct::results::FAILED;
  }

  int32_t truly_png_or_no {GL_RGB};
  if (sampler_alloc_info.gl_internal_format == 4) {
    truly_png_or_no = GL_RGBA;
  }

  sampler_alloc_info.gl_wrap_modes[0] = GL_REPEAT;
  sampler_alloc_info.gl_wrap_modes[1] = GL_REPEAT;
  sampler_alloc_info.gl_parameter_filter[0] = GL_LINEAR;
  sampler_alloc_info.gl_parameter_filter[1] = GL_LINEAR;
  sampler_alloc_info.gl_internal_format = truly_png_or_no;
  sampler_alloc_info.gl_format = truly_png_or_no;
  sampler_alloc_info.gl_type = GL_UNSIGNED_BYTE;
  sampler_alloc_info.gl_generate_mipmap = GL_TRUE;

  ekg::allocate_sampler(
    &sampler_alloc_info,
    p_sampler
  );  

  return ct::results::SUCCESS;
}