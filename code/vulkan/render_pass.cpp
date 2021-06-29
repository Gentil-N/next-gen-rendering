#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace render_pass
              {
                     RenderPass create(SoftwareDevice software_device, const CreateInfo &create_info)
                     {
                            RenderPass render_pass = new RenderPass_I();
                            std::vector<VkAttachmentDescription> attachments(create_info.attachment_count);
                            for (size_t i = 0; i < attachments.size(); ++i)
                            {
                                   attachments[i].flags = 0;
                                   attachments[i].format = switch_texture_format(create_info.attachments[i].format);
                                   attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
                                   attachments[i].loadOp = switch_attachment_load_op(create_info.attachments[i].load_op);
                                   attachments[i].storeOp = switch_attachment_store_op(create_info.attachments[i].store_op);
                                   attachments[i].stencilLoadOp = switch_attachment_load_op(create_info.attachments[i].stencil_load_op);
                                   attachments[i].stencilStoreOp = switch_attachment_store_op(create_info.attachments[i].stencil_store_op);
                                   attachments[i].initialLayout = switch_texture_layout(create_info.attachments[i].initial_layout);
                                   attachments[i].finalLayout = switch_texture_layout(create_info.attachments[i].final_layout);
                            }
                            std::vector<VkSubpassDescription> subpasses(create_info.subpass_count);
                            std::vector<VkAttachmentReference> attachment_refs;
                            for (size_t i = 0; i < subpasses.size(); ++i)
                            {
                                   subpasses[i].flags = 0;
                                   subpasses[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                                   subpasses[i].inputAttachmentCount = create_info.subpasses[i].input_attachment_count;
                                   subpasses[i].pInputAttachments = attachment_refs.data() + attachment_refs.size();
                                   for (size_t j = 0; j < subpasses[i].inputAttachmentCount; ++j)
                                   {
                                          attachment_refs.push_back(
                                              VkAttachmentReference{
                                                  create_info.subpasses[i].input_attachments[j].index,
                                                  switch_texture_layout(create_info.subpasses[i].input_attachments[j].layout)});
                                   }
                                   subpasses[i].colorAttachmentCount = create_info.subpasses[i].color_attachment_count;
                                   subpasses[i].pColorAttachments = attachment_refs.data() + attachment_refs.size();
                                   for (size_t j = 0; j < subpasses[i].colorAttachmentCount; ++j)
                                   {
                                          attachment_refs.push_back(
                                              VkAttachmentReference{
                                                  create_info.subpasses[i].color_attachments[j].index,
                                                  switch_texture_layout(create_info.subpasses[i].color_attachments[j].layout)});
                                   }
                                   if (create_info.subpasses[i].resolve_attachment)
                                   {
                                          subpasses[i].pResolveAttachments = attachment_refs.data() + attachment_refs.size();
                                          attachment_refs.push_back(
                                              VkAttachmentReference{
                                                  create_info.subpasses[i].resolve_attachment->index,
                                                  switch_texture_layout(create_info.subpasses[i].resolve_attachment->layout)});
                                   }
                                   else
                                   {
                                          subpasses[i].pResolveAttachments = nullptr;
                                   }
                                   if (create_info.subpasses[i].depth_stencil_attachment)
                                   {
                                          subpasses[i].pDepthStencilAttachment = attachment_refs.data() + attachment_refs.size();
                                          attachment_refs.push_back(
                                              VkAttachmentReference{
                                                  create_info.subpasses[i].depth_stencil_attachment->index,
                                                  switch_texture_layout(create_info.subpasses[i].depth_stencil_attachment->layout)});
                                   }
                                   else
                                   {
                                          subpasses[i].pDepthStencilAttachment = nullptr;
                                   }
                                   subpasses[i].preserveAttachmentCount = create_info.subpasses[i].preserve_attachment_count;
                                   subpasses[i].pPreserveAttachments = create_info.subpasses[i].preserve_attachments;
                            }
                            std::vector<VkSubpassDependency> subpass_dependencies(create_info.dependency_count);
                            for (size_t i = 0; i < subpass_dependencies.size(); ++i)
                            {
                                   subpass_dependencies[i].srcSubpass = create_info.dependencies[i].src_subpass;
                                   subpass_dependencies[i].dstSubpass = create_info.dependencies[i].dst_subpass;
                                   subpass_dependencies[i].srcStageMask = switch_pipeline_stage(create_info.dependencies[i].src_stage);
                                   subpass_dependencies[i].dstStageMask = switch_pipeline_stage(create_info.dependencies[i].dst_stage);
                                   subpass_dependencies[i].srcAccessMask = switch_pipeline_access(create_info.dependencies[i].src_access);
                                   subpass_dependencies[i].dstAccessMask = switch_pipeline_access(create_info.dependencies[i].dst_access);
                                   subpass_dependencies[i].dependencyFlags = 0;
                            }

                            VkRenderPassCreateInfo render_pass_info =
                                {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr, 0,
                                 (std::uint32_t)attachments.size(), attachments.data(),
                                 (std::uint32_t)subpasses.size(), subpasses.data(),
                                 (std::uint32_t)subpass_dependencies.size(), subpass_dependencies.data()};
                            vk_assert(software_device->func.vk_create_render_pass(software_device->vk_device, &render_pass_info, nullptr, &render_pass->vk_render_pass));
                            return render_pass;
                     }

                     void destroy(SoftwareDevice software_device, RenderPass render_pass)
                     {
                            software_device->func.vk_destroy_render_pass(software_device->vk_device, render_pass->vk_render_pass, nullptr);
                            delete render_pass;
                     }
              }
       }
}