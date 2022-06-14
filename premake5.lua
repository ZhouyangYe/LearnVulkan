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

IncludeDirs = {}
IncludeDirs["GLFW"] = "vendors/GLFW"

include "vendors/"

project "LearnVulkan"
  location "%{wks.location}/%{prj.name}"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

  files { "src/**.h", "src/**.cpp" }

  includedirs {
		"src",
		"%{IncludeDirs.GLFW}/include",
		"vendors/glm",
		-- "vendors/stb_image"
	}

  links {
		"GLFW",
	}
  
	filter "system:windows"
    cppdialect "c++17"
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
