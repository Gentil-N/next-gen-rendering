#include "core.hpp"

namespace ngr
{
       namespace api
       {
              namespace queue
              {
                     Queue graphics(SoftwareDevice software_device)
                     {
                            return software_device->graphics_queue;
                     }

                     Queue compute(SoftwareDevice software_device)
                     {
                            return software_device->compute_queue;
                     }

                     Queue transfer(SoftwareDevice software_device)
                     {
                            return software_device->transfer_queue;
                     }

                     void wait(SoftwareDevice software_device, Queue queue)
                     {
                            software_device->func.vk_queue_wait_idle(queue->vk_queue);
                     }
              }
       }
}