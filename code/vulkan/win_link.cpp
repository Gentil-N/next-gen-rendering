#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace ext
              {
                     namespace win_link
                     {
                            WinLink create(Driver driver, const CreateInfo &create_info)
                            {
                                   WinLink win_link = new WinLink_I();
#ifdef NGR_LINUX_PLATFORM
                                   VkXlibSurfaceCreateInfoKHR surface_info = {VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, nullptr, 0, create_info.x11_display, create_info.x11_window};
#endif //NGR_LINUX_PLATFORM
                                   driver->func.vk_create_surface_khr(driver->vk_instance, &surface_info, nullptr, &win_link->vk_surface);
                                   win_link->width = create_info.width;
                                   win_link->height = create_info.height;
                                   return win_link;
                            }

                            void destroy(Driver driver, WinLink win_link)
                            {
                                   driver->func.vk_destroy_surface_khr(driver->vk_instance, win_link->vk_surface, nullptr);
                                   delete win_link;
                            }
                     }
              }
       }
}