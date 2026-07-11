# VulkanSandbox

An immediate mode 2D Vulkan renderer written in c++

## Requirements

Vulkan 1.3

## Build

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

If CMake cannot find Vulkan, install the Vulkan SDK and ensure its libraries are available on your system path.
