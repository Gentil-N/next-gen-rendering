#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace ext
              {
                     namespace swapchain
                     {
                            const std::uint32_t FRAME_COUNT_REQUIRED = 2;

                            VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR> &surface_formats)
                            {
                                   for (VkSurfaceFormatKHR surface_format : surface_formats)
                                   {
                                          if (surface_format.format == VK_FORMAT_R8G8B8A8_UNORM && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                                          {
                                                 return surface_format;
                                          }
                                   }
                                   return surface_formats[0];
                            }

                            VkExtent2D choose_extent(VkSurfaceCapabilitiesKHR surface_capabilities, uint32_t width, uint32_t height)
                            {
                                   if (surface_capabilities.currentExtent.width != UINT32_MAX && surface_capabilities.currentExtent.height != UINT32_MAX)
                                   {
                                          return surface_capabilities.currentExtent;
                                   }
                                   VkExtent2D extent;
                                   extent.width = vk_max(surface_capabilities.minImageExtent.width, vk_min(width, surface_capabilities.maxImageExtent.width));
                                   extent.height = vk_max(surface_capabilities.minImageExtent.height, vk_min(height, surface_capabilities.maxImageExtent.height));
                                   return extent;
                            }

                            VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &present_modes)
                            {
                                   for (VkPresentModeKHR present_mode : present_modes)
                                   {
                                          if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
                                          {
                                                 return present_mode;
                                          }
                                   }
                                   return VK_PRESENT_MODE_FIFO_KHR;
                            }

                            Swapchain create(SoftwareDevice software_device, const CreateInfo &create_info)
                            {
                                   Swapchain swapchain = new Swapchain_I();
                                   std::vector<VkSurfaceFormatKHR> surface_formats;
                                   {
                                          std::uint32_t count = 0;
                                          vk_assert(software_device->hardware_device->vk_get_physical_device_surface_formats_khr(
                                              software_device->hardware_device->vk_physical_device, create_info.win_link->vk_surface, &count, nullptr));
                                          surface_formats.resize(count);
                                          vk_assert(software_device->hardware_device->vk_get_physical_device_surface_formats_khr(
                                              software_device->hardware_device->vk_physical_device, create_info.win_link->vk_surface, &count, surface_formats.data()));
                                   }
                                   std::vector<VkPresentModeKHR> present_modes;
                                   {
                                          std::uint32_t count = 0;
                                          vk_assert(software_device->hardware_device->vk_get_physical_device_surface_present_modes_khr(
                                              software_device->hardware_device->vk_physical_device, create_info.win_link->vk_surface, &count, nullptr));
                                          present_modes.resize(count);
                                          vk_assert(software_device->hardware_device->vk_get_physical_device_surface_present_modes_khr(
                                              software_device->hardware_device->vk_physical_device, create_info.win_link->vk_surface, &count, present_modes.data()));
                                   }
                                   vk_assert(software_device->hardware_device->vk_get_physical_device_surface_capabilities_khr(
                                       software_device->hardware_device->vk_physical_device, create_info.win_link->vk_surface, &swapchain->vk_surface_capabilities));
                                   swapchain->vk_surface_format = choose_surface_format(surface_formats);
                                   swapchain->vk_present_mode = choose_present_mode(present_modes);
                                   swapchain->vk_extent = choose_extent(swapchain->vk_surface_capabilities, create_info.win_link->width, create_info.win_link->height);

                                   uint32_t image_count = vk_max(swapchain->vk_surface_capabilities.minImageCount, vk_min(swapchain->vk_surface_capabilities.maxImageCount, FRAME_COUNT_REQUIRED));

                                   VkSwapchainCreateInfoKHR swapchain_info =
                                       {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr, 0,
                                        create_info.win_link->vk_surface, image_count, swapchain->vk_surface_format.format, swapchain->vk_surface_format.colorSpace, swapchain->vk_extent, 1,
                                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr, swapchain->vk_surface_capabilities.currentTransform,
                                        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, swapchain->vk_present_mode, VK_TRUE, VK_NULL_HANDLE};
                                   if (!software_device->has_unique_queue)
                                   {
                                          swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                                          swapchain_info.queueFamilyIndexCount = (std::uint32_t)software_device->queue_family_indices.size();
                                          swapchain_info.pQueueFamilyIndices = software_device->queue_family_indices.data();
                                   }
                                   vk_assert(software_device->func.vk_create_swapchain_khr(software_device->vk_device, &swapchain_info, nullptr, &swapchain->vk_swapchain));

                                   std::vector<VkImage> images;
                                   {
                                          std::uint32_t count = 0;
                                          vk_assert(software_device->func.vk_get_swaphain_images_khr(software_device->vk_device, swapchain->vk_swapchain, &count, nullptr));
                                          images.resize(count);
                                          vk_assert(software_device->func.vk_get_swaphain_images_khr(software_device->vk_device, swapchain->vk_swapchain, &count, images.data()));
                                   }
                                   swapchain->textures.resize(images.size());
                                   for (size_t i = 0; i < images.size(); ++i)
                                   {
                                          swapchain->textures[i] = new Texture_I();
                                          VkComponentMapping components =
                                              {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
                                          VkImageSubresourceRange subresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
                                          VkImageViewCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, images[i], VK_IMAGE_VIEW_TYPE_2D, swapchain->vk_surface_format.format, components, subresource};
                                          swapchain->textures[i]->vk_image = images[i];
                                          swapchain->textures[i]->vma_allocation = VMA_NULL;
                                          swapchain->textures[i]->map = nullptr;
                                          vk_assert(software_device->func.vk_create_image_view(software_device->vk_device, &image_info, nullptr, &swapchain->textures[i]->vk_image_view));
                                   }
                                   return swapchain;
                            }

                            void destroy(SoftwareDevice software_device, Swapchain swapchain)
                            {
                                   for(size_t i = 0; i < swapchain->textures.size(); ++i)
                                   {
                                          software_device->func.vk_destroy_image_view(software_device->vk_device, swapchain->textures[i]->vk_image_view, nullptr);
                                          delete swapchain->textures[i];
                                   }
                                   software_device->func.vk_destroy_swapchain_khr(software_device->vk_device, swapchain->vk_swapchain, nullptr);
                                   delete swapchain;
                            }

                            std::uint32_t get_texture_count(Swapchain swapchain)
                            {
                                   return (std::uint32_t)swapchain->textures.size();
                            }

                            Texture get_texture(Swapchain swapchain, std::uint32_t index)
                            {
                                   return swapchain->textures[index];
                            }

                            void render_frame(SoftwareDevice software_device, Command *render_commands, std::uint32_t render_command_count)
                            {
                                   // TODO
                            }
                     }
              }
       }
}