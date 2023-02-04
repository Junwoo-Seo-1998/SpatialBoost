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
    startproject "RenderingEngine"
include "Libs/glfw"
include "Libs/glad"
include "Libs/imgui"
IncludeDir={}
IncludeDir["GLFW"]="Libs/glfw/glfw/include/"
IncludeDir["GLAD"]="Libs/glad/"
IncludeDir["GLM"]="Libs/glm/"
IncludeDir["ENTT"]="Libs/entt/"
IncludeDir["IMGUI"]="Libs/imgui/"
IncludeDir["STB_IMAGE"]="Libs/stb_image/"
IncludeDir["ASSIMP"]="Libs/assimp/include/"

Library = {}
Library["DASSIMP"]="assimp-vc143-mtd.lib"
Library["ASSIMP"]="assimp-vc143-mt.lib"
LibraryDir = {}
LibraryDir["DASSIMP"]="Libs/assimp/dbg"
LibraryDir["ASSIMP"]="Libs/assimp/rel"

Shared = {}
Shared["DASSIMP"]="../Libs/assimp/dbg/assimp-vc143-mtd.dll"
Shared["ASSIMP"]="../Libs/assimp/rel/assimp-vc143-mt.dll"

project "RenderingEngine"
    location "RenderingEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/%{cfg.buildcfg}-obj"
    architecture "x86_64"
    links {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui",
    }
    
    disablewarnings { "4819", "4002", "4005","6031", "4267", "26498", "26819", "28020", "26439" }
    linkoptions { "-IGNORE:4075","-IGNORE:4098"}
    includedirs
    {
        "RenderingEngine",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.ASSIMP}"
    }
    files {
        "RenderingEngine/**.h",
        "RenderingEngine/**.cpp"
    }
    defines{
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    postbuildcommands {
        "{COPYDIR} \"../Assets\" \"../bin/%{cfg.buildcfg}/Assets\"",
        "{COPYDIR} \"../Assets\" \"../RenderingEngine/Assets\"",
        '{COPYFILE} "../Settings/imgui.ini" ../bin/%{cfg.buildcfg}/imgui.ini',
    }


    -- All of these settings will appear in the Debug configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        debugdir "bin/%{cfg.buildcfg}"
        libdirs 
        {
            "%{LibraryDir.DASSIMP}"
        }

        links {
            "%{Library.DASSIMP}"
        }
        postbuildcommands 
        {
            '{COPYFILE} %{Shared.DASSIMP} ../bin/%{cfg.buildcfg}/assimp-vc143-mtd.dll',
        }
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        debugdir "bin/%{cfg.buildcfg}"
        libdirs 
        {
            "%{LibraryDir.ASSIMP}"
        }
        links {
            "%{Library.ASSIMP}"
        }
        postbuildcommands 
        {
            '{COPYFILE} %{Shared.ASSIMP} ../bin/%{cfg.buildcfg}/assimp-vc143-mt.dll',
        }
