#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace framebuffer
              {
                     Framebuffer create(SoftwareDevice software_device, const CreateInfo &create_info)
                     {
                            Framebuffer framebuffer = new Framebuffer_I();
                            std::vector<VkImageView> attachments(create_info.attachment_count);
                            for (size_t i = 0; i < create_info.attachment_count; ++i)
                            {
                                   attachments[i] = create_info.attachments[i]->vk_image_view;
                            }
                            VkFramebufferCreateInfo framebuffer_info =
                                {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr, 0, create_info.render_pass->vk_render_pass, create_info.attachment_count, attachments.data(),
                                 create_info.width, create_info.height, create_info.layer_count};
                            vk_assert(software_device->func.vk_create_framebuffer(software_device->vk_device, &framebuffer_info, nullptr, &framebuffer->vk_framebuffer));
                            return framebuffer;
                     }

                     void destroy(SoftwareDevice software_device, Framebuffer framebuffer)
                     {
                            software_device->func.vk_destroy_framebuffer(software_device->vk_device, framebuffer->vk_framebuffer, nullptr);
                            delete framebuffer;
                     }
              }
       }
}