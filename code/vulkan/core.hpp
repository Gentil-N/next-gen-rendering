#pragma once
#ifndef __CORE_HPP__
#define __CORE_HPP__

#include "../ngr_api.hpp"

#ifdef NGR_LINUX_PLATFORM
#include <dlfcn.h>
#define VK_USE_PLATFORM_XLIB_KHR
#endif //NGR_LINUX_PLATFORM

#include <assert.h>
#include <string.h>

#define VK_NO_PROTOTYPES
#include "../../external/Vulkan-Headers/include/vulkan/vulkan.h"
#include "../../external/Vulkan-Headers/include/vulkan/vk_platform.h"

#ifndef NDEBUG
#define vk_assert(expr) assert(expr == VK_SUCCESS)
#else
#define vk_assert(expr) expr
#endif //NDEBUG
#define vk_max(a, b) ((a) > (b) ? (a) : (b))
#define vk_min(a, b) ((a) < (b) ? (a) : (b))

namespace ngr
{
       namespace api
       {
              struct Driver_I
              {
                     void *vk_dl_handler;
                     struct
                     {
                            PFN_vkGetInstanceProcAddr vk_get_instance_proc_addr;
                            PFN_vkCreateInstance vk_create_instance;
                            PFN_vkEnumerateInstanceExtensionProperties vk_enumerate_instance_extension_properties;
                            PFN_vkEnumerateInstanceLayerProperties vk_enumerate_instance_layer_properties;
#ifdef VK_VERSION_1_1
                            PFN_vkEnumerateInstanceVersion vk_enumerate_instance_version;
#endif //VK_VERSION_1_1
                            PFN_vkGetDeviceProcAddr vk_get_device_proc_addr;
                            PFN_vkDestroyInstance vk_destroy_instance;
                            PFN_vkEnumeratePhysicalDevices vk_enumerate_physical_devices;
                            PFN_vkGetPhysicalDeviceFeatures vk_get_physical_device_features;
                            PFN_vkGetPhysicalDeviceFormatProperties vk_get_physical_device_format_properties;
                            PFN_vkGetPhysicalDeviceImageFormatProperties vk_get_physical_device_image_format_properties;
                            PFN_vkGetPhysicalDeviceProperties vk_get_physical_device_properties;
                            PFN_vkGetPhysicalDeviceQueueFamilyProperties vk_get_physical_device_queue_family_properties;
                            PFN_vkGetPhysicalDeviceMemoryProperties vk_get_physical_device_memory_properties;
                            PFN_vkCreateDevice vk_create_device;
                            PFN_vkEnumerateDeviceExtensionProperties vk_enumerate_device_extension_properties;
                            PFN_vkEnumerateDeviceLayerProperties vk_enumerate_device_layer_properties;
                            PFN_vkGetPhysicalDeviceSparseImageFormatProperties vk_get_physical_device_sparse_image_format_properties;
                            PFN_vkDestroySurfaceKHR vk_destroy_surface_khr;
                            PFN_vkGetPhysicalDeviceSurfaceSupportKHR vk_get_physical_device_surface_support_khr;
                            PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vk_get_physical_device_surface_capabilities_khr;
                            PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vk_get_physical_device_surface_formats_khr;
                            PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vk_get_physical_device_surface_present_modes_khr;
#ifdef NGR_LINUX_PLATFORM
                            PFN_vkCreateXlibSurfaceKHR vk_create_surface_khr;
#endif //NGR_LINUX_PLATFORM
                            PFN_vkCreateDebugUtilsMessengerEXT vk_create_debug_utils_messenger_ext;
                            PFN_vkDestroyDebugUtilsMessengerEXT vk_destroy_debug_utils_messenger_ext;
                     } func;
                     VkInstance vk_instance;
                     std::uint32_t api_version;
                     VkDebugUtilsMessengerEXT vk_debug_utils_messenger = VK_NULL_HANDLE;
                     std::vector<HardwareDevice> hardware_devices;
              };

              struct HardwareDevice_I
              {
                     VkPhysicalDevice vk_physical_device;
                     VkPhysicalDeviceProperties vk_properties;
                     VkPhysicalDeviceFeatures vk_features;
                     PFN_vkGetPhysicalDeviceSurfaceSupportKHR vk_get_physical_device_surface_support_khr;
                     PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vk_get_physical_device_surface_capabilities_khr;
                     PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vk_get_physical_device_surface_formats_khr;
                     PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vk_get_physical_device_surface_present_modes_khr;
                     std::vector<VkQueueFamilyProperties> queue_families;
                     std::vector<VkExtensionProperties> extensions;
              };

              struct SoftwareDevice_I
              {
                     HardwareDevice hardware_device;
                     VkDevice vk_device;
                     struct
                     {
                            PFN_vkDestroyDevice vk_destroy_device;
                            PFN_vkGetDeviceQueue vk_get_device_queue;
                            PFN_vkQueueSubmit vk_queue_submit;
                            PFN_vkQueueWaitIdle vk_queue_wait_idle;
                            PFN_vkDeviceWaitIdle vk_device_wait_idle;
                            PFN_vkAllocateMemory vk_allocate_memory;
                            PFN_vkFreeMemory vk_free_memory;
                            PFN_vkMapMemory vk_map_memory;
                            PFN_vkUnmapMemory vk_unmap_memory;
                            PFN_vkFlushMappedMemoryRanges vk_flush_mapped_memory_ranges;
                            PFN_vkInvalidateMappedMemoryRanges vk_invalidate_mapped_memory_ranges;
                            PFN_vkGetDeviceMemoryCommitment vk_get_device_memory_commitment;
                            PFN_vkBindBufferMemory vk_bind_buffer_memory;
                            PFN_vkBindImageMemory vk_bind_image_memory;
                            PFN_vkGetBufferMemoryRequirements vk_get_buffer_memory_requirements;
                            PFN_vkGetImageMemoryRequirements vk_get_image_memory_requirements;
                            PFN_vkGetImageSparseMemoryRequirements vk_get_image_sparse_memory_requirements;
                            PFN_vkQueueBindSparse vk_queue_bind_sparse;
                            PFN_vkCreateFence vk_create_fence;
                            PFN_vkDestroyFence vk_destroy_fence;
                            PFN_vkResetFences vk_reset_fences;
                            PFN_vkGetFenceStatus vk_get_fence_status;
                            PFN_vkWaitForFences vk_wait_for_fences;
                            PFN_vkCreateSemaphore vk_create_semaphore;
                            PFN_vkDestroySemaphore vk_destroy_semaphore;
                            PFN_vkCreateEvent vk_create_event;
                            PFN_vkDestroyEvent vk_destroy_event;
                            PFN_vkGetEventStatus vk_get_event_status;
                            PFN_vkSetEvent vk_set_event;
                            PFN_vkResetEvent vk_reset_event;
                            PFN_vkCreateQueryPool vk_create_query_pool;
                            PFN_vkDestroyQueryPool vk_destroy_query_pool;
                            PFN_vkGetQueryPoolResults vk_get_query_pool_results;
                            PFN_vkCreateBuffer vk_create_buffer;
                            PFN_vkDestroyBuffer vk_destroy_buffer;
                            PFN_vkCreateBufferView vk_create_buffer_view;
                            PFN_vkDestroyBufferView vk_destroy_buffer_view;
                            PFN_vkCreateImage vk_create_image;
                            PFN_vkDestroyImage vk_destroy_image;
                            PFN_vkGetImageSubresourceLayout vk_get_image_subresource_layout;
                            PFN_vkCreateImageView vk_create_image_view;
                            PFN_vkDestroyImageView vk_destroy_image_view;
                            PFN_vkCreateShaderModule vk_create_shader_module;
                            PFN_vkDestroyShaderModule vk_destroy_shader_module;
                            PFN_vkCreatePipelineCache vk_create_pipeline_cache;
                            PFN_vkDestroyPipelineCache vk_destroy_pipeline_cache;
                            PFN_vkGetPipelineCacheData vk_get_pipeline_cache_data;
                            PFN_vkMergePipelineCaches vk_merge_pipeline_caches;
                            PFN_vkCreateGraphicsPipelines vk_create_graphics_pipelines;
                            PFN_vkCreateComputePipelines vk_create_compute_pipelines;
                            PFN_vkDestroyPipeline vk_destroy_pipeline;
                            PFN_vkCreatePipelineLayout vk_create_pipeline_layout;
                            PFN_vkDestroyPipelineLayout vk_destroy_pipeline_layout;
                            PFN_vkCreateSampler vk_create_sampler;
                            PFN_vkDestroySampler vk_destroy_sampler;
                            PFN_vkCreateDescriptorSetLayout vk_create_descriptor_set_layout;
                            PFN_vkDestroyDescriptorSetLayout vk_destroy_descriptor_set_layout;
                            PFN_vkCreateDescriptorPool vk_create_descriptor_pool;
                            PFN_vkDestroyDescriptorPool vk_destroy_descriptor_pool;
                            PFN_vkResetDescriptorPool vk_reset_descriptor_pool;
                            PFN_vkAllocateDescriptorSets vk_allocate_descriptor_sets;
                            PFN_vkFreeDescriptorSets vk_free_descriptor_sets;
                            PFN_vkUpdateDescriptorSets vk_update_descriptor_sets;
                            PFN_vkCreateFramebuffer vk_create_framebuffer;
                            PFN_vkDestroyFramebuffer vk_destroy_framebuffer;
                            PFN_vkCreateRenderPass vk_create_render_pass;
                            PFN_vkDestroyRenderPass vk_destroy_render_pass;
                            PFN_vkGetRenderAreaGranularity vk_get_render_area_granularity;
                            PFN_vkCreateCommandPool vk_create_command_pool;
                            PFN_vkDestroyCommandPool vk_destroy_command_pool;
                            PFN_vkResetCommandPool vk_reset_command_pool;
                            PFN_vkAllocateCommandBuffers vk_allocate_command_buffers;
                            PFN_vkFreeCommandBuffers vk_free_command_buffers;
                            PFN_vkBeginCommandBuffer vk_begin_command_buffer;
                            PFN_vkEndCommandBuffer vk_end_command_buffer;
                            PFN_vkResetCommandBuffer vk_reset_command_buffer;
                            PFN_vkCmdBindPipeline vk_cmd_bind_pipeline;
                            PFN_vkCmdSetViewport vk_cmd_set_viewport;
                            PFN_vkCmdSetScissor vk_cmd_set_scissor;
                            PFN_vkCmdSetLineWidth vk_cmd_set_line_width;
                            PFN_vkCmdSetDepthBias vk_cmd_set_depth_bias;
                            PFN_vkCmdSetBlendConstants vk_cmd_set_blend_constants;
                            PFN_vkCmdSetDepthBounds vk_cmd_set_depth_bounds;
                            PFN_vkCmdSetStencilCompareMask vk_cmd_set_stencil_compare_mask;
                            PFN_vkCmdSetStencilWriteMask vk_cmd_set_stencil_write_mask;
                            PFN_vkCmdSetStencilReference vk_cmd_set_stencil_reference;
                            PFN_vkCmdBindDescriptorSets vk_cmd_bind_descriptor_sets;
                            PFN_vkCmdBindIndexBuffer vk_cmd_bind_index_buffer;
                            PFN_vkCmdBindVertexBuffers vk_cmd_bind_vertex_buffers;
                            PFN_vkCmdDraw vk_cmd_draw;
                            PFN_vkCmdDrawIndexed vk_cmd_draw_indexed;
                            PFN_vkCmdDrawIndirect vk_cmd_draw_indirect;
                            PFN_vkCmdDrawIndexedIndirect vk_cmd_draw_indexed_indirect;
                            PFN_vkCmdDispatch vk_cmd_dispatch;
                            PFN_vkCmdDispatchIndirect vk_cmd_dispatch_indirect;
                            PFN_vkCmdCopyBuffer vk_cmd_copy_buffer;
                            PFN_vkCmdCopyImage vk_cmd_copy_image;
                            PFN_vkCmdBlitImage vk_cmd_blit_image;
                            PFN_vkCmdCopyBufferToImage vk_cmd_copy_buffer_to_image;
                            PFN_vkCmdCopyImageToBuffer vk_cmd_copy_image_to_buffer;
                            PFN_vkCmdUpdateBuffer vk_cmd_update_buffer;
                            PFN_vkCmdFillBuffer vk_cmd_fill_buffer;
                            PFN_vkCmdClearColorImage vk_cmd_clear_color_image;
                            PFN_vkCmdClearDepthStencilImage vk_cmd_clear_depth_stencil_image;
                            PFN_vkCmdClearAttachments vk_cmd_clear_attachments;
                            PFN_vkCmdResolveImage vk_cmd_resolve_image;
                            PFN_vkCmdSetEvent vk_cmd_set_event;
                            PFN_vkCmdResetEvent vk_cmd_reset_event;
                            PFN_vkCmdWaitEvents vk_cmd_wait_events;
                            PFN_vkCmdPipelineBarrier vk_cmd_pipeline_barrier;
                            PFN_vkCmdBeginQuery vk_cmd_begin_query;
                            PFN_vkCmdEndQuery vk_cmd_end_query;
                            PFN_vkCmdResetQueryPool vk_cmd_reset_query_pool;
                            PFN_vkCmdWriteTimestamp vk_cmd_write_timestamp;
                            PFN_vkCmdCopyQueryPoolResults vk_cmd_copy_query_pool_results;
                            PFN_vkCmdPushConstants vk_cmd_push_constants;
                            PFN_vkCmdBeginRenderPass vk_cmd_begin_render_pass;
                            PFN_vkCmdNextSubpass vk_cmd_next_subpass;
                            PFN_vkCmdEndRenderPass vk_cmd_end_render_pass;
                            PFN_vkCmdExecuteCommands vk_cmd_execute_commands;
                            PFN_vkCreateSwapchainKHR vk_create_swapchain_khr;
                            PFN_vkDestroySwapchainKHR vk_destroy_swapchain_khr;
                            PFN_vkGetSwapchainImagesKHR vk_get_swaphain_images_khr;
                            PFN_vkAcquireNextImageKHR vk_acquire_next_image_khr;
                            PFN_vkQueuePresentKHR vk_queue_present_khr;
                     } func;
                     bool has_unique_queue;
                     std::vector<std::uint32_t> queue_family_indices;
                     Queue graphics_queue, compute_queue, transfer_queue, present_queue;
                     std::vector<std::function<void(SoftwareDevice)>> operations;
              };

              struct Queue_I
              {
                     VkQueueFlags types;
                     VkQueue vk_queue;
                     VkCommandPool vk_command_pool;
              };
              struct Command_I
              {
              };
              struct Buffer_I
              {
              };
              struct Texture_I
              {
                     VkImage vk_image;
                     VkImageView vk_image_view;
              };
              struct Sampler_I
              {
              };
              struct RenderPass_I
              {
              };
              struct Framebuffer_I
              {
              };
              struct Shader_I
              {
              };
              struct PipLink_I
              {
              };
              struct UniformSet_I
              {
              };
              struct Pipeline_I
              {
              };

              namespace ext
              {
                     struct WinLink_I
                     {
                            std::uint32_t width, height;
                            VkSurfaceKHR vk_surface;
                     };

                     struct Swapchain_I
                     {
                            VkSwapchainKHR vk_swapchain;
                            VkSurfaceCapabilitiesKHR vk_surface_capabilities;
                            VkSurfaceFormatKHR vk_surface_format;
                            VkPresentModeKHR vk_present_mode;
                            VkExtent2D vk_extent;
                            std::vector<Texture> textures;
                     };
              }
       }
}

#endif //__CORE_HPP__