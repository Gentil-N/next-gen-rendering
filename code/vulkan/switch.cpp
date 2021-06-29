#include "core.hpp"

namespace ngr
{
       namespace api
       {
              VmaMemoryUsage switch_device_memory_type(software_device::MemoryType memory_type)
              {
                     switch (memory_type)
                     {
                     case software_device::MemoryType::CPU:
                            return VMA_MEMORY_USAGE_CPU_ONLY;
                     case software_device::MemoryType::CPU_TO_GPU:
                            return VMA_MEMORY_USAGE_CPU_TO_GPU;
                     case software_device::MemoryType::GPU:
                            return VMA_MEMORY_USAGE_GPU_ONLY;
                     case software_device::MemoryType::GPU_TO_CPU:
                            return VMA_MEMORY_USAGE_GPU_TO_CPU;
                     default:
                            return VMA_MEMORY_USAGE_UNKNOWN;
                     }
              }

              VkBufferUsageFlags switch_buffer_usage(buffer::Usage usage)
              {
                     VkBufferUsageFlags vk_usage = 0;
                     if (vk_bitwise_contain(usage, buffer::Usage::TRANSFER_SRC))
                     {
                            vk_usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::TRANSFER_DST))
                     {
                            vk_usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::UNIFORM))
                     {
                            vk_usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::VERTEX))
                     {
                            vk_usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::INDEX))
                     {
                            vk_usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::INDIRECT))
                     {
                            vk_usage |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
                     }
                     if (vk_bitwise_contain(usage, buffer::Usage::STORAGE))
                     {
                            vk_usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
                     }
                     return vk_usage;
              }

              VkFormat switch_texture_format(texture::Format format)
              {
                     switch (format)
                     {
                     case texture::Format::UNDEFINED:
                            return VK_FORMAT_UNDEFINED;

                     case texture::Format::R_8_UNORM:
                            return VK_FORMAT_R8_UNORM;
                     case texture::Format::R_8_SNORM:
                            return VK_FORMAT_R8_SNORM;
                     case texture::Format::R_8_USCALED:
                            return VK_FORMAT_R8_USCALED;
                     case texture::Format::R_8_SSCALED:
                            return VK_FORMAT_R8_SSCALED;
                     case texture::Format::R_8_UINT:
                            return VK_FORMAT_R8_UINT;
                     case texture::Format::R_8_SINT:
                            return VK_FORMAT_R8_SINT;
                     case texture::Format::R_8_SRGB:
                            return VK_FORMAT_R8_SRGB;

                     case texture::Format::RG_8_UNORM:
                            return VK_FORMAT_R8G8_UNORM;
                     case texture::Format::RG_8_SNORM:
                            return VK_FORMAT_R8G8_SNORM;
                     case texture::Format::RG_8_USCALED:
                            return VK_FORMAT_R8G8_USCALED;
                     case texture::Format::RG_8_SSCALED:
                            return VK_FORMAT_R8G8_SSCALED;
                     case texture::Format::RG_8_UINT:
                            return VK_FORMAT_R8G8_UINT;
                     case texture::Format::RG_8_SINT:
                            return VK_FORMAT_R8G8_SINT;
                     case texture::Format::RG_8_SRGB:
                            return VK_FORMAT_R8G8_SRGB;

                     case texture::Format::RGB_8_UNORM:
                            return VK_FORMAT_R8G8B8_UNORM;
                     case texture::Format::RGB_8_SNORM:
                            return VK_FORMAT_R8G8B8_SNORM;
                     case texture::Format::RGB_8_USCALED:
                            return VK_FORMAT_R8G8B8_USCALED;
                     case texture::Format::RGB_8_SSCALED:
                            return VK_FORMAT_R8G8B8_SSCALED;
                     case texture::Format::RGB_8_UINT:
                            return VK_FORMAT_R8G8B8_UINT;
                     case texture::Format::RGB_8_SINT:
                            return VK_FORMAT_R8G8B8_SINT;
                     case texture::Format::RGB_8_SRGB:
                            return VK_FORMAT_R8G8B8_SRGB;

                     case texture::Format::BGR_8_UNORM:
                            return VK_FORMAT_B8G8R8_UNORM;
                     case texture::Format::BGR_8_SNORM:
                            return VK_FORMAT_B8G8R8_SNORM;
                     case texture::Format::BGR_8_USCALED:
                            return VK_FORMAT_B8G8R8_USCALED;
                     case texture::Format::BGR_8_SSCALED:
                            return VK_FORMAT_B8G8R8_SSCALED;
                     case texture::Format::BGR_8_UINT:
                            return VK_FORMAT_B8G8R8_UINT;
                     case texture::Format::BGR_8_SINT:
                            return VK_FORMAT_B8G8R8_SINT;
                     case texture::Format::BGR_8_SRGB:
                            return VK_FORMAT_B8G8R8_SRGB;

                     case texture::Format::RGBA_8_UNORM:
                            return VK_FORMAT_R8G8B8A8_UNORM;
                     case texture::Format::RGBA_8_SNORM:
                            return VK_FORMAT_R8G8B8A8_SNORM;
                     case texture::Format::RGBA_8_USCALED:
                            return VK_FORMAT_R8G8B8A8_USCALED;
                     case texture::Format::RGBA_8_SSCALED:
                            return VK_FORMAT_R8G8B8A8_SSCALED;
                     case texture::Format::RGBA_8_UINT:
                            return VK_FORMAT_R8G8B8A8_UINT;
                     case texture::Format::RGBA_8_SINT:
                            return VK_FORMAT_R8G8B8A8_SINT;
                     case texture::Format::RGBA_8_SRGB:
                            return VK_FORMAT_R8G8B8A8_SRGB;

                     case texture::Format::BGRA_8_UNORM:
                            return VK_FORMAT_B8G8R8A8_UNORM;
                     case texture::Format::BGRA_8_SNORM:
                            return VK_FORMAT_B8G8R8A8_SNORM;
                     case texture::Format::BGRA_8_USCALED:
                            return VK_FORMAT_B8G8R8A8_USCALED;
                     case texture::Format::BGRA_8_SSCALED:
                            return VK_FORMAT_B8G8R8A8_SSCALED;
                     case texture::Format::BGRA_8_UINT:
                            return VK_FORMAT_B8G8R8A8_UINT;
                     case texture::Format::BGRA_8_SINT:
                            return VK_FORMAT_B8G8R8A8_SINT;
                     case texture::Format::BGRA_8_SRGB:
                            return VK_FORMAT_B8G8R8A8_SRGB;

                     case texture::Format::R_16_UNORM:
                            return VK_FORMAT_R16_UNORM;
                     case texture::Format::R_16_SNORM:
                            return VK_FORMAT_R16_SNORM;
                     case texture::Format::R_16_USCALED:
                            return VK_FORMAT_R16_USCALED;
                     case texture::Format::R_16_SSCALED:
                            return VK_FORMAT_R16_SSCALED;
                     case texture::Format::R_16_UINT:
                            return VK_FORMAT_R16_UINT;
                     case texture::Format::R_16_SINT:
                            return VK_FORMAT_R16_SINT;
                     case texture::Format::R_16_SFLOAT:
                            return VK_FORMAT_R16_SFLOAT;

                     case texture::Format::RG_16_UNORM:
                            return VK_FORMAT_R16G16_UNORM;
                     case texture::Format::RG_16_SNORM:
                            return VK_FORMAT_R16G16_SNORM;
                     case texture::Format::RG_16_USCALED:
                            return VK_FORMAT_R16G16_USCALED;
                     case texture::Format::RG_16_SSCALED:
                            return VK_FORMAT_R16G16_SSCALED;
                     case texture::Format::RG_16_UINT:
                            return VK_FORMAT_R16G16_UINT;
                     case texture::Format::RG_16_SINT:
                            return VK_FORMAT_R16G16_SINT;
                     case texture::Format::RG_16_SFLOAT:
                            return VK_FORMAT_R16G16_SFLOAT;

                     case texture::Format::RGB_16_UNORM:
                            return VK_FORMAT_R16G16B16_UNORM;
                     case texture::Format::RGB_16_SNORM:
                            return VK_FORMAT_R16G16B16_SNORM;
                     case texture::Format::RGB_16_USCALED:
                            return VK_FORMAT_R16G16B16_USCALED;
                     case texture::Format::RGB_16_SSCALED:
                            return VK_FORMAT_R16G16B16_SSCALED;
                     case texture::Format::RGB_16_UINT:
                            return VK_FORMAT_R16G16B16_UINT;
                     case texture::Format::RGB_16_SINT:
                            return VK_FORMAT_R16G16B16_SINT;
                     case texture::Format::RGB_16_SFLOAT:
                            return VK_FORMAT_R16G16B16_SFLOAT;

                     case texture::Format::RGBA_16_UNORM:
                            return VK_FORMAT_R16G16B16A16_UNORM;
                     case texture::Format::RGBA_16_SNORM:
                            return VK_FORMAT_R16G16B16A16_SNORM;
                     case texture::Format::RGBA_16_USCALED:
                            return VK_FORMAT_R16G16B16A16_USCALED;
                     case texture::Format::RGBA_16_SSCALED:
                            return VK_FORMAT_R16G16B16A16_SSCALED;
                     case texture::Format::RGBA_16_UINT:
                            return VK_FORMAT_R16G16B16A16_UINT;
                     case texture::Format::RGBA_16_SINT:
                            return VK_FORMAT_R16G16B16A16_SINT;
                     case texture::Format::RGBA_16_SFLOAT:
                            return VK_FORMAT_R16G16B16A16_SFLOAT;

                     case texture::Format::R_32_UINT:
                            return VK_FORMAT_R32_UINT;
                     case texture::Format::R_32_SINT:
                            return VK_FORMAT_R32_SINT;
                     case texture::Format::R_32_SFLOAT:
                            return VK_FORMAT_R32_SFLOAT;

                     case texture::Format::RG_32_UINT:
                            return VK_FORMAT_R32G32_UINT;
                     case texture::Format::RG_32_SINT:
                            return VK_FORMAT_R32G32_SINT;
                     case texture::Format::RG_32_SFLOAT:
                            return VK_FORMAT_R32G32_SFLOAT;

                     case texture::Format::RGB_32_UINT:
                            return VK_FORMAT_R32G32B32_UINT;
                     case texture::Format::RGB_32_SINT:
                            return VK_FORMAT_R32G32B32_SINT;
                     case texture::Format::RGB_32_SFLOAT:
                            return VK_FORMAT_R32G32B32_SFLOAT;

                     case texture::Format::RGBA_32_UINT:
                            return VK_FORMAT_R32G32B32A32_UINT;
                     case texture::Format::RGBA_32_SINT:
                            return VK_FORMAT_R32G32B32A32_SINT;
                     case texture::Format::RGBA_32_SFLOAT:
                            return VK_FORMAT_R32G32B32A32_SFLOAT;

                     case texture::Format::R_64_UINT:
                            return VK_FORMAT_R64_UINT;
                     case texture::Format::R_64_SINT:
                            return VK_FORMAT_R64_SINT;
                     case texture::Format::R_64_SFLOAT:
                            return VK_FORMAT_R64_SFLOAT;

                     case texture::Format::RG_64_UINT:
                            return VK_FORMAT_R64G64_UINT;
                     case texture::Format::RG_64_SINT:
                            return VK_FORMAT_R64G64_SINT;
                     case texture::Format::RG_64_SFLOAT:
                            return VK_FORMAT_R64G64_SFLOAT;

                     case texture::Format::RGB_64_UINT:
                            return VK_FORMAT_R64G64B64_UINT;
                     case texture::Format::RGB_64_SINT:
                            return VK_FORMAT_R64G64B64_SINT;
                     case texture::Format::RGB_64_SFLOAT:
                            return VK_FORMAT_R64G64B64_SFLOAT;

                     case texture::Format::RGBA_64_UINT:
                            return VK_FORMAT_R64G64B64A64_UINT;
                     case texture::Format::RGBA_64_SINT:
                            return VK_FORMAT_R64G64B64A64_SINT;
                     case texture::Format::RGBA_64_SFLOAT:
                            return VK_FORMAT_R64G64B64A64_SFLOAT;

                     case texture::Format::D16_UNORM:
                            return VK_FORMAT_D16_UNORM;
                     case texture::Format::D32_SFLOAT:
                            return VK_FORMAT_D32_SFLOAT;
                     case texture::Format::S8_UINT:
                            return VK_FORMAT_S8_UINT;
                     case texture::Format::D16_UNORM_S8_UINT:
                            return VK_FORMAT_D16_UNORM_S8_UINT;
                     case texture::Format::D24_UNORM_S8_UINT:
                            return VK_FORMAT_D24_UNORM_S8_UINT;
                     case texture::Format::D32_SFLOAT_S8_UINT:
                            return VK_FORMAT_D32_SFLOAT_S8_UINT;

                     default:
                            return VK_FORMAT_UNDEFINED;
                     }
              }

              VkImageLayout switch_texture_layout(texture::Layout layout)
              {
                     switch (layout)
                     {
                     case texture::Layout::UNDEFINED:
                            return VK_IMAGE_LAYOUT_UNDEFINED;
                     case texture::Layout::COLOR_ATTACHMENT:
                            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                     case texture::Layout::DEPTH_STENCIL_ATTACHMENT:
                            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                     case texture::Layout::DEPTH_STENCIL_READ_ONLY:
                            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                     case texture::Layout::SHADER_READ_ONLY:
                            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                     case texture::Layout::TRANSFER_SRC:
                            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                     case texture::Layout::TRANSFER_DST:
                            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                     case texture::Layout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT:
                            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
                     case texture::Layout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY:
                            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
                     case texture::Layout::DEPTH_ATTACHMENT:
                            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
                     case texture::Layout::DEPTH_READ_ONLY:
                            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
                     case texture::Layout::STENCIL_ATTACHMENT:
                            return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
                     case texture::Layout::STENCIL_READ_ONLY:
                            return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
                     case texture::Layout::PRESENT:
                            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                     default:
                            return VK_IMAGE_LAYOUT_UNDEFINED;
                     }
              }

              VkImageUsageFlags switch_texture_usage(texture::Usage usage)
              {
                     VkImageUsageFlags vk_usage;
                     if (vk_bitwise_contain(usage, texture::Usage::TRANSFER_DST))
                     {
                            vk_usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::TRANSFER_SRC))
                     {
                            vk_usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::SAMPLED))
                     {
                            vk_usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::STORAGE))
                     {
                            vk_usage |= VK_IMAGE_USAGE_STORAGE_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::COLOR_ATTACHMENT))
                     {
                            vk_usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::DEPTH_STENCIL_ATTACHMENT))
                     {
                            vk_usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::TRANSIENT_ATTACHMENT))
                     {
                            vk_usage |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::INPUT_ATTACHMENT))
                     {
                            vk_usage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
                     }
                     return vk_usage;
              }

              VkImageAspectFlags get_image_aspect_from_texture_usage(texture::Usage usage)
              {
                     VkImageAspectFlags vk_aspect = 0;
                     /*if(vk_bitwise_contain(usage, texture::Usage::TRANSFER_DST))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }
                     if(vk_bitwise_contain(usage, texture::Usage::TRANSFER_SRC))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }
                     if(vk_bitwise_contain(usage, texture::Usage::SAMPLED))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }
                     if(vk_bitwise_contain(usage, texture::Usage::STORAGE))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }*/
                     if (vk_bitwise_contain(usage, texture::Usage::COLOR_ATTACHMENT))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }
                     if (vk_bitwise_contain(usage, texture::Usage::DEPTH_STENCIL_ATTACHMENT))
                     {
                            vk_aspect |= (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
                     }
                     /*if(vk_bitwise_contain(usage, texture::Usage::TRANSIENT_ATTACHMENT))
                     {
                            vk_aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
                     }
                     if(vk_bitwise_contain(usage, texture::Usage::INPUT_ATTACHMENT))
                     {
                            vk_aspect |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
                     }*/
                     return vk_aspect;
              }

              VkShaderStageFlags switch_shader_type(shader::Type type)
              {
                     VkShaderStageFlags vk_stage = 0;
                     if (type == shader::Type::ALL)
                     {
                            return VK_SHADER_STAGE_ALL;
                     }
                     if (vk_bitwise_contain(type, shader::Type::VERTEX))
                     {
                            vk_stage |= VK_SHADER_STAGE_VERTEX_BIT;
                     }
                     if (vk_bitwise_contain(type, shader::Type::TESSELLATION_CONTROL))
                     {
                            vk_stage |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                     }
                     if (vk_bitwise_contain(type, shader::Type::TESSELLATION_EVALUATION))
                     {
                            vk_stage |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                     }
                     if (vk_bitwise_contain(type, shader::Type::GEOMETRY))
                     {
                            vk_stage |= VK_SHADER_STAGE_GEOMETRY_BIT;
                     }
                     if (vk_bitwise_contain(type, shader::Type::FRAGMENT))
                     {
                            vk_stage |= VK_SHADER_STAGE_FRAGMENT_BIT;
                     }
                     if (vk_bitwise_contain(type, shader::Type::COMPUTE))
                     {
                            vk_stage |= VK_SHADER_STAGE_COMPUTE_BIT;
                     }
                     return vk_stage;
              }

              VkAttachmentLoadOp switch_attachment_load_op(render_pass::AttachmentLoadOp load_op)
              {
                     switch (load_op)
                     {
                     case render_pass::AttachmentLoadOp::LOAD:
                            return VK_ATTACHMENT_LOAD_OP_LOAD;
                     case render_pass::AttachmentLoadOp::CLEAR:
                            return VK_ATTACHMENT_LOAD_OP_CLEAR;
                     case render_pass::AttachmentLoadOp::DISCARD:
                            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                     }
              }

              VkAttachmentStoreOp switch_attachment_store_op(render_pass::AttachmentStoreOp store_op)
              {
                     switch (store_op)
                     {
                     case render_pass::AttachmentStoreOp::STORE:
                            return VK_ATTACHMENT_STORE_OP_STORE;
                     case render_pass::AttachmentStoreOp::DISCARD:
                            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
                     }
              }

              VkPipelineStageFlags switch_pipeline_stage(pipeline::Stage stage)
              {
                     VkPipelineStageFlags vk_stage = 0;
                     if (vk_bitwise_contain(stage, pipeline::Stage::TOP_OF_PIPE))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::DRAW_INDIRECT))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::VERTEX_INPUT))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::VERTEX_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::TESSELLATION_CONTROL_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::TESSELLATION_EVALUATION_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::GEOMETRY_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::FRAGMENT_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::EARLY_FRAGMENT_TESTS))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::LATE_FRAGMENT_TESTS))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::COLOR_ATTACHMENT))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::COMPUTE_SHADER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::TRANSFER))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_TRANSFER_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::BOTTOM_OF_PIPE))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                     }
                     if (vk_bitwise_contain(stage, pipeline::Stage::HOST))
                     {
                            vk_stage |= VK_PIPELINE_STAGE_HOST_BIT;
                     }
                     return vk_stage;
              }

              VkAccessFlags switch_pipeline_access(pipeline::Access access)
              {
                     VkAccessFlags vk_access = 0;
                     if (vk_bitwise_contain(access, pipeline::Access::INDIRECT_COMMAND_READ))
                     {
                            vk_access |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::INDEX_READ))
                     {
                            vk_access |= VK_ACCESS_INDEX_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::VERTEX_ATTRIBUTE_READ))
                     {
                            vk_access |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::UNIFORM_READ))
                     {
                            vk_access |= VK_ACCESS_UNIFORM_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::INPUT_ATTACHMENT_READ))
                     {
                            vk_access |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::SHADER_READ))
                     {
                            vk_access |= VK_ACCESS_SHADER_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::SHADER_WRITE))
                     {
                            vk_access |= VK_ACCESS_SHADER_WRITE_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::COLOR_ATTACHMENT_READ))
                     {
                            vk_access |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::COLOR_ATTACHMENT_WRITE))
                     {
                            vk_access |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::DEPTH_STENCIL_ATTACHMENT_READ))
                     {
                            vk_access |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::DEPTH_STENCIL_ATTACHMENT_WRITE))
                     {
                            vk_access |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::TRANSFER_READ))
                     {
                            vk_access |= VK_ACCESS_TRANSFER_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::TRANSFER_WRITE))
                     {
                            vk_access |= VK_ACCESS_TRANSFER_WRITE_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::HOST_READ))
                     {
                            vk_access |= VK_ACCESS_HOST_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::HOST_WRITE))
                     {
                            vk_access |= VK_ACCESS_HOST_WRITE_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::MEMORY_READ))
                     {
                            vk_access |= VK_ACCESS_MEMORY_READ_BIT;
                     }
                     if (vk_bitwise_contain(access, pipeline::Access::MEMORY_WRITE))
                     {
                            vk_access |= VK_ACCESS_MEMORY_WRITE_BIT;
                     }
                     return vk_access;
              }
       }
}