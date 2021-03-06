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

#include <stdgpu/memory.h>          // createDeviceArray, destroyDeviceArray, createHostArray, destroyHostArray
#include <stdgpu/platform.h>        // STDGPU_HOST_DEVICE



class Image
{
    public:
        Image() = default;

        static Image
        createDeviceObject(const stdgpu::index_t width,
                           const stdgpu::index_t height)
        {
            Image result;

            result._values = createDeviceArray<std::uint8_t>(width * height);
            result._width = width;
            result._height = height;

            return result;
        }

        static void
        destroyDeviceObject(Image& device_object)
        {
            destroyDeviceArray<std::uint8_t>(device_object._values);
            device_object._width = 0;
            device_object._height = 0;
        }

        static Image
        createHostObject(const stdgpu::index_t width,
                         const stdgpu::index_t height)
        {
            Image result;

            result._values = createHostArray<std::uint8_t>(width * height);
            result._width = width;
            result._height = height;

            return result;
        }

        static void
        destroyHostObject(Image& host_object)
        {
            destroyHostArray<std::uint8_t>(host_object._values);
            host_object._width = 0;
            host_object._height = 0;
        }

        // Further (static) member functions ...

        STDGPU_HOST_DEVICE std::uint8_t&
        operator()(const stdgpu::index_t x,
                   const stdgpu::index_t y)
        {
            return _values[y * _width + x];
        }

        STDGPU_HOST_DEVICE stdgpu::index_t
        width() const
        {
            return _width;
        }

        STDGPU_HOST_DEVICE stdgpu::index_t
        height() const
        {
            return _height;
        }

    private:
        std::uint8_t* _values = nullptr;
        stdgpu::index_t _width = 0;
        stdgpu::index_t _height = 0;
};


void
fill_image(Image image)
{
    for (stdgpu::index_t i = 0; i < image.width(); ++i)
    {
        for (stdgpu::index_t j = 0; j < image.height(); ++j)
        {
            std::uint8_t value = (i * i + j * j) % (1 << 8);

            image(i, j) = value;
        }
    }
}


int
main()
{
    Image image = Image::createHostObject(1920, 1080);

    fill_image(image);

    Image::destroyHostObject(image);
}


