#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace shader
              {
                     Shader create(SoftwareDevice software_device, const CreateInfo &create_info)
                     {
                            Shader shader = new Shader_I();
                            VkShaderModuleCreateInfo module_info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, 0, create_info.size, (const std::uint32_t*)create_info.code};
                            vk_assert(software_device->func.vk_create_shader_module(software_device->vk_device, &module_info, nullptr, &shader->vk_shader_module));
                            return shader;
                     }

                     void destroy(SoftwareDevice software_device, Shader shader)
                     {
                            software_device->func.vk_destroy_shader_module(software_device->vk_device, shader->vk_shader_module, nullptr);
                            delete shader;
                     }
              }
       }
}