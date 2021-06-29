#pragma once
#ifndef __NEXT_GEN_RENDERING_HPP__
#define __NEXT_GEN_RENDERING_HPP__

#if defined(__linux__) && !defined(__ANDROID__)
#define NGR_LINUX_PLATFORM
#include <X11/Xlib.h>
#else
#error "no other platforms supported yet"
#endif

#include <cstdint>
#include <utility>
#include <functional>

#define NGR_DEF_ENUM_OR_BIT_FLAGS(enum_name, enum_type)                                                      \
       inline enum_name operator|(enum_name first, enum_name second)                                         \
       {                                                                                                     \
              return static_cast<enum_name>(static_cast<enum_type>(first) | static_cast<enum_type>(second)); \
       }
#define NGR_DEF_ENUM_AND_BIT_FLAGS(enum_name, enum_type)                                                     \
       inline enum_name operator&(enum_name first, enum_name second)                                         \
       {                                                                                                     \
              return static_cast<enum_name>(static_cast<enum_type>(first) & static_cast<enum_type>(second)); \
       }

namespace ngr
{
       template <typename type>
       using ptr = type *;

       namespace api
       {
              /**
               * STRUCTS
               */
              using Driver = struct Driver_I *;
              using HardwareDevice = struct HardwareDevice_I *;
              using SoftwareDevice = struct SoftwareDevice_I *;
              using Queue = struct Queue_I *;
              using Command = struct Command_I *;
              using Buffer = struct Buffer_I *;
              using Texture = struct Texture_I *;
              using Sampler = struct Sampler_I *;
              using RenderPass = struct RenderPass_I *;
              using Framebuffer = struct Framebuffer_I *;
              using Shader = struct Shader_I *;
              using PipLink = struct PipLink_I *;
              using UniformSet = struct UniformSet_I *;
              using Pipeline = struct Pipeline_I *;
              namespace ext
              {
                     using WinLink = struct WinLink_I *;
                     using Swapchain = struct Swapchain_I *;
              }

              /**
               * FUNCTIONS
               */
              namespace driver
              {
                     struct LoadInfo
                     {
                            bool request_debug_mode;
                            bool request_win_link_support;
                     };

                     Driver load(const LoadInfo &load_info);

                     void unload(Driver driver);
              }

              namespace hardware_device
              {
                     struct Features
                     {
                            bool image_cube_array;
                            bool independent_blend;
                            bool geometry_shader;
                            bool tessellation_shader;
                            bool sample_rate_shading;
                            bool dual_src_blend;
                            bool logic_op;
                            bool depth_clamp;
                            bool depth_bias_clamp;
                            bool fill_mode_non_solid;
                            bool depth_bounds;
                            bool wide_lines;
                            bool large_points;
                            bool alpha_to_one;
                            bool multi_viewport;
                            bool sampler_anisotropy;
                            bool occlusion_query_precise;
                            bool pipeline_statistics_query;
                            bool shader_clip_distance;
                            bool shader_cull_distance;
                     };

                     std::uint32_t count(Driver driver);

                     HardwareDevice get(Driver driver, std::uint32_t index);

                     bool is_swapchain_extension_supported(HardwareDevice hardware_device, ext::WinLink win_link); // check for present queue

                     //bool is_win_link_extension_supported(HardwareDevice hardware_device, ext::WinLink win_link); // check for present queue

                     bool has_graphics_queue(HardwareDevice hardware_device);

                     bool has_compute_queue(HardwareDevice hardware_device);

                     bool has_transfer_queue(HardwareDevice hardware_device);

                     bool is_dedicated(HardwareDevice hardware_device);

                     void get_features(Features &features);

                     /*add : test texture format support*/
              }

              namespace software_device
              {
                     enum MemoryType
                     {
                            CPU,
                            GPU,
                            CPU_TO_GPU,
                            GPU_TO_CPU
                     };

                     struct CreateInfo
                     {
                            HardwareDevice hardware_device;
                            bool request_graphics_queue;
                            bool request_compute_queue;
                            bool request_transfer_queue;
                            bool request_swapchain_extension; // request present queue
                            ext::WinLink win_link;
                     };

                     SoftwareDevice create(Driver driver, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device);

                     void push_operation(SoftwareDevice software_device, std::function<void(SoftwareDevice)> &func);

                     void wait(SoftwareDevice software_device);

                     void process_operations(SoftwareDevice software_device);
              }

              namespace queue
              {
                     /*impl : command pool for each queue*/
                     Queue graphics(SoftwareDevice software_device);

                     Queue compute(SoftwareDevice software_device);

                     Queue transfer(SoftwareDevice software_device);

                     void wait(SoftwareDevice software_device, Queue queue);
              }

              namespace buffer
              {
                     enum class Usage : std::uint32_t
                     {
                            TRANSFER_SRC = 0x1,
                            TRANSFER_DST = 0x2,
                            UNIFORM = 0x4,
                            VERTEX = 0x8,
                            INDEX = 0x10,
                            INDIRECT = 0x20,
                            STORAGE = 0x40,
                     };
                     NGR_DEF_ENUM_OR_BIT_FLAGS(Usage, std::uint32_t);
                     NGR_DEF_ENUM_AND_BIT_FLAGS(Usage, std::uint32_t);

                     struct CreateInfo
                     {
                            Usage usage;
                            software_device::MemoryType memory_type;
                            std::uint64_t size;
                     };

                     /*impl : automatic sharing mode with queues from device*/
                     Buffer create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device, Buffer buffer);

                     void *map(SoftwareDevice software_device, Buffer buffer);

                     void unmap(SoftwareDevice software_device, Buffer buffer);

                     void *get_map(Buffer buffer);
              }

              namespace texture
              {
                     enum Format
                     {
                            UNDEFINED = 0,

                            R_8_UNORM,
                            R_8_SNORM,
                            R_8_USCALED,
                            R_8_SSCALED,
                            R_8_UINT,
                            R_8_SINT,
                            R_8_SRGB,

                            RG_8_UNORM,
                            RG_8_SNORM,
                            RG_8_USCALED,
                            RG_8_SSCALED,
                            RG_8_UINT,
                            RG_8_SINT,
                            RG_8_SRGB,

                            RGB_8_UNORM,
                            RGB_8_SNORM,
                            RGB_8_USCALED,
                            RGB_8_SSCALED,
                            RGB_8_UINT,
                            RGB_8_SINT,
                            RGB_8_SRGB,

                            BGR_8_UNORM,
                            BGR_8_SNORM,
                            BGR_8_USCALED,
                            BGR_8_SSCALED,
                            BGR_8_UINT,
                            BGR_8_SINT,
                            BGR_8_SRGB,

                            RGBA_8_UNORM,
                            RGBA_8_SNORM,
                            RGBA_8_USCALED,
                            RGBA_8_SSCALED,
                            RGBA_8_UINT,
                            RGBA_8_SINT,
                            RGBA_8_SRGB,

                            BGRA_8_UNORM,
                            BGRA_8_SNORM,
                            BGRA_8_USCALED,
                            BGRA_8_SSCALED,
                            BGRA_8_UINT,
                            BGRA_8_SINT,
                            BGRA_8_SRGB,

                            R_16_UNORM,
                            R_16_SNORM,
                            R_16_USCALED,
                            R_16_SSCALED,
                            R_16_UINT,
                            R_16_SINT,
                            R_16_SFLOAT,

                            RG_16_UNORM,
                            RG_16_SNORM,
                            RG_16_USCALED,
                            RG_16_SSCALED,
                            RG_16_UINT,
                            RG_16_SINT,
                            RG_16_SFLOAT,

                            RGB_16_UNORM,
                            RGB_16_SNORM,
                            RGB_16_USCALED,
                            RGB_16_SSCALED,
                            RGB_16_UINT,
                            RGB_16_SINT,
                            RGB_16_SFLOAT,

                            RGBA_16_UNORM,
                            RGBA_16_SNORM,
                            RGBA_16_USCALED,
                            RGBA_16_SSCALED,
                            RGBA_16_UINT,
                            RGBA_16_SINT,
                            RGBA_16_SFLOAT,

                            R_32_UINT,
                            R_32_SINT,
                            R_32_SFLOAT,

                            RG_32_UINT,
                            RG_32_SINT,
                            RG_32_SFLOAT,

                            RGB_32_UINT,
                            RGB_32_SINT,
                            RGB_32_SFLOAT,

                            RGBA_32_UINT,
                            RGBA_32_SINT,
                            RGBA_32_SFLOAT,

                            R_64_UINT,
                            R_64_SINT,
                            R_64_SFLOAT,

                            RG_64_UINT,
                            RG_64_SINT,
                            RG_64_SFLOAT,

                            RGB_64_UINT,
                            RGB_64_SINT,
                            RGB_64_SFLOAT,

                            RGBA_64_UINT,
                            RGBA_64_SINT,
                            RGBA_64_SFLOAT,

                            D16_UNORM,
                            D32_SFLOAT,
                            S8_UINT,
                            D16_UNORM_S8_UINT,
                            D24_UNORM_S8_UINT,
                            D32_SFLOAT_S8_UINT
                     };

                     enum class Layout
                     {
                            UNDEFINED,
                            COLOR_ATTACHMENT,
                            DEPTH_STENCIL_ATTACHMENT,
                            DEPTH_STENCIL_READ_ONLY,
                            SHADER_READ_ONLY,
                            TRANSFER_SRC,
                            TRANSFER_DST,
                            DEPTH_READ_ONLY_STENCIL_ATTACHMENT,
                            DEPTH_ATTACHMENT_STENCIL_READ_ONLY,
                            DEPTH_ATTACHMENT,
                            DEPTH_READ_ONLY,
                            STENCIL_ATTACHMENT,
                            STENCIL_READ_ONLY,
                            PRESENT,
                     };

                     enum class Usage : std::uint32_t
                     {
                            TRANSFER_DST = 0x1,
                            TRANSFER_SRC = 0x2,
                            SAMPLED = 0x4,
                            STORAGE = 0x8,
                            COLOR_ATTACHMENT = 0x10,
                            DEPTH_STENCIL_ATTACHMENT = 0x20,
                            TRANSIENT_ATTACHMENT = 0x40,
                            INPUT_ATTACHMENT = 0x80,
                     };
                     NGR_DEF_ENUM_OR_BIT_FLAGS(Usage, std::uint32_t);
                     NGR_DEF_ENUM_AND_BIT_FLAGS(Usage, std::uint32_t);

                     struct CreateInfoBase
                     {
                            software_device::MemoryType memory_type;
                            Format format;
                            bool tiling_linear;
                            Usage usage;
                            /*impl : aspect detected from usage*/
                            std::uint32_t mip_level_count;
                            std::uint32_t width;
                     };

                     struct CreateInfoArray
                     {
                            std::uint32_t array_layer_count;
                     };

                     struct CreateInfo1D : CreateInfoBase
                     {
                     };

                     struct CreateInfo1DArray : CreateInfo1D, CreateInfoArray
                     {
                     };

                     struct CreateInfo2D : CreateInfoBase
                     {
                            /*add : samples*/
                            std::uint32_t height;
                     };

                     struct CreateInfo2DArray : CreateInfo2D, CreateInfoArray
                     {
                     };

                     struct CreateInfoCube : CreateInfoBase
                     {
                            // 6 layers
                     };

                     struct CreateInfoCubeArray : CreateInfoCube
                     {
                            std::uint32_t array_layer_cube_count; // 'array_layer_count = array_layer_cube_count * 6'
                     };

                     struct CreateInfo3D : CreateInfoBase
                     {
                            std::uint32_t height, depth;
                     };

                     Texture create_1d(SoftwareDevice software_device, const CreateInfo1D &create_info_1d);

                     Texture create_1d_array(SoftwareDevice software_device, const CreateInfo1DArray &create_info_1d_array);

                     Texture create_2d(SoftwareDevice software_device, const CreateInfo2D &create_info_2d);

                     Texture create_2d_array(SoftwareDevice software_device, const CreateInfo2DArray &create_info_2d_array);

                     Texture create_cube(SoftwareDevice software_device, const CreateInfoCube &create_info_cube);

                     Texture create_cube_array(SoftwareDevice software_device, const CreateInfoCubeArray &create_info_cube_array);

                     Texture create_3d(SoftwareDevice software_device, const CreateInfo3D &create_info_3d);

                     void destroy(SoftwareDevice software_device, Texture texture);

                     void *map(SoftwareDevice software_device, Texture texture);

                     void unmap(SoftwareDevice software_device, Texture texture);

                     void *get_map(Texture texture);

                     struct CreateInfoSampler
                     {
                     };

                     Sampler create_sampler(SoftwareDevice software_device, const CreateInfoSampler &create_info_sampler);

                     void destroy_sampler(SoftwareDevice software_device, Sampler sampler);
              }

              namespace render_pass
              {
                     enum AttachmentLoadOp
                     {
                            LOAD,
                            CLEAR,
                            DISCARD
                     };

                     enum AttachmentStoreOp
                     {
                            STORE,
                            DISCARD
                     };

                     struct Attachment
                     {
                            texture::Format format;
                            AttachmentLoadOp load_op, stencil_load_op;
                            AttachmentStoreOp store_op, stencil_store_op;
                            texture::Layout initial_layout, final_layout;
                     };

                     struct AttachmentReference
                     {
                            std::uint32_t index;
                            texture::Layout layout;
                     };

                     struct Subpass
                     {
                            std::uint32_t input_attachment_count;
                            const AttachmentReference *input_attachments;
                            std::uint32_t color_attachment_count;
                            const AttachmentReference *color_attachments;
                            const AttachmentReference *resolve_attachment;
                            const AttachmentReference *depth_stencil_attachment;
                            std::uint32_t preserve_attachment_count;
                            const std::uint32_t *preserve_attachments;
                     };

                     struct SubpassDependency
                     {
                            std::uint32_t src_subpass, dst_subpass;
                            pipeline::Stage src_stage, dst_stage;
                            pipeline::Access src_access, dst_access;
                     };

                     struct CreateInfo
                     {
                            std::uint32_t attachment_count;
                            const Attachment *attachments;
                            std::uint32_t subpass_count;
                            const Subpass *subpasses;
                            std::uint32_t dependency_count;
                            const SubpassDependency *dependencies;
                     };

                     RenderPass create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device, RenderPass render_pass);
              }

              namespace framebuffer
              {
                     struct CreateInfo
                     {
                            RenderPass render_pass;
                            std::uint32_t attachment_count;
                            const Texture *attachments;
                            std::uint32_t width, height, layer_count;
                     };

                     Framebuffer create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device, Framebuffer framebuffer);
              }

              namespace shader
              {
                     enum class Type : std::uint32_t
                     {
                            VERTEX = 0x1,
                            TESSELLATION_CONTROL = 0x2,
                            TESSELLATION_EVALUATION = 0x4,
                            GEOMETRY = 0x8,
                            FRAGMENT = 0x10,
                            COMPUTE = 0x20,
                            ALL = VERTEX | TESSELLATION_CONTROL | TESSELLATION_EVALUATION | GEOMETRY | FRAGMENT | COMPUTE
                     };
                     NGR_DEF_ENUM_OR_BIT_FLAGS(Type, std::uint32_t);
                     NGR_DEF_ENUM_AND_BIT_FLAGS(Type, std::uint32_t);

                     struct CreateInfo
                     {
                            const void *code;
                            std::uint64_t size;
                     };

                     Shader create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device, Shader shader);
              }

              namespace pip_link
              {
                     enum UniformType
                     {
                            BUFFER,
                            COMBINED_IMAGE_SAMPLER
                     };

                     struct UniformBindingInfo
                     {
                            UniformType type;
                            shader::Type shader_access;
                     };

                     struct UniformSetInfo
                     {
                            std::uint32_t uniform_binding_info_count;
                            UniformBindingInfo *uniform_binding_infos;
                     };

                     struct CreateInfo
                     {
                            std::uint32_t uniform_set_info_count;
                            UniformSetInfo *uniform_set_infos;
                     };

                     PipLink create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void destroy(SoftwareDevice software_device, PipLink pip_link);

                     struct AllocInfoUniformSet
                     {
                            std::uint32_t set_index;
                     };

                     UniformSet alloc_uniform_set(SoftwareDevice software_device, PipLink pip_link, const AllocInfoUniformSet &alloc_info_uniform_set);

                     void alloc_multiple_uniform_sets(
                         SoftwareDevice software_device, PipLink pip_link, const AllocInfoUniformSet &alloc_info_uniform_set, UniformSet *uniform_sets,
                         std::uint32_t uniform_set_count);

                     void free_uniform_set(PipLink pip_link, UniformSet uniform_set);

                     void free_multiple_uniform_set(PipLink pip_link, UniformSet *uniform_sets, std::uint32_t uniform_set_count);
              }

              namespace pipeline
              {
                     enum Type
                     {
                            GRAPHICS,
                            COMPUTE
                     };

                     enum class Stage : std::uint32_t
                     {
                            TOP_OF_PIPE = 0x1,
                            DRAW_INDIRECT = 0x2,
                            VERTEX_INPUT = 0x4,
                            VERTEX_SHADER = 0x8,
                            TESSELLATION_CONTROL_SHADER = 0x10,
                            TESSELLATION_EVALUATION_SHADER = 0x20,
                            GEOMETRY_SHADER = 0x40,
                            FRAGMENT_SHADER = 0x80,
                            EARLY_FRAGMENT_TESTS = 0x100,
                            LATE_FRAGMENT_TESTS = 0x200,
                            COLOR_ATTACHMENT = 0x400,
                            COMPUTE_SHADER = 0x800,
                            TRANSFER = 0x1000,
                            BOTTOM_OF_PIPE = 0x2000,
                            HOST = 0x4000,
                     };
                     NGR_DEF_ENUM_OR_BIT_FLAGS(Stage, std::uint32_t);
                     NGR_DEF_ENUM_AND_BIT_FLAGS(Stage, std::uint32_t);

                     enum class Access : std::uint32_t
                     {
                            INDIRECT_COMMAND_READ = 0x1,
                            INDEX_READ = 0x2,
                            VERTEX_ATTRIBUTE_READ = 0x4,
                            UNIFORM_READ = 0x8,
                            INPUT_ATTACHMENT_READ = 0x10,
                            SHADER_READ = 0x20,
                            SHADER_WRITE = 0x40,
                            COLOR_ATTACHMENT_READ = 0x80,
                            COLOR_ATTACHMENT_WRITE = 0x100,
                            DEPTH_STENCIL_ATTACHMENT_READ = 0x200,
                            DEPTH_STENCIL_ATTACHMENT_WRITE = 0x400,
                            TRANSFER_READ = 0x800,
                            TRANSFER_WRITE = 0x1000,
                            HOST_READ = 0x2000,
                            HOST_WRITE = 0x4000,
                            MEMORY_READ = 0x8000,
                            MEMORY_WRITE = 0x10000,
                     };
                     NGR_DEF_ENUM_OR_BIT_FLAGS(Access, std::uint32_t);
                     NGR_DEF_ENUM_AND_BIT_FLAGS(Access, std::uint32_t);

                     namespace graphics
                     {
                            enum VertexInput
                            {
                                   INT,
                                   IVEC2,
                                   IVEC3,
                                   IVEC4,
                                   FLOAT,
                                   VEC2,
                                   VEC3,
                                   VEC4,
                                   MAT3,
                                   MAT4,
                            };

                            struct VertexInputBinding
                            {
                                   bool per_instance;
                                   std::uint32_t attribute_count;
                                   const VertexInput *attributes;
                            };

                            enum Primitive
                            {
                                   POINT_LIST,
                                   LINE_LIST,
                                   LINE_STRIP,
                                   TRIANGLE_LIST,
                                   TRIANGLE_STRIP,
                                   TRIANGLE_FAN,
                                   LINE_LIST_WITH_ADJACENCY,
                                   LINE_STRIP_WITH_ADJACENCY,
                                   TRIANGLE_LIST_WITH_ADJACENCY,
                                   TRIANGLE_STRIP_WITH_ADJACENCY
                            };

                            enum Polygon
                            {
                                   FILL,
                                   LINE,
                                   POINT
                            };

                            enum Cull
                            {
                                   NONE,
                                   FRONT,
                                   BACK,
                                   FRONT_AND_BACK
                            };

                            enum FrontFace
                            {
                                   COUNTER_CLOCKWISE,
                                   CLOCKWISE
                            };

                            struct ColorBlendAttachment
                            {
                                   bool write_color;
                                   bool enable_blend;
                            };

                            struct CreateInfo
                            {
                                   /*add : enable stencil test*/
                                   std::uint32_t vertex_input_count;
                                   const VertexInputBinding *vertex_inputs;
                                   Shader vertex_shader, tesselation_control_shader, tesselation_evaluation_shader, geometry_shader, fragment_shader;
                                   Primitive primitive;
                                   Polygon polygon;
                                   Cull cull;
                                   FrontFace front_face;
                                   bool enable_depth_test;
                                   std::uint32_t color_blend_attachment_count;
                                   const ColorBlendAttachment *color_blend_attachments;
                                   PipLink pip_link;
                                   RenderPass render_pass;
                                   std::uint32_t subpass_index;
                            };

                            Pipeline create(SoftwareDevice software_device, const CreateInfo &create_info);

                            void create_mutiple(
                                SoftwareDevice software_device, const CreateInfo &create_infos, Pipeline *graphics_pipelines,
                                std::uint32_t graphics_pipeline_count);

                            void destroy(SoftwareDevice software_device, Pipeline graphics_pipeline);
                     }

                     namespace compute
                     {

                     }
              }

              namespace command
              {
                     struct CreateInfo
                     {
                            Queue queue;
                            bool secondary;
                     };

                     Command create(SoftwareDevice software_device, const CreateInfo &create_info);

                     void create_multiple(SoftwareDevice software_device, const CreateInfo &create_info, Command *commands, std::uint32_t command_count);

                     void destroy(SoftwareDevice software_device, Command command);

                     void destroy_multiple(SoftwareDevice software_device, Command *commands, std::uint32_t command_count);

                     void begin_record(SoftwareDevice software_device, Command command);

                     void end_record(SoftwareDevice software_device, Command command);

                     void submit(Command command);

                     void wait(Command command);

                     void reset(Command command);

                     union ClearColor
                     {
                            float f[4];
                            std::int32_t i[4];
                            std::uint32_t ui[4];
                     };

                     struct ClearDepthStencil
                     {
                            float depth;
                            uint32_t stencil;
                     };

                     union ClearValue
                     {
                            ClearColor color;
                            ClearDepthStencil depth_stencil;
                     };

                     void begin_render_pass(
                         Command command, RenderPass render_pass, Framebuffer framebuffer, const ClearValue *clear_values, std::uint32_t clear_value_count);

                     void end_render_pass(Command command);

                     void bind_pipeline(Command command, pipeline::Type pipeline_type, Pipeline pipeline);

                     void bind_uniform_set(Command command, pipeline::Type pipeline_type, UniformSet uniform_set);

                     /*'first_set_index' overrides set_index from uniform sets*/
                     //void bind_multiple_uniform_set(Command command, std::uint32_t first_set_index, UniformSet *uniform_sets, std::uint32_t uniform_set_count);

                     void bind_vertex_buffers(Command command, std::uint64_t *offsets, Buffer *buffers, std::uint32_t buffer_count);

                     void bind_index_buffer(Command command, std::uint64_t offset, bool is_uint32, Buffer buffer);

                     void draw_indexed(
                         Command command, std::uint32_t index_count, std::uint32_t instance_count, std::uint32_t firest_index, std::int32_t vertex_offset,
                         std::uint32_t first_instance);

                     void draw(Command command, std::uint32_t vertex_count, std::uint32_t instance_count, std::uint32_t first_vertex, std::uint32_t first_instance);

                     struct CopyBufferInfo
                     {
                            std::uint64_t src_offset;
                            std::uint64_t dst_offset;
                            std::uint64_t size;
                     };

                     struct CopyTextureInfo
                     {
                            std::uint32_t src_mip_level, src_first_layer, src_layer_count;
                            std::uint32_t src_offset_width, src_offset_height, src_offset_depth;
                            std::uint32_t dst_mip_level, dst_first_layer, dst_layer_count;
                            std::uint32_t dst_offset_width, dst_offset_height, dst_offset_depth;
                            std::uint32_t dst_width, dst_height, dst_depth;
                     };

                     struct CopyBufferTextureInfo
                     {
                            std::uint64_t buffer_offset;
                            std::uint32_t texture_mip_level, texture_first_layer, texture_layer_count;
                            std::uint32_t texture_offset_width, texture_offset_height, texture_offset_depth;
                            std::uint32_t texture_width, texture_height, texture_depth;
                     };

                     void copy_buffer_to_buffer(Command command, Buffer src_buffer, Buffer dst_buffer, const CopyBufferInfo *copy_infos, std::uint32_t copy_info_count);

                     void copy_texture_to_texture(
                         Command command, Texture src_texture, texture::Layout src_layout, Texture dst_texture, texture::Layout dst_layout,
                         const CopyTextureInfo *copy_infos, std::uint32_t copy_info_count);

                     void copy_texture_to_buffer(
                         Command command, Texture texture, texture::Layout layout, Buffer buffer, const CopyBufferTextureInfo *copy_infos,
                         std::uint32_t copy_info_count);

                     void copy_buffer_to_texture(
                         Command command, Buffer buffer, Texture texture, texture::Layout layout, const CopyBufferTextureInfo *copy_infos,
                         std::uint32_t copy_info_count);

                     struct TextureBarrier
                     {
                            pipeline::Access src_access, dst_access;
                            texture::Layout src_layout, dst_layout;
                            std::uint32_t first_mip_level, mip_level_count, first_layer, layer_count;
                            Texture texture;
                     };

                     void pipeline_barrier(
                         Command command, pipeline::Stage src_stage, pipeline::Stage dst_stage, const TextureBarrier *texture_barriers, std::uint32_t texture_barrier_count);
              }

              namespace ext
              {
                     namespace win_link
                     {
                            struct CreateInfo
                            {
                                   std::uint32_t width, height;
#ifdef NGR_LINUX_PLATFORM
                                   Display *x11_display;
                                   Window x11_window;
#endif //NGR_LINUX_PLATFORM
                            };

                            WinLink create(Driver driver, const CreateInfo &create_info);

                            void destroy(Driver driver, WinLink win_link);
                     }

                     namespace swapchain
                     {
                            struct CreateInfo
                            {
                                   WinLink win_link;
                            };

                            Swapchain create(SoftwareDevice software_device, const CreateInfo &create_info);

                            void destroy(SoftwareDevice software_device, Swapchain swapchain);

                            std::uint32_t get_texture_count(Swapchain swapchain);

                            Texture get_texture(Swapchain swapchain, std::uint32_t index);

                            void render_frame(SoftwareDevice software_device, Command *render_commands, std::uint32_t render_command_count);
                     }
              }
       }
}

#endif //__NEXT_GEN_RENDERING_HPP__