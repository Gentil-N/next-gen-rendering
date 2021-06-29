#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace texture
              {
                     Texture create(SoftwareDevice software_device, const VkImageCreateInfo &image_info, VkImageViewCreateInfo &image_view_info, software_device::MemoryType memory_type)
                     {
                            Texture texture = new Texture_I();
                            vk_assert(software_device->func.vk_create_image(software_device->vk_device, &image_info, nullptr, &texture->vk_image));
                            image_view_info.image = texture->vk_image;
                            vk_assert(software_device->func.vk_create_image_view(software_device->vk_device, &image_view_info, nullptr, &texture->vk_image_view));
                            VmaAllocationCreateInfo alloc_info = {0};
                            alloc_info.usage = switch_device_memory_type(memory_type);
                            vk_assert(vmaAllocateMemoryForImage(software_device->vma_allocator, texture->vk_image, &alloc_info, &texture->vma_allocation, nullptr));
                            vk_assert(vmaBindImageMemory(software_device->vma_allocator, texture->vma_allocation, texture->vk_image));
                            texture->map = nullptr;
                            return texture;
                     }

                     Texture create_1d(SoftwareDevice software_device, const CreateInfo1D &create_info_1d)
                     {
                            VkFormat format = switch_texture_format(create_info_1d.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0, VK_IMAGE_TYPE_1D, format, VkExtent3D{create_info_1d.width, 1, 1}, create_info_1d.mip_level_count,
                                 1, VK_SAMPLE_COUNT_1_BIT, (create_info_1d.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL), switch_texture_usage(create_info_1d.usage),
                                 software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_1D, format, {}, {get_image_aspect_from_texture_usage(create_info_1d.usage), 0, create_info_1d.mip_level_count, 0, 1}};
                            return create(software_device, image_info, image_view_info, create_info_1d.memory_type);
                     }

                     Texture create_1d_array(SoftwareDevice software_device, const CreateInfo1DArray &create_info_1d_array)
                     {
                            VkFormat format = switch_texture_format(create_info_1d_array.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0, VK_IMAGE_TYPE_1D, format, VkExtent3D{create_info_1d_array.width, 1, 1}, create_info_1d_array.mip_level_count,
                                 create_info_1d_array.array_layer_count, VK_SAMPLE_COUNT_1_BIT, (create_info_1d_array.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL),
                                 switch_texture_usage(create_info_1d_array.usage), software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_1D_ARRAY, format, {}, {get_image_aspect_from_texture_usage(create_info_1d_array.usage), 0, create_info_1d_array.mip_level_count, 0, create_info_1d_array.array_layer_count}};
                            return create(software_device, image_info, image_view_info, create_info_1d_array.memory_type);
                     }

                     Texture create_2d(SoftwareDevice software_device, const CreateInfo2D &create_info_2d)
                     {
                            VkFormat format = switch_texture_format(create_info_2d.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0, VK_IMAGE_TYPE_2D, format, VkExtent3D{create_info_2d.width, create_info_2d.height, 1}, create_info_2d.mip_level_count,
                                 1, VK_SAMPLE_COUNT_1_BIT, (create_info_2d.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL), switch_texture_usage(create_info_2d.usage),
                                 software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_2D, format, {}, {get_image_aspect_from_texture_usage(create_info_2d.usage), 0, create_info_2d.mip_level_count, 0, 1}};
                            return create(software_device, image_info, image_view_info, create_info_2d.memory_type);
                     }

                     Texture create_2d_array(SoftwareDevice software_device, const CreateInfo2DArray &create_info_2d_array)
                     {
                            VkFormat format = switch_texture_format(create_info_2d_array.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0, VK_IMAGE_TYPE_2D, format, VkExtent3D{create_info_2d_array.width, create_info_2d_array.height, 1}, create_info_2d_array.mip_level_count,
                                 create_info_2d_array.array_layer_count, VK_SAMPLE_COUNT_1_BIT, (create_info_2d_array.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL),
                                 switch_texture_usage(create_info_2d_array.usage), software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_2D_ARRAY, format, {}, {get_image_aspect_from_texture_usage(create_info_2d_array.usage), 0, create_info_2d_array.mip_level_count, 0, create_info_2d_array.array_layer_count}};
                            return create(software_device, image_info, image_view_info, create_info_2d_array.memory_type);
                     }

                     Texture create_cube(SoftwareDevice software_device, const CreateInfoCube &create_info_cube)
                     {
                            VkFormat format = switch_texture_format(create_info_cube.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, VK_IMAGE_TYPE_2D, format, VkExtent3D{create_info_cube.width, create_info_cube.width, 1},
                                 create_info_cube.mip_level_count, 6, VK_SAMPLE_COUNT_1_BIT, (create_info_cube.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL),
                                 switch_texture_usage(create_info_cube.usage), software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_CUBE, format, {}, {get_image_aspect_from_texture_usage(create_info_cube.usage), 0, create_info_cube.mip_level_count, 0, 6}};
                            return create(software_device, image_info, image_view_info, create_info_cube.memory_type);
                     }

                     Texture create_cube_array(SoftwareDevice software_device, const CreateInfoCubeArray &create_info_cube_array)
                     {
                            VkFormat format = switch_texture_format(create_info_cube_array.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, VK_IMAGE_TYPE_2D, format, VkExtent3D{create_info_cube_array.width, create_info_cube_array.width, 1},
                                 create_info_cube_array.mip_level_count, 6 * create_info_cube_array.array_layer_cube_count, VK_SAMPLE_COUNT_1_BIT,
                                 (create_info_cube_array.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL), switch_texture_usage(create_info_cube_array.usage),
                                 software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_CUBE_ARRAY, format, {}, {get_image_aspect_from_texture_usage(create_info_cube_array.usage), 0, create_info_cube_array.mip_level_count, 0, 6 * create_info_cube_array.array_layer_cube_count}};
                            return create(software_device, image_info, image_view_info, create_info_cube_array.memory_type);
                     }

                     Texture create_3d(SoftwareDevice software_device, const CreateInfo3D &create_info_3d)
                     {
                            VkFormat format = switch_texture_format(create_info_3d.format);
                            VkImageCreateInfo image_info =
                                {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr, 0, VK_IMAGE_TYPE_3D, format, VkExtent3D{create_info_3d.width, create_info_3d.height, create_info_3d.depth}, create_info_3d.mip_level_count,
                                 1, VK_SAMPLE_COUNT_1_BIT, (create_info_3d.tiling_linear ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL), switch_texture_usage(create_info_3d.usage),
                                 software_device->vk_sharing_mode, (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            VkImageViewCreateInfo image_view_info =
                                {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, VK_NULL_HANDLE, VK_IMAGE_VIEW_TYPE_3D, format, {}, {get_image_aspect_from_texture_usage(create_info_3d.usage), 0, create_info_3d.mip_level_count, 0, 1}};
                            return create(software_device, image_info, image_view_info, create_info_3d.memory_type);
                     }

                     void destroy(SoftwareDevice software_device, Texture texture)
                     {
                            if (texture->map)
                            {
                                   vmaUnmapMemory(software_device->vma_allocator, texture->vma_allocation);
                            }
                            vmaFreeMemory(software_device->vma_allocator, texture->vma_allocation);
                            software_device->func.vk_destroy_image_view(software_device->vk_device, texture->vk_image_view, nullptr);
                            software_device->func.vk_destroy_image(software_device->vk_device, texture->vk_image, nullptr);
                            delete texture;
                     }

                     void *map(SoftwareDevice software_device, Texture texture)
                     {
                            vk_assert(vmaMapMemory(software_device->vma_allocator, texture->vma_allocation, &texture->map));
                            return texture->map;
                     }

                     void unmap(SoftwareDevice software_device, Texture texture)
                     {
                            vmaUnmapMemory(software_device->vma_allocator, texture->vma_allocation);
                            texture->map = nullptr;
                     }

                     void *get_map(Texture texture)
                     {
                            return texture->map;
                     }

                     Sampler create_sampler(SoftwareDevice software_device, const CreateInfoSampler &create_info_sampler)
                     {
                            Sampler sampler = new Sampler_I();
                            VkSamplerCreateInfo sampler_info = {
                                VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT,
                                VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_ALWAYS, 0.0f, 0.0f,
                                VK_BORDER_COLOR_INT_OPAQUE_BLACK, VK_FALSE};
                            vk_assert(software_device->func.vk_create_sampler(software_device->vk_device, &sampler_info, nullptr, &sampler->vk_sampler));
                            return sampler;
                     }

                     void destroy_sampler(SoftwareDevice software_device, Sampler sampler)
                     {
                            software_device->func.vk_destroy_sampler(software_device->vk_device, sampler->vk_sampler, nullptr);
                            delete sampler;
                     }
              }
       }
}