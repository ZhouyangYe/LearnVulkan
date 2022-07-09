rootFolder = {}
rootFolder["GLFW"] = "glfw"
rootFolder["FMT"] = "fmt"
rootFolder["VKB"] = "vk-bootstrap"

-- GLFW
project "GLFW"
	kind "StaticLib"
	language "C"
  location "%{wks.location}/%{prj.name}"
	targetdir ("%{wks.location}/dist/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/dist/obj/" .. outputdir .. "/%{prj.name}")

	files {
		rootFolder["GLFW"] .. "/include/GLFW/glfw3.h",
		rootFolder["GLFW"] .. "/include/GLFW/glfw3native.h",
		rootFolder["GLFW"] .. "/src/glfw_config.h",
		rootFolder["GLFW"] .. "/src/context.c",
		rootFolder["GLFW"] .. "/src/init.c",
		rootFolder["GLFW"] .. "/src/input.c",
		rootFolder["GLFW"] .. "/src/monitor.c",
		rootFolder["GLFW"] .. "/src/vulkan.c",
		rootFolder["GLFW"] .. "/src/window.c",

		rootFolder["GLFW"] .. "/src/internal.h",
		rootFolder["GLFW"] .. "/src/mappings.h",
		rootFolder["GLFW"] .. "/src/platform.h",

		rootFolder["GLFW"] .. "/src/platform.c",
		rootFolder["GLFW"] .. "/src/egl_context.c",
		rootFolder["GLFW"] .. "/src/osmesa_context.c",
		rootFolder["GLFW"] .. "/src/null_platform.h",
		rootFolder["GLFW"] .. "/src/null_joystick.h",
		rootFolder["GLFW"] .. "/src/null_init.c",

		rootFolder["GLFW"] .. "/src/null_monitor.c",
		rootFolder["GLFW"] .. "/src/null_window.c",
		rootFolder["GLFW"] .. "/src/null_joystick.c",
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{
			rootFolder["GLFW"] .. "/src/x11_init.c",
			rootFolder["GLFW"] .. "/src/x11_monitor.c",
			rootFolder["GLFW"] .. "/src/x11_window.c",
			rootFolder["GLFW"] .. "/src/xkb_unicode.c",
			rootFolder["GLFW"] .. "/src/posix_time.c",
			rootFolder["GLFW"] .. "/src/posix_thread.c",
			rootFolder["GLFW"] .. "/src/glx_context.c",
			rootFolder["GLFW"] .. "/src/egl_context.c",
			rootFolder["GLFW"] .. "/src/osmesa_context.c",
			rootFolder["GLFW"] .. "/src/linux_joystick.c"
		}

		defines	{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files {
			rootFolder["GLFW"] .. "/src/win32_init.c",
			rootFolder["GLFW"] .. "/src/win32_module.c",
			rootFolder["GLFW"] .. "/src/win32_joystick.c",
			rootFolder["GLFW"] .. "/src/win32_monitor.c",
			rootFolder["GLFW"] .. "/src/win32_time.h",
			rootFolder["GLFW"] .. "/src/win32_time.c",
			rootFolder["GLFW"] .. "/src/win32_thread.h",
			rootFolder["GLFW"] .. "/src/win32_thread.c",
			rootFolder["GLFW"] .. "/src/win32_window.c",
			rootFolder["GLFW"] .. "/src/wgl_context.c",
			rootFolder["GLFW"] .. "/src/egl_context.c",
			rootFolder["GLFW"] .. "/src/osmesa_context.c"
		}

		defines { 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

-- FMT
project "fmt"
  kind "StaticLib"
  language "C++"
  location "%{wks.location}/%{prj.name}"
  targetdir ("%{wks.location}/dist/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/dist/obj/" .. outputdir .. "/%{prj.name}")

  files {
    rootFolder["FMT"] .. "/include/core.h",
    rootFolder["FMT"] .. "/include/format-inl.h",
    rootFolder["FMT"] .. "/include/format.h",
    rootFolder["FMT"] .. "/src/format.cc",
  }

  includedirs {
    rootFolder["FMT"] .. "/include"
  }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

    defines {
      "_CRT_SECURE_NO_WARNINGS"
    }

  filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    runtime "Release"
    optimize "on"

--vk-bootstrap
project "vk-bootstrap"
kind "StaticLib"
language "C++"
location "%{wks.location}/%{prj.name}"
targetdir ("%{wks.location}/dist/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/dist/obj/" .. outputdir .. "/%{prj.name}")

files {
  rootFolder["VKB"] .. "/src/VkBootstrap.h",
  rootFolder["VKB"] .. "/src/VkBootstrap.cpp",
}

includedirs {
  rootFolder["VKB"] .. "/src"
}

filter "system:windows"
  systemversion "latest"
  staticruntime "On"

  defines {
    "_CRT_SECURE_NO_WARNINGS"
  }

filter "configurations:Debug"
  runtime "Debug"
  symbols "on"

filter "configurations:Release"
  runtime "Release"
  optimize "on"
