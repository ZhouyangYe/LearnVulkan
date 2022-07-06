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
IncludeDirs["GLFW"] = "vendors/glfw"
IncludeDirs["FMT"] = "vendors/fmt"
IncludeDirs["GLM"] = "vendors/glm"
IncludeDirs["STB"] = "vendors/stb"
IncludeDirs["TOL"] = "vendors/tiny_obj_loader"

include "vendors/"

project "LearnVulkan"
  location "%{wks.location}/%{prj.name}"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir ("%{wks.location}/dist/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/dist/obj/" .. outputdir .. "/%{prj.name}")

  files { "src/**.h", "src/**.cpp" }

  includedirs {
		"src",
		"%{IncludeDirs.GLFW}/include",
		"%{IncludeDirs.FMT}/include",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.STB}",
		"%{IncludeDirs.TOL}",
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
