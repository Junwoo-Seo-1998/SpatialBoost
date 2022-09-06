--[[
Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: premake5.lua
Purpose: script to generate visual studio solution file.
Language: lua - used by premake5
Platform: premake5 Windows
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------
--]]
workspace "RenderingEngine"
    platforms {"Win64"}
    configurations  { "Debug", "Release" }    

include "Libs/glfw"
include "Libs/glad"
IncludeDir={}
IncludeDir["GLFW"]="Libs/glfw/glfw/include"
IncludeDir["GLAD"]="Libs/glad/"


project "RenderingEngine"
    location "RenderingEngine"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x86_64"
    links {
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }
    includedirs
    {
        
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}"
    }
    files {
        "RenderingEngine/**.h",
        "RenderingEngine/**.cpp"
    }
    defines{
        "GLFW_INCLUDE_NONE",
    }
    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
