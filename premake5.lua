-- premake5.lua
workspace "LearnVulkan"
  architecture "x64"
  startproject "LearnVulkan"
  location "build"
  configurations { "Debug", "Release" }
  flags {
    "MultiProcessorCompile"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VendorDirs = {}
VendorDirs["GLFW"] = "vendors/glfw"
VendorDirs["FMT"] = "vendors/fmt"
VendorDirs["GLM"] = "vendors/glm"
VendorDirs["STB"] = "vendors/stb"
VendorDirs["TOL"] = "vendors/tiny_obj_loader"
VendorDirs["VKB"] = "vendors/vk-bootstrap"
VendorDirs["VMA"] = "vendors/VulkanMemoryAllocator"
VendorDirs["LOG"] = "vendors/spdlog"

include "vendors/"

project "LearnVulkan"
  location "%{wks.location}/%{prj.name}"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir ("%{wks.location}/dist/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/dist/obj/" .. outputdir .. "/%{prj.name}")

  files {
    "src/**.h",
    "src/**.cpp",
    "%{VendorDirs.VKB}/src/VkBootstrap.h",
    "%{VendorDirs.VKB}/src/VkBootstrap.cpp",
  }

  includedirs {
		"src",
    "%{VendorDirs.LOG}/include",
		"%{VendorDirs.GLFW}/include",
		"%{VendorDirs.FMT}/include",
		"%{VendorDirs.GLM}",
		"%{VendorDirs.STB}",
		"%{VendorDirs.TOL}",
		"%{VendorDirs.VKB}/src",
		"%{VendorDirs.VMA}/include"
	}

  links {
		"GLFW",
    "FMT",
	}
  
	filter "system:windows"
    cppdialect "c++20"
    staticruntime "On"
    systemversion "latest"
    links
    {
      "vulkan-1",
    }
    libdirs
    {
      "C:/VulkanSDK/1.3.211.0/Lib",
    }
    sysincludedirs
    {
      "C:/VulkanSDK/1.3.211.0/Include",
    }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
