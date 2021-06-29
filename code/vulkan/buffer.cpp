#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace buffer
              {
                     Buffer create(SoftwareDevice software_device, const CreateInfo &create_info)
                     {
                            Buffer buffer = new Buffer_I();
                            VkBufferCreateInfo buffer_info =
                                {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr, 0, create_info.size, switch_buffer_usage(create_info.usage), software_device->vk_sharing_mode,
                                 (std::uint32_t)software_device->queue_family_indices.size(), software_device->queue_family_indices.data()};
                            vk_assert(software_device->func.vk_create_buffer(software_device->vk_device, &buffer_info, nullptr, &buffer->vk_buffer));
                            VmaAllocationCreateInfo alloc_info = {0};
                            alloc_info.usage = switch_device_memory_type(create_info.memory_type);
                            vk_assert(vmaAllocateMemoryForBuffer(software_device->vma_allocator, buffer->vk_buffer, &alloc_info, &buffer->vma_allocation, nullptr));
                            vk_assert(vmaBindBufferMemory(software_device->vma_allocator, buffer->vma_allocation, buffer->vk_buffer));
                            buffer->map = nullptr;
                            return buffer;
                     }

                     void destroy(SoftwareDevice software_device, Buffer buffer)
                     {
                            if(buffer->map)
                            {
                                   vmaUnmapMemory(software_device->vma_allocator, buffer->vma_allocation);
                            }
                            vmaFreeMemory(software_device->vma_allocator, buffer->vma_allocation);
                            software_device->func.vk_destroy_buffer(software_device->vk_device, buffer->vk_buffer, nullptr);
                            delete buffer;
                     }

                     void *map(SoftwareDevice software_device, Buffer buffer)
                     {
                            vk_assert(vmaMapMemory(software_device->vma_allocator, buffer->vma_allocation, &buffer->map));
                            return buffer->map;
                     }

                     void unmap(SoftwareDevice software_device, Buffer buffer)
                     {
                            vmaUnmapMemory(software_device->vma_allocator, buffer->vma_allocation);
                            buffer->map = nullptr;
                     }

                     void *get_map(Buffer buffer)
                     {
                            return buffer->map;
                     }
              }
       }
}