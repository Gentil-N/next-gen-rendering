#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace hardware_device
              {
                     bool check_physical_device_extensions(HardwareDevice hd, const char **required_extensions, uint32_t extension_count)
                     {
                            for (uint32_t i = 0; i < extension_count; ++i)
                            {
                                   const char *required_extension = required_extensions[i];
                                   bool found = false;
                                   for (uint32_t j = 0; j < hd->extensions.size(); ++j)
                                   {
                                          if (strcmp(hd->extensions[j].extensionName, required_extension) == 0)
                                          {
                                                 found = true;
                                                 break;
                                          }
                                   }
                                   if (!found)
                                   {
                                          return false;
                                   }
                            }
                            return true;
                     }

                     bool check_for_queue(HardwareDevice hardware_device, VkQueueFlagBits queue_type)
                     {
                            for (size_t i = 0; i < hardware_device->queue_families.size(); ++i)
                            {
                                   VkQueueFamilyProperties &queue_props = hardware_device->queue_families[i];
                                   if((queue_props.queueFlags & queue_type) == queue_type && queue_props.queueCount > 0)
                                   {
                                          return true;
                                   }
                            }
                            return false;
                     }

                     /*BINDING*/

                     std::uint32_t count(Driver driver)
                     {
                            return (std::uint32_t)driver->hardware_devices.size();
                     }

                     HardwareDevice get(Driver driver, std::uint32_t index)
                     {
                            return driver->hardware_devices[index];
                     }

                     bool is_swapchain_extension_supported(HardwareDevice hardware_device)
                     {
                            std::vector<const char *> swapchain_extension = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
                            return check_physical_device_extensions(hardware_device, swapchain_extension.data(), swapchain_extension.size());
                     }

                     bool is_win_link_extension_supported(HardwareDevice hardware_device, ext::WinLink win_link)
                     {
                            for (size_t i = 0; i < hardware_device->queue_families.size(); ++i)
                            {
                                   VkBool32 supported = VK_FALSE;
                                   vk_assert(hardware_device->vk_get_physical_device_surface_support_khr(hardware_device->vk_physical_device, i, win_link->vk_surface, &supported));
                                   if(supported == VK_TRUE)
                                   {
                                          return true;
                                   }
                            }
                            return false;
                     }

                     bool has_graphics_queue(HardwareDevice hardware_device)
                     {
                            return check_for_queue(hardware_device, VK_QUEUE_GRAPHICS_BIT);
                     }

                     bool has_compute_queue(HardwareDevice hardware_device)
                     {
                            return check_for_queue(hardware_device, VK_QUEUE_COMPUTE_BIT);
                     }

                     bool has_transfer_queue(HardwareDevice hardware_device)
                     {
                            return check_for_queue(hardware_device, VK_QUEUE_TRANSFER_BIT);
                     }

                     bool is_dedicated(HardwareDevice hardware_device)
                     {
                            return hardware_device->vk_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
                     }

                     void get_features(Features &features)
                     {
                            // TODO
                     }
              }
       }
}