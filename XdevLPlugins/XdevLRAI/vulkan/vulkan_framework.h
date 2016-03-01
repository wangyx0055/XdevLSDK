/*
	Vulkan Framework Copyright (c) 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

*/

#ifndef VULKAN_FRAMEWORK_H
#define VULKAN_FRAMEWORK_H

#include <string>
#include <vector>
#include <iostream>

namespace vkf {

  const char* vkVkResultToString(VkResult result) {
    switch(result) {
      case VK_SUCCESS:
        return "VK_SUCCESS";
      case VK_NOT_READY:
        return "VK_NOT_READY";
      case VK_TIMEOUT:
        return "VK_TIMEOUT";
      case VK_EVENT_SET:
        return "VK_EVENT_SET";
      case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
      case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
      case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
      case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
      case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
      case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
      case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
      case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
      case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
      case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
      case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
      case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
      case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
      case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
      case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
      case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
      case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
      case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
      case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
      case VK_RESULT_RANGE_SIZE:
        return "VK_RESULT_RANGE_SIZE";
      default:
        break;
    }
    return "VK_RESULT_MAX_ENUM";
  }

  class VulkanContext {
    public:
      VulkanContext() :
        vulkanInstance(nullptr),
        physicalDevice(nullptr),
        device(nullptr) {

      }
      virtual ~VulkanContext() {
        shutdown();
      }

      /// Initialize Vulkan.
      int initialize(const char* appName="VulkanApp", uint32_t appVersion = 0) {
        //
        // Let's first create a application info which is mandatory.
        //
        VkApplicationInfo appInfo = {
          VK_STRUCTURE_TYPE_APPLICATION_INFO, // Mandatory.
          nullptr,                            // Mandatory.
          appName,                            // Null terminated string contains the name of the app.
          appVersion,                         // Version of the app.
          nullptr,                            // Null terminated string contains the name of the engine.
          0,                                  // Version of the engine.
          VK_MAKE_VERSION(1, 0, 0)            // The version of the Vulkan API against which the application expects to run.
        };

        //
        // Let's create the instance info which is mandatory.
        //

        // Let's prepare the extension stuff. We will use the surface extension to handle native windows.
        std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
        enabledExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

        VkInstanceCreateInfo instanceCreateInfo = {
          VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // Mandatory.
          nullptr,                                // Mandatory.
          0,                                      // Reserved for future use.
          &appInfo,                               // Pointer to a valid VkApplicationInfo structure.
          0,                                      // Number of global layers to enable.
          nullptr,                                // A pointer of array of null terminated strings containing the layer names.
          (uint32_t)enabledExtensions.size(),     // Number of global extensions to enable.
          enabledExtensions.data()                // A pointer of array of null terminated strings containing the extensions names.
        };

        //
        // Create the vulkan instance.
        //
        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance);
        if(VK_SUCCESS != result) {
          std::cerr << "vkCreateInstance failed: " << vkf::vkVkResultToString(result) << std::endl;
          return 1;
        }

        //
        // Get first the number of physical devices.
        //
        uint32_t deviceCount = 0;
        result = vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
        if(result != VK_SUCCESS) {
          std::cerr << "vkEnumeratePhysicalDevices failed: " << vkf::vkVkResultToString(result) << std::endl;
          vkDestroyInstance(vulkanInstance, nullptr);
          return 1;
        }

        //
        // Do the same but this time get the infos for the physical devices too.
        //
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        result = vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());
        if(result != VK_SUCCESS) {
          std::cerr << "vkEnumeratePhysicalDevices failed: " << vkf::vkVkResultToString(result) << std::endl;
          vkDestroyInstance(vulkanInstance, nullptr);
          return 1;
        }

        //
        // Print out some info about the physical devices.
        //
        physicalDevice = physicalDevices[0];
        for(auto device : physicalDevices) {
          VkPhysicalDeviceProperties deviceProperties;
          vkGetPhysicalDeviceProperties(device, &deviceProperties);
          std::cout << "Vulkan API Version : " <<
                    ((deviceProperties.apiVersion>>22)&0x3FF) << "." <<
                    ((deviceProperties.apiVersion>>12)&0x3FF) << "." <<
                    ((deviceProperties.apiVersion&0x3FF)) << std::endl;
          std::cout << "Driver Version     : " << deviceProperties.driverVersion << std::endl;
          std::cout << "Device Name        : " << deviceProperties.deviceName << std::endl;
          std::cout << "Device Type        : " << deviceProperties.deviceType << std::endl;

          uint32_t queueFamilyCount = 0;
          vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
          std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyCount);
          vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, familyProperties.data());

          for(uint32_t i = 0; i < deviceCount; i++) {
            for(uint32_t j = 0; j < queueFamilyCount; j++) {
              std::cout << "Number of Queues: " << familyProperties[j].queueCount << std::endl;
              std::cout << "Supported operationg on this queue: " << std::endl;
              if(familyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                printf("\t\t Graphics\n");
              if(familyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT)
                printf("\t\t Compute\n");
              if(familyProperties[j].queueFlags & VK_QUEUE_TRANSFER_BIT)
                printf("\t\t Transfer\n");
              if(familyProperties[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                printf("\t\t Sparse Binding\n");
            }
          }
        }

        //
        // Here we will create the real physical device connection.
        //
        VkDeviceCreateInfo deviceInfo;

        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;  // Again, spec says this must be like this.
        deviceInfo.pNext = nullptr;                               // This is for now nullptr.
        deviceInfo.flags = 0;                                     // Spec says this must be nullptr.

        // We don't do anything with layer or extensions here.
        deviceInfo.enabledLayerCount = 0;
        deviceInfo.ppEnabledLayerNames = nullptr;
        deviceInfo.enabledExtensionCount = 0;
        deviceInfo.ppEnabledExtensionNames = nullptr;

        // Here's where we initialize our queues
        VkDeviceQueueCreateInfo deviceQueueInfo;
        deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueInfo.pNext = nullptr;
        deviceQueueInfo.flags = 0;

        // Use the first queue family in the family list
        deviceQueueInfo.queueFamilyIndex = 0;

        // Create only one queue
        float queuePriorities[] = { 1.0f };
        deviceQueueInfo.queueCount = 1;
        deviceQueueInfo.pQueuePriorities = queuePriorities;

        // Set queue(s) into the device
        deviceInfo.queueCreateInfoCount = 1;
        deviceInfo.pQueueCreateInfos = &deviceQueueInfo;

        result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);
        if(result != VK_SUCCESS) {
          std::cerr << "vkCreateDevice failed: " << vkf::vkVkResultToString(result) << std::endl;
          vkDestroyInstance(vulkanInstance, nullptr);
          return 1;
        }

        return 0;
      }

      /// Shutdown the system.
      void shutdown() {
        if(nullptr != device) {
          vkDestroyDevice(device, nullptr);
        }
        if(nullptr != vulkanInstance) {
          vkDestroyInstance(vulkanInstance, nullptr);
        }
      }

    private:
      VkInstance vulkanInstance;
      VkPhysicalDevice physicalDevice;
      VkDevice device;
  };
}

#endif
