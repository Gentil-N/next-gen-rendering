#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace software_device
              {
                     struct QueueInfo
                     {
                            VkQueueFlags types;
                            /*filled by 'get_queues'*/
                            std::uint32_t queue_family_index;
                            std::uint32_t queue_index; // into family
                            bool have_unique_granularity;
                     };

                     bool is_matching_queue(const QueueInfo &required_queue, const QueueInfo &available_queue)
                     {
                            if ((required_queue.types & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
                            {
                                   if (available_queue.have_unique_granularity && required_queue.types == available_queue.types)
                                   {
                                          return true;
                                   }
                            }
                            else if (required_queue.types == available_queue.types)
                            {
                                   return true;
                            }
                            return false;
                     }

                     VkDeviceQueueCreateInfo &get_queue_create_info(std::vector<VkDeviceQueueCreateInfo> &queue_create_infos, std::uint32_t queue_family_index)
                     {
                            for (VkDeviceQueueCreateInfo &queue_create_info : queue_create_infos)
                            {
                                   if (queue_create_info.queueFamilyIndex == queue_family_index)
                                   {
                                          return queue_create_info;
                                   }
                            }
                            queue_create_infos.push_back(VkDeviceQueueCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, queue_family_index, 0, nullptr});
                            return queue_create_infos.back();
                     }

                     bool get_queues(HardwareDevice hardware_device, std::vector<QueueInfo> &queue_infos, bool request_present_queue, ext::WinLink win_link, std::vector<VkDeviceQueueCreateInfo> &queue_create_infos)
                     {
                            std::vector<QueueInfo> available_queues, reserved_queues;
                            for (size_t i = 0; i < hardware_device->queue_families.size(); ++i)
                            {
                                   VkQueueFamilyProperties &queue_family = hardware_device->queue_families[i];
                                   for (std::uint32_t count = queue_family.queueCount; count != 0; --count)
                                   {
                                          bool have_unique_granularity = queue_family.minImageTransferGranularity.width == 1 && queue_family.minImageTransferGranularity.height == 1 && queue_family.minImageTransferGranularity.depth == 1;
                                          available_queues.push_back(QueueInfo{queue_family.queueFlags, (std::uint32_t)i, count - 1, have_unique_granularity});
                                   }
                            }
                            for (QueueInfo &required_queue : queue_infos)
                            {
                                   bool found = false;
                                   std::uint32_t found_index = 0;
                                   for (size_t i = 0; i < available_queues.size(); ++i)
                                   {
                                          if (is_matching_queue(required_queue, available_queues[i]))
                                          {
                                                 found_index = i;
                                                 break;
                                          }
                                   }
                                   if (found)
                                   {
                                          required_queue.queue_family_index = available_queues[found_index].queue_family_index;
                                          required_queue.queue_index = available_queues[found_index].queue_index;
                                          reserved_queues.push_back(available_queues[found_index]);
                                          available_queues.erase(available_queues.begin() + found_index);
                                          continue;
                                   }
                                   for (size_t i = 0; i < reserved_queues.size(); ++i)
                                   {
                                          if (is_matching_queue(required_queue, reserved_queues[i]))
                                          {
                                                 found_index = i;
                                                 break;
                                          }
                                   }
                                   if (!found)
                                   {
                                          return false;
                                   }
                                   required_queue.queue_family_index = reserved_queues[found_index].queue_family_index;
                                   required_queue.queue_index = reserved_queues[found_index].queue_index;
                            }

                            if (request_present_queue) // push a fake queue in 'queue_infos' to show the 'present queue'
                            {
                                   VkBool32 supported = VK_FALSE;
                                   for (size_t i = 0; i < available_queues.size(); ++i)
                                   {
                                          vk_assert(hardware_device->vk_get_physical_device_surface_support_khr(hardware_device->vk_physical_device, available_queues[i].queue_family_index, win_link->vk_surface, &supported));
                                          if (supported == VK_TRUE)
                                          {
                                                 queue_infos.push_back(QueueInfo{0, available_queues[i].queue_family_index, available_queues[i].queue_index, false});
                                                 break;
                                          }
                                   }
                                   if (!supported)
                                   {
                                          for (size_t i = 0; i < reserved_queues.size(); ++i)
                                          {
                                                 vk_assert(hardware_device->vk_get_physical_device_surface_support_khr(hardware_device->vk_physical_device, reserved_queues[i].queue_family_index, win_link->vk_surface, &supported));
                                                 if (supported == VK_TRUE)
                                                 {
                                                        queue_infos.push_back(QueueInfo{0, reserved_queues[i].queue_family_index, reserved_queues[i].queue_index, false});
                                                        break;
                                                 }
                                          }
                                   }
                                   if (!supported)
                                   {
                                          return false;
                                   }
                            }
                            for (QueueInfo &queue_info : queue_infos)
                            {
                                   VkDeviceQueueCreateInfo &queue_create_info = get_queue_create_info(queue_create_infos, queue_info.queue_family_index);
                                   ++queue_create_info.queueCount;
                            }
                            return true;
                     }

                     /*BINDING*/

                     SoftwareDevice create(Driver driver, const CreateInfo &create_info)
                     {
                            SoftwareDevice software_device = new SoftwareDevice_I();
                            software_device->hardware_device = create_info.hardware_device;
                            std::vector<const char *> extensions;
                            if (create_info.request_swapchain_extension)
                            {
                                   extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                            }
                            std::vector<QueueInfo> queue_infos;
                            if (create_info.request_graphics_queue)
                            {
                                   software_device->graphics_queue = new Queue_I();
                                   queue_infos.push_back(QueueInfo{VK_QUEUE_GRAPHICS_BIT});
                            }
                            else
                            {
                                   software_device->graphics_queue = nullptr;
                            }
                            if (create_info.request_compute_queue)
                            {
                                   software_device->compute_queue = new Queue_I();
                                   queue_infos.push_back(QueueInfo{VK_QUEUE_COMPUTE_BIT});
                            }
                            else
                            {
                                   software_device->compute_queue = nullptr;
                            }
                            if (create_info.request_transfer_queue)
                            {
                                   software_device->transfer_queue = new Queue_I();
                                   queue_infos.push_back(QueueInfo{VK_QUEUE_TRANSFER_BIT});
                            }
                            else
                            {
                                   software_device->transfer_queue = nullptr;
                            }
                            std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
                            if (!get_queues(create_info.hardware_device, queue_infos, create_info.request_present_queue, create_info.win_link, queue_create_infos))
                            {
                                   return nullptr;
                            }
                            std::vector<std::vector<float>> priorities(queue_create_infos.size());
                            software_device->queue_family_indices.resize(queue_create_infos.size());
                            software_device->has_unique_queue = (queue_create_infos.size() == 1);
                            for (size_t i = 0; i < priorities.size(); ++i)
                            {
                                   priorities[i].resize(queue_create_infos[i].queueCount);
                                   std::fill(priorities[i].begin(), priorities[i].end(), 1.0f);
                                   queue_create_infos[i].pQueuePriorities = priorities[i].data();
                                   software_device->queue_family_indices[i] = queue_create_infos[i].queueFamilyIndex;
                            }
                            VkDeviceCreateInfo device_info =
                                {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0, (std::uint32_t)queue_create_infos.size(), queue_create_infos.data(), 0, nullptr,
                                 (std::uint32_t)extensions.size(), extensions.data(), &create_info.hardware_device->vk_features};
                            vk_assert(driver->func.vk_create_device(create_info.hardware_device->vk_physical_device, &device_info, nullptr, &software_device->vk_device));

                            PFN_vkGetDeviceProcAddr load = driver->func.vk_get_device_proc_addr;
                            software_device->func.vk_destroy_device = (PFN_vkDestroyDevice)load(software_device->vk_device, "vkDestroyDevice");
                            software_device->func.vk_get_device_queue = (PFN_vkGetDeviceQueue)load(software_device->vk_device, "vkGetDeviceQueue");
                            software_device->func.vk_queue_submit = (PFN_vkQueueSubmit)load(software_device->vk_device, "vkQueueSubmit");
                            software_device->func.vk_queue_wait_idle = (PFN_vkQueueWaitIdle)load(software_device->vk_device, "vkQueueWaitIdle");
                            software_device->func.vk_device_wait_idle = (PFN_vkDeviceWaitIdle)load(software_device->vk_device, "vkDeviceWaitIdle");
                            software_device->func.vk_allocate_memory = (PFN_vkAllocateMemory)load(software_device->vk_device, "vkAllocateMemory");
                            software_device->func.vk_free_memory = (PFN_vkFreeMemory)load(software_device->vk_device, "vkFreeMemory");
                            software_device->func.vk_map_memory = (PFN_vkMapMemory)load(software_device->vk_device, "vkMapMemory");
                            software_device->func.vk_unmap_memory = (PFN_vkUnmapMemory)load(software_device->vk_device, "vkUnmapMemory");
                            software_device->func.vk_flush_mapped_memory_ranges = (PFN_vkFlushMappedMemoryRanges)load(software_device->vk_device, "vkFlushMappedMemoryRanges");
                            software_device->func.vk_invalidate_mapped_memory_ranges = (PFN_vkInvalidateMappedMemoryRanges)load(software_device->vk_device, "vkInvalidateMappedMemoryRanges");
                            software_device->func.vk_get_device_memory_commitment = (PFN_vkGetDeviceMemoryCommitment)load(software_device->vk_device, "vkGetDeviceMemoryCommitment");
                            software_device->func.vk_bind_buffer_memory = (PFN_vkBindBufferMemory)load(software_device->vk_device, "vkBindBufferMemory");
                            software_device->func.vk_bind_image_memory = (PFN_vkBindImageMemory)load(software_device->vk_device, "vkBindImageMemory");
                            software_device->func.vk_get_buffer_memory_requirements = (PFN_vkGetBufferMemoryRequirements)load(software_device->vk_device, "vkGetBufferMemoryRequirements");
                            software_device->func.vk_get_image_memory_requirements = (PFN_vkGetImageMemoryRequirements)load(software_device->vk_device, "vkGetImageMemoryRequirements");
                            software_device->func.vk_get_image_sparse_memory_requirements = (PFN_vkGetImageSparseMemoryRequirements)load(software_device->vk_device, "vkGetImageSparseMemoryRequirements");
                            software_device->func.vk_queue_bind_sparse = (PFN_vkQueueBindSparse)load(software_device->vk_device, "vkQueueBindSparse");
                            software_device->func.vk_create_fence = (PFN_vkCreateFence)load(software_device->vk_device, "vkCreateFence");
                            software_device->func.vk_destroy_fence = (PFN_vkDestroyFence)load(software_device->vk_device, "vkDestroyFence");
                            software_device->func.vk_reset_fences = (PFN_vkResetFences)load(software_device->vk_device, "vkResetFences");
                            software_device->func.vk_get_fence_status = (PFN_vkGetFenceStatus)load(software_device->vk_device, "vkGetFenceStatus");
                            software_device->func.vk_wait_for_fences = (PFN_vkWaitForFences)load(software_device->vk_device, "vkWaitForFences");
                            software_device->func.vk_create_semaphore = (PFN_vkCreateSemaphore)load(software_device->vk_device, "vkCreateSemaphore");
                            software_device->func.vk_destroy_semaphore = (PFN_vkDestroySemaphore)load(software_device->vk_device, "vkDestroySemaphore");
                            software_device->func.vk_create_event = (PFN_vkCreateEvent)load(software_device->vk_device, "vkCreateEvent");
                            software_device->func.vk_destroy_event = (PFN_vkDestroyEvent)load(software_device->vk_device, "vkDestroyEvent");
                            software_device->func.vk_get_event_status = (PFN_vkGetEventStatus)load(software_device->vk_device, "vkGetEventStatus");
                            software_device->func.vk_set_event = (PFN_vkSetEvent)load(software_device->vk_device, "vkSetEvent");
                            software_device->func.vk_reset_event = (PFN_vkResetEvent)load(software_device->vk_device, "vkResetEvent");
                            software_device->func.vk_create_query_pool = (PFN_vkCreateQueryPool)load(software_device->vk_device, "vkCreateQueryPool");
                            software_device->func.vk_destroy_query_pool = (PFN_vkDestroyQueryPool)load(software_device->vk_device, "vkDestroyQueryPool");
                            software_device->func.vk_get_query_pool_results = (PFN_vkGetQueryPoolResults)load(software_device->vk_device, "vkGetQueryPoolResults");
                            software_device->func.vk_create_buffer = (PFN_vkCreateBuffer)load(software_device->vk_device, "vkCreateBuffer");
                            software_device->func.vk_destroy_buffer = (PFN_vkDestroyBuffer)load(software_device->vk_device, "vkDestroyBuffer");
                            software_device->func.vk_create_buffer_view = (PFN_vkCreateBufferView)load(software_device->vk_device, "vkCreateBufferView");
                            software_device->func.vk_destroy_buffer_view = (PFN_vkDestroyBufferView)load(software_device->vk_device, "vkDestroyBufferView");
                            software_device->func.vk_create_image = (PFN_vkCreateImage)load(software_device->vk_device, "vkCreateImage");
                            software_device->func.vk_destroy_image = (PFN_vkDestroyImage)load(software_device->vk_device, "vkDestroyImage");
                            software_device->func.vk_get_image_subresource_layout = (PFN_vkGetImageSubresourceLayout)load(software_device->vk_device, "vkGetImageSubresourceLayout");
                            software_device->func.vk_create_image_view = (PFN_vkCreateImageView)load(software_device->vk_device, "vkCreateImageView");
                            software_device->func.vk_destroy_image_view = (PFN_vkDestroyImageView)load(software_device->vk_device, "vkDestroyImageView");
                            software_device->func.vk_create_shader_module = (PFN_vkCreateShaderModule)load(software_device->vk_device, "vkCreateShaderModule");
                            software_device->func.vk_destroy_shader_module = (PFN_vkDestroyShaderModule)load(software_device->vk_device, "vkDestroyShaderModule");
                            software_device->func.vk_create_pipeline_cache = (PFN_vkCreatePipelineCache)load(software_device->vk_device, "vkCreatePipelineCache");
                            software_device->func.vk_destroy_pipeline_cache = (PFN_vkDestroyPipelineCache)load(software_device->vk_device, "vkDestroyPipelineCache");
                            software_device->func.vk_get_pipeline_cache_data = (PFN_vkGetPipelineCacheData)load(software_device->vk_device, "vkGetPipelineCacheData");
                            software_device->func.vk_merge_pipeline_caches = (PFN_vkMergePipelineCaches)load(software_device->vk_device, "vkMergePipelineCaches");
                            software_device->func.vk_create_graphics_pipelines = (PFN_vkCreateGraphicsPipelines)load(software_device->vk_device, "vkCreateGraphicsPipelines");
                            software_device->func.vk_create_compute_pipelines = (PFN_vkCreateComputePipelines)load(software_device->vk_device, "vkCreateComputePipelines");
                            software_device->func.vk_destroy_pipeline = (PFN_vkDestroyPipeline)load(software_device->vk_device, "vkDestroyPipeline");
                            software_device->func.vk_create_pipeline_layout = (PFN_vkCreatePipelineLayout)load(software_device->vk_device, "vkCreatePipelineLayout");
                            software_device->func.vk_destroy_pipeline_layout = (PFN_vkDestroyPipelineLayout)load(software_device->vk_device, "vkDestroyPipelineLayout");
                            software_device->func.vk_create_sampler = (PFN_vkCreateSampler)load(software_device->vk_device, "vkCreateSampler");
                            software_device->func.vk_destroy_sampler = (PFN_vkDestroySampler)load(software_device->vk_device, "vkDestroySampler");
                            software_device->func.vk_create_descriptor_set_layout = (PFN_vkCreateDescriptorSetLayout)load(software_device->vk_device, "vkCreateDescriptorSetLayout");
                            software_device->func.vk_destroy_descriptor_set_layout = (PFN_vkDestroyDescriptorSetLayout)load(software_device->vk_device, "vkDestroyDescriptorSetLayout");
                            software_device->func.vk_create_descriptor_pool = (PFN_vkCreateDescriptorPool)load(software_device->vk_device, "vkCreateDescriptorPool");
                            software_device->func.vk_destroy_descriptor_pool = (PFN_vkDestroyDescriptorPool)load(software_device->vk_device, "vkDestroyDescriptorPool");
                            software_device->func.vk_reset_descriptor_pool = (PFN_vkResetDescriptorPool)load(software_device->vk_device, "vkResetDescriptorPool");
                            software_device->func.vk_allocate_descriptor_sets = (PFN_vkAllocateDescriptorSets)load(software_device->vk_device, "vkAllocateDescriptorSets");
                            software_device->func.vk_free_descriptor_sets = (PFN_vkFreeDescriptorSets)load(software_device->vk_device, "vkFreeDescriptorSets");
                            software_device->func.vk_update_descriptor_sets = (PFN_vkUpdateDescriptorSets)load(software_device->vk_device, "vkUpdateDescriptorSets");
                            software_device->func.vk_create_framebuffer = (PFN_vkCreateFramebuffer)load(software_device->vk_device, "vkCreateFramebuffer");
                            software_device->func.vk_destroy_framebuffer = (PFN_vkDestroyFramebuffer)load(software_device->vk_device, "vkDestroyFramebuffer");
                            software_device->func.vk_create_render_pass = (PFN_vkCreateRenderPass)load(software_device->vk_device, "vkCreateRenderPass");
                            software_device->func.vk_destroy_render_pass = (PFN_vkDestroyRenderPass)load(software_device->vk_device, "vkDestroyRenderPass");
                            software_device->func.vk_get_render_area_granularity = (PFN_vkGetRenderAreaGranularity)load(software_device->vk_device, "vkGetRenderAreaGranularity");
                            software_device->func.vk_create_command_pool = (PFN_vkCreateCommandPool)load(software_device->vk_device, "vkCreateCommandPool");
                            software_device->func.vk_destroy_command_pool = (PFN_vkDestroyCommandPool)load(software_device->vk_device, "vkDestroyCommandPool");
                            software_device->func.vk_reset_command_pool = (PFN_vkResetCommandPool)load(software_device->vk_device, "vkResetCommandPool");
                            software_device->func.vk_allocate_command_buffers = (PFN_vkAllocateCommandBuffers)load(software_device->vk_device, "vkAllocateCommandBuffers");
                            software_device->func.vk_free_command_buffers = (PFN_vkFreeCommandBuffers)load(software_device->vk_device, "vkFreeCommandBuffers");
                            software_device->func.vk_begin_command_buffer = (PFN_vkBeginCommandBuffer)load(software_device->vk_device, "vkBeginCommandBuffer");
                            software_device->func.vk_end_command_buffer = (PFN_vkEndCommandBuffer)load(software_device->vk_device, "vkEndCommandBuffer");
                            software_device->func.vk_reset_command_buffer = (PFN_vkResetCommandBuffer)load(software_device->vk_device, "vkResetCommandBuffer");
                            software_device->func.vk_cmd_bind_pipeline = (PFN_vkCmdBindPipeline)load(software_device->vk_device, "vkCmdBindPipeline");
                            software_device->func.vk_cmd_set_viewport = (PFN_vkCmdSetViewport)load(software_device->vk_device, "vkCmdSetViewport");
                            software_device->func.vk_cmd_set_scissor = (PFN_vkCmdSetScissor)load(software_device->vk_device, "vkCmdSetScissor");
                            software_device->func.vk_cmd_set_line_width = (PFN_vkCmdSetLineWidth)load(software_device->vk_device, "vkCmdSetLineWidth");
                            software_device->func.vk_cmd_set_depth_bias = (PFN_vkCmdSetDepthBias)load(software_device->vk_device, "vkCmdSetDepthBias");
                            software_device->func.vk_cmd_set_blend_constants = (PFN_vkCmdSetBlendConstants)load(software_device->vk_device, "vkCmdSetBlendConstants");
                            software_device->func.vk_cmd_set_depth_bounds = (PFN_vkCmdSetDepthBounds)load(software_device->vk_device, "vkCmdSetDepthBounds");
                            software_device->func.vk_cmd_set_stencil_compare_mask = (PFN_vkCmdSetStencilCompareMask)load(software_device->vk_device, "vkCmdSetStencilCompareMask");
                            software_device->func.vk_cmd_set_stencil_write_mask = (PFN_vkCmdSetStencilWriteMask)load(software_device->vk_device, "vkCmdSetStencilWriteMask");
                            software_device->func.vk_cmd_set_stencil_reference = (PFN_vkCmdSetStencilReference)load(software_device->vk_device, "vkCmdSetStencilReference");
                            software_device->func.vk_cmd_bind_descriptor_sets = (PFN_vkCmdBindDescriptorSets)load(software_device->vk_device, "vkCmdBindDescriptorSets");
                            software_device->func.vk_cmd_bind_index_buffer = (PFN_vkCmdBindIndexBuffer)load(software_device->vk_device, "vkCmdBindIndexBuffer");
                            software_device->func.vk_cmd_bind_vertex_buffers = (PFN_vkCmdBindVertexBuffers)load(software_device->vk_device, "vkCmdBindVertexBuffers");
                            software_device->func.vk_cmd_draw = (PFN_vkCmdDraw)load(software_device->vk_device, "vkCmdDraw");
                            software_device->func.vk_cmd_draw_indexed = (PFN_vkCmdDrawIndexed)load(software_device->vk_device, "vkCmdDrawIndexed");
                            software_device->func.vk_cmd_draw_indirect = (PFN_vkCmdDrawIndirect)load(software_device->vk_device, "vkCmdDrawIndirect");
                            software_device->func.vk_cmd_draw_indexed_indirect = (PFN_vkCmdDrawIndexedIndirect)load(software_device->vk_device, "vkCmdDrawIndexedIndirect");
                            software_device->func.vk_cmd_dispatch = (PFN_vkCmdDispatch)load(software_device->vk_device, "vkCmdDispatch");
                            software_device->func.vk_cmd_dispatch_indirect = (PFN_vkCmdDispatchIndirect)load(software_device->vk_device, "vkCmdDispatchIndirect");
                            software_device->func.vk_cmd_copy_buffer = (PFN_vkCmdCopyBuffer)load(software_device->vk_device, "vkCmdCopyBuffer");
                            software_device->func.vk_cmd_copy_image = (PFN_vkCmdCopyImage)load(software_device->vk_device, "vkCmdCopyImage");
                            software_device->func.vk_cmd_blit_image = (PFN_vkCmdBlitImage)load(software_device->vk_device, "vkCmdBlitImage");
                            software_device->func.vk_cmd_copy_buffer_to_image = (PFN_vkCmdCopyBufferToImage)load(software_device->vk_device, "vkCmdCopyBufferToImage");
                            software_device->func.vk_cmd_copy_image_to_buffer = (PFN_vkCmdCopyImageToBuffer)load(software_device->vk_device, "vkCmdCopyImageToBuffer");
                            software_device->func.vk_cmd_update_buffer = (PFN_vkCmdUpdateBuffer)load(software_device->vk_device, "vkCmdUpdateBuffer");
                            software_device->func.vk_cmd_fill_buffer = (PFN_vkCmdFillBuffer)load(software_device->vk_device, "vkCmdFillBuffer");
                            software_device->func.vk_cmd_clear_color_image = (PFN_vkCmdClearColorImage)load(software_device->vk_device, "vkCmdClearColorImage");
                            software_device->func.vk_cmd_clear_depth_stencil_image = (PFN_vkCmdClearDepthStencilImage)load(software_device->vk_device, "vkCmdClearDepthStencilImage");
                            software_device->func.vk_cmd_clear_attachments = (PFN_vkCmdClearAttachments)load(software_device->vk_device, "vkCmdClearAttachments");
                            software_device->func.vk_cmd_resolve_image = (PFN_vkCmdResolveImage)load(software_device->vk_device, "vkCmdResolveImage");
                            software_device->func.vk_cmd_set_event = (PFN_vkCmdSetEvent)load(software_device->vk_device, "vkCmdSetEvent");
                            software_device->func.vk_cmd_reset_event = (PFN_vkCmdResetEvent)load(software_device->vk_device, "vkCmdResetEvent");
                            software_device->func.vk_cmd_wait_events = (PFN_vkCmdWaitEvents)load(software_device->vk_device, "vkCmdWaitEvents");
                            software_device->func.vk_cmd_pipeline_barrier = (PFN_vkCmdPipelineBarrier)load(software_device->vk_device, "vkCmdPipelineBarrier");
                            software_device->func.vk_cmd_begin_query = (PFN_vkCmdBeginQuery)load(software_device->vk_device, "vkCmdBeginQuery");
                            software_device->func.vk_cmd_end_query = (PFN_vkCmdEndQuery)load(software_device->vk_device, "vkCmdEndQuery");
                            software_device->func.vk_cmd_reset_query_pool = (PFN_vkCmdResetQueryPool)load(software_device->vk_device, "vkCmdResetQueryPool");
                            software_device->func.vk_cmd_write_timestamp = (PFN_vkCmdWriteTimestamp)load(software_device->vk_device, "vkCmdWriteTimestamp");
                            software_device->func.vk_cmd_copy_query_pool_results = (PFN_vkCmdCopyQueryPoolResults)load(software_device->vk_device, "vkCmdCopyQueryPoolResults");
                            software_device->func.vk_cmd_push_constants = (PFN_vkCmdPushConstants)load(software_device->vk_device, "vkCmdPushConstants");
                            software_device->func.vk_cmd_begin_render_pass = (PFN_vkCmdBeginRenderPass)load(software_device->vk_device, "vkCmdBeginRenderPass");
                            software_device->func.vk_cmd_next_subpass = (PFN_vkCmdNextSubpass)load(software_device->vk_device, "vkCmdNextSubpass");
                            software_device->func.vk_cmd_end_render_pass = (PFN_vkCmdEndRenderPass)load(software_device->vk_device, "vkCmdEndRenderPass");
                            software_device->func.vk_cmd_execute_commands = (PFN_vkCmdExecuteCommands)load(software_device->vk_device, "vkCmdExecuteCommands");
                            software_device->func.vk_create_swapchain_khr = (PFN_vkCreateSwapchainKHR)load(software_device->vk_device, "vkCreateSwapchainKHR");
                            software_device->func.vk_destroy_swapchain_khr = (PFN_vkDestroySwapchainKHR)load(software_device->vk_device, "vkDestroySwapchainKHR");
                            software_device->func.vk_get_swaphain_images_khr = (PFN_vkGetSwapchainImagesKHR)load(software_device->vk_device, "vkGetSwapchainImagesKHR");
                            software_device->func.vk_queue_present_khr = (PFN_vkQueuePresentKHR)load(software_device->vk_device, "vkQueuePresentKHR");
                            software_device->func.vk_acquire_next_image_khr = (PFN_vkAcquireNextImageKHR)load(software_device->vk_device, "vkAcquireNextImageKHR");

                            VkCommandPoolCreateInfo command_pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 0};
                            for (QueueInfo &queue_info : queue_infos)
                            {
                                   command_pool_info.queueFamilyIndex = queue_info.queue_family_index;
                                   switch (queue_info.types)
                                   {
                                   case VK_QUEUE_GRAPHICS_BIT:
                                   {
                                          software_device->graphics_queue->types = VK_QUEUE_GRAPHICS_BIT;
                                          software_device->func.vk_get_device_queue(software_device->vk_device, queue_info.queue_family_index, queue_info.queue_index, &software_device->graphics_queue->vk_queue);
                                          vk_assert(software_device->func.vk_create_command_pool(software_device->vk_device, &command_pool_info, nullptr, &software_device->graphics_queue->vk_command_pool));
                                          break;
                                   }
                                   case VK_QUEUE_COMPUTE_BIT:
                                   {
                                          software_device->compute_queue->types = VK_QUEUE_COMPUTE_BIT;
                                          software_device->func.vk_get_device_queue(software_device->vk_device, queue_info.queue_family_index, queue_info.queue_index, &software_device->compute_queue->vk_queue);
                                          vk_assert(software_device->func.vk_create_command_pool(software_device->vk_device, &command_pool_info, nullptr, &software_device->compute_queue->vk_command_pool));
                                          break;
                                   }
                                   case VK_QUEUE_TRANSFER_BIT:
                                   {
                                          software_device->transfer_queue->types = VK_QUEUE_TRANSFER_BIT;
                                          software_device->func.vk_get_device_queue(software_device->vk_device, queue_info.queue_family_index, queue_info.queue_index, &software_device->transfer_queue->vk_queue);
                                          vk_assert(software_device->func.vk_create_command_pool(software_device->vk_device, &command_pool_info, nullptr, &software_device->transfer_queue->vk_command_pool));
                                          break;
                                   }
                                   }
                            }
                            if (create_info.request_present_queue)
                            {
                                   software_device->present_queue = new Queue_I();
                                   software_device->present_queue->types = 0;
                                   software_device->func.vk_get_device_queue(software_device->vk_device, queue_infos.back().queue_family_index, queue_infos.back().queue_index, &software_device->present_queue->vk_queue);
                            }
                            else
                            {
                                   software_device->present_queue = nullptr;
                            }
                            return software_device;
                     }

                     void destroy(SoftwareDevice software_device)
                     {
                            if(software_device->graphics_queue)
                            {
                                   software_device->func.vk_destroy_command_pool(software_device->vk_device, software_device->graphics_queue->vk_command_pool, nullptr);
                            }
                            if(software_device->compute_queue)
                            {
                                   software_device->func.vk_destroy_command_pool(software_device->vk_device, software_device->compute_queue->vk_command_pool, nullptr);
                            }
                            if(software_device->transfer_queue)
                            {
                                   software_device->func.vk_destroy_command_pool(software_device->vk_device, software_device->transfer_queue->vk_command_pool, nullptr);
                            }
                            software_device->func.vk_destroy_device(software_device->vk_device, nullptr);
                            delete software_device;
                     }

                     void push_operation(SoftwareDevice software_device, std::function<void(SoftwareDevice)> &func)
                     {
                            software_device->operations.push_back(func);
                     }

                     void wait(SoftwareDevice software_device)
                     {
                            vk_assert(software_device->func.vk_device_wait_idle(software_device->vk_device));
                     }

                     void process_operations(SoftwareDevice software_device)
                     {
                            for (auto &operation : software_device->operations)
                            {
                                   operation(software_device);
                            }
                            software_device->operations.clear();
                     }
              }
       }
}