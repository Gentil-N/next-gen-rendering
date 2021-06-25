#include "core.hpp"

#include <iostream>

namespace ngr
{
       namespace api
       {
              namespace driver
              {
                     VKAPI_ATTR VkBool32 VKAPI_CALL vk_dbg_callback(
                         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
                     {
                            if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
                            {
                                   std::cerr << "Vulkan Callback : " << pCallbackData->pMessage << std::endl;
                            }
                            return VK_FALSE;
                     }

                     bool check_instance_layers(Driver driver, const char **required_layers, std::uint32_t layer_count)
                     {
                            if (!layer_count)
                            {
                                   return false;
                            }
                            std::uint32_t count = 0;
                            vk_assert(driver->func.vk_enumerate_instance_layer_properties(&count, NULL));
                            std::vector<VkLayerProperties> properties(count);
                            vk_assert(driver->func.vk_enumerate_instance_layer_properties(&count, properties.data()));

                            for (std::uint32_t i = 0; i < layer_count; ++i)
                            {
                                   const char *required_layer = required_layers[i];
                                   bool found = false;
                                   for (size_t j = 0; j < count; ++j)
                                   {
                                          if (strcmp(properties[j].layerName, required_layer) == 0)
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

                     bool check_instance_extensions(Driver driver, const char **required_extensions, std::uint32_t extensions_count)
                     {
                            if (!extensions_count)
                            {
                                   return false;
                            }
                            std::uint32_t count = 0;
                            vk_assert(driver->func.vk_enumerate_instance_extension_properties(NULL, &count, NULL));
                            std::vector<VkExtensionProperties> properties(count);
                            vk_assert(driver->func.vk_enumerate_instance_extension_properties(NULL, &count, properties.data()));

                            for (std::uint32_t i = 0; i < extensions_count; ++i)
                            {
                                   const char *required_extension = required_extensions[i];
                                   bool found = false;
                                   for (size_t j = 0; j < count; ++j)
                                   {
                                          if (strcmp(properties[j].extensionName, required_extension) == 0)
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

                     /*BINDING*/

                     Driver load(const LoadInfo &load_info)
                     {
                            Driver driver = new Driver_I();
#ifdef NGR_LINUX_PLATFORM
                            driver->vk_dl_handler = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
                            if (!driver->vk_dl_handler)
                            {
                                   driver->vk_dl_handler = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
                            }
#endif //NGR_LINUX_PLATFORM
                            if (!driver->vk_dl_handler)
                            {
                                   return nullptr;
                            }
#ifdef NGR_LINUX_PLATFORM
                            driver->func.vk_get_instance_proc_addr = (PFN_vkGetInstanceProcAddr)dlsym(driver->vk_dl_handler, "vkGetInstanceProcAddr");
#endif //NGR_LINUX_PLATFORM
                            if (!driver->func.vk_get_instance_proc_addr)
                            {
                                   return nullptr;
                            }
                            PFN_vkGetInstanceProcAddr load = driver->func.vk_get_instance_proc_addr;
                            driver->func.vk_create_instance = (PFN_vkCreateInstance)load(NULL, "vkCreateInstance");
                            driver->func.vk_enumerate_instance_extension_properties = (PFN_vkEnumerateInstanceExtensionProperties)load(NULL, "vkEnumerateInstanceExtensionProperties");
                            driver->func.vk_enumerate_instance_layer_properties = (PFN_vkEnumerateInstanceLayerProperties)load(NULL, "vkEnumerateInstanceLayerProperties");
                            driver->func.vk_enumerate_instance_version = (PFN_vkEnumerateInstanceVersion)load(NULL, "vkEnumerateInstanceVersion");

                            std::vector<const char *> layers;
                            std::vector<const char *> extensions;
                            if (load_info.request_debug_mode)
                            {
                                   layers.push_back("VK_LAYER_KHRONOS_validation");
                                   extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                            }
                            if (load_info.request_win_link_support)
                            {
                                   extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef NGR_LINUX_PLATFORM
                                   extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif //NGR_LINUX_PLATFORM
                            }
                            if (!check_instance_layers(driver, layers.data(), (std::uint32_t)layers.size()) || !check_instance_extensions(driver, extensions.data(), (std::uint32_t)extensions.size()))
                            {
                                   return nullptr;
                            }
                            vk_assert(driver->func.vk_enumerate_instance_version(&driver->api_version));
                            VkApplicationInfo app_info =
                                {VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "", VK_MAKE_VERSION(0, 0, 0), "", VK_MAKE_VERSION(0, 0, 0), driver->api_version};
                            VkInstanceCreateInfo instance_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &app_info, (uint32_t)layers.size(), layers.data(), (uint32_t)extensions.size(), extensions.data()};
                            vk_assert(driver->func.vk_create_instance(&instance_info, nullptr, &driver->vk_instance));

                            driver->func.vk_get_device_proc_addr = (PFN_vkGetDeviceProcAddr)load(driver->vk_instance, "vkGetDeviceProcAddr");
                            driver->func.vk_destroy_instance = (PFN_vkDestroyInstance)load(driver->vk_instance, "vkDestroyInstance");
                            driver->func.vk_enumerate_physical_devices = (PFN_vkEnumeratePhysicalDevices)load(driver->vk_instance, "vkEnumeratePhysicalDevices");
                            driver->func.vk_get_physical_device_features = (PFN_vkGetPhysicalDeviceFeatures)load(driver->vk_instance, "vkGetPhysicalDeviceFeatures");
                            driver->func.vk_get_physical_device_format_properties = (PFN_vkGetPhysicalDeviceFormatProperties)load(driver->vk_instance, "vkGetPhysicalDeviceFormatProperties");
                            driver->func.vk_get_physical_device_image_format_properties = (PFN_vkGetPhysicalDeviceImageFormatProperties)load(driver->vk_instance, "vkGetPhysicalDeviceImageFormatProperties");
                            driver->func.vk_get_physical_device_properties = (PFN_vkGetPhysicalDeviceProperties)load(driver->vk_instance, "vkGetPhysicalDeviceProperties");
                            driver->func.vk_get_physical_device_queue_family_properties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)load(driver->vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties");
                            driver->func.vk_get_physical_device_memory_properties = (PFN_vkGetPhysicalDeviceMemoryProperties)load(driver->vk_instance, "vkGetPhysicalDeviceMemoryProperties");
                            driver->func.vk_create_device = (PFN_vkCreateDevice)load(driver->vk_instance, "vkCreateDevice");
                            driver->func.vk_enumerate_instance_extension_properties = (PFN_vkEnumerateInstanceExtensionProperties)load(driver->vk_instance, "vkEnumerateInstanceExtensionProperties");
                            driver->func.vk_enumerate_device_extension_properties = (PFN_vkEnumerateDeviceExtensionProperties)load(driver->vk_instance, "vkEnumerateDeviceExtensionProperties");
                            driver->func.vk_enumerate_device_layer_properties = (PFN_vkEnumerateDeviceLayerProperties)load(driver->vk_instance, "vkEnumerateDeviceLayerProperties");
                            driver->func.vk_get_physical_device_sparse_image_format_properties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)load(driver->vk_instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
                            driver->func.vk_destroy_surface_khr = (PFN_vkDestroySurfaceKHR)load(driver->vk_instance, "vkDestroySurfaceKHR");
                            driver->func.vk_get_physical_device_surface_support_khr = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)load(driver->vk_instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
                            driver->func.vk_get_physical_device_surface_capabilities_khr = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)load(driver->vk_instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
                            driver->func.vk_get_physical_device_surface_formats_khr = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)load(driver->vk_instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
                            driver->func.vk_get_physical_device_surface_present_modes_khr = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)load(driver->vk_instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#ifdef NGR_LINUX_PLATFORM
                            driver->func.vk_create_surface_khr = (PFN_vkCreateXlibSurfaceKHR)load(driver->vk_instance, "vkCreateXlibSurfaceKHR");
#endif //NGR_LINUX_PLATFORM
                            driver->func.vk_create_debug_utils_messenger_ext = nullptr;
                            driver->func.vk_destroy_debug_utils_messenger_ext = nullptr;

                            if (load_info.request_debug_mode)
                            {
                                   driver->func.vk_create_debug_utils_messenger_ext = (PFN_vkCreateDebugUtilsMessengerEXT)load(driver->vk_instance, "vkCreateDebugUtilsMessengerEXT");
                                   driver->func.vk_destroy_debug_utils_messenger_ext = (PFN_vkDestroyDebugUtilsMessengerEXT)load(driver->vk_instance, "vkDestroyDebugUtilsMessengerEXT");
                                   VkDebugUtilsMessengerCreateInfoEXT debug_info =
                                       {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, nullptr, 0,
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                        vk_dbg_callback, nullptr};
                                   vk_assert(driver->func.vk_create_debug_utils_messenger_ext(driver->vk_instance, &debug_info, nullptr, &driver->vk_debug_utils_messenger));
                            }
                            {
                                   std::uint32_t count = 0;
                                   driver->func.vk_enumerate_physical_devices(driver->vk_instance, &count, nullptr);
                                   std::vector<VkPhysicalDevice> physical_devices(count);
                                   driver->func.vk_enumerate_physical_devices(driver->vk_instance, &count, physical_devices.data());
                                   driver->hardware_devices.resize(count);
                                   for (uint32_t i = 0; i < count; ++i)
                                   {
                                          driver->hardware_devices[i] = new HardwareDevice_I();
                                          driver->hardware_devices[i]->vk_physical_device = physical_devices[i];
                                          driver->func.vk_get_physical_device_properties(physical_devices[i], &driver->hardware_devices[i]->vk_properties);
                                          driver->func.vk_get_physical_device_features(physical_devices[i], &driver->hardware_devices[i]->vk_features);
                                          std::uint32_t family_count = 0;
                                          driver->func.vk_get_physical_device_queue_family_properties(physical_devices[i], &family_count, nullptr);
                                          driver->hardware_devices[i]->queue_families.resize(family_count);
                                          driver->func.vk_get_physical_device_queue_family_properties(physical_devices[i], &family_count, driver->hardware_devices[i]->queue_families.data());
                                          std::uint32_t ext_count = 0;
                                          vk_assert(driver->func.vk_enumerate_device_extension_properties(physical_devices[i], NULL, &ext_count, NULL));
                                          driver->hardware_devices[i]->extensions.resize(ext_count);
                                          vk_assert(driver->func.vk_enumerate_device_extension_properties(physical_devices[i], NULL, &ext_count, driver->hardware_devices[i]->extensions.data()));
                                          driver->hardware_devices[i]->vk_get_physical_device_surface_support_khr = driver->func.vk_get_physical_device_surface_support_khr;
                                          driver->hardware_devices[i]->vk_get_physical_device_surface_capabilities_khr = driver->func.vk_get_physical_device_surface_capabilities_khr;
                                          driver->hardware_devices[i]->vk_get_physical_device_surface_formats_khr = driver->func.vk_get_physical_device_surface_formats_khr;
                                          driver->hardware_devices[i]->vk_get_physical_device_surface_present_modes_khr = driver->func.vk_get_physical_device_surface_present_modes_khr;
                                   }
                            }
                            return driver;
                     }

                     void unload(Driver driver)
                     {
                            for (size_t i = 0; i < driver->hardware_devices.size(); ++i)
                            {
                                   delete driver->hardware_devices[i];
                            }
                            if (driver->vk_debug_utils_messenger != VK_NULL_HANDLE)
                            {
                                   driver->func.vk_destroy_debug_utils_messenger_ext(driver->vk_instance, driver->vk_debug_utils_messenger, nullptr);
                            }
                            driver->func.vk_destroy_instance(driver->vk_instance, nullptr);
#ifdef NGR_LINUX_PLATFORM
                            dlclose(driver->vk_dl_handler);
#endif //NGR_LINUX_PLATFORM
                            delete driver;
                     }
              }
       }
}