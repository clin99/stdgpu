/*
 *  Copyright 2019 Patrick Stotko
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <algorithm>
#include <cstdio>
#include <cuda_runtime_api.h>
#include <string>



namespace detail
{
    float
    kilo_to_mega_hertz(const float kilo_hertz)
    {
        return kilo_hertz / 1000.0f;
    }

    float
    byte_to_kibi_byte(const float byte)
    {
        return byte / 1024.0f;
    }

    float
    byte_to_gibi_byte(const float byte)
    {
        return byte / (1024.0f * 1024.0f * 1024.0f);
    }
}


namespace stdgpu
{

namespace cuda
{

void
print_device_information()
{
    cudaDeviceProp properties;
    cudaGetDeviceProperties( &properties, 0 );

    size_t free_memory  = 0;
    size_t total_memory = 0;
    cudaMemGetInfo(&free_memory, &total_memory);

    std::string gpu_name = properties.name;
    int gpu_name_total_width = 57;
    int gpu_name_size = static_cast<int>(gpu_name.size());
    int gpu_name_space_left  = std::max<int>(1, (gpu_name_total_width - gpu_name_size) / 2);
    int gpu_name_space_right = std::max<int>(1, gpu_name_total_width - gpu_name_size - gpu_name_space_left);

    printf( "+---------------------------------------------------------+\n" );
    printf( "|%*s%*s%*s|\n", gpu_name_space_left, " ", gpu_name_size, gpu_name.c_str(), gpu_name_space_right, " ");
    printf( "+---------------------------------------------------------+\n" );
    printf( "| Compute Capability        :   %1d.%1d                       |\n", properties.major, properties.minor );
    printf( "| Clock rate                :   %-6.0f MHz                |\n", detail::kilo_to_mega_hertz(properties.clockRate));
    printf( "| Global Memory             :   %-6.3f GiB / %-6.3f GiB   |\n", detail::byte_to_gibi_byte(free_memory), detail::byte_to_gibi_byte(total_memory));
    printf( "| Memory Bus Width          :   %-6d Bit                |\n", properties.memoryBusWidth );
    printf( "| Multiprocessor (SM) count :   %-6d                    |\n", properties.multiProcessorCount );
    printf( "| Warp size                 :   %-6d Threads            |\n", properties.warpSize );
    printf( "| L2 Cache                  :   %-6.0f KiB                |\n", detail::byte_to_kibi_byte(properties.l2CacheSize));
    printf( "| Total Constant Memory     :   %-6.0f KiB                |\n", detail::byte_to_kibi_byte(properties.totalConstMem));
    printf( "| Shared Memory per SM      :   %-6.0f KiB                |\n", detail::byte_to_kibi_byte(properties.sharedMemPerMultiprocessor));
    printf( "| Total Shared Memory       :   %-6.0f KiB                |\n", detail::byte_to_kibi_byte(properties.sharedMemPerMultiprocessor * static_cast<size_t>(properties.multiProcessorCount)));
    printf( "+---------------------------------------------------------+\n\n" );
}

} // namespace cuda

} // namespace stdgpu


