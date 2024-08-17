cmake_minimum_required(VERSION 3.2)
project(imgui CXX)

add_library(imgui STATIC 
	"imgui.cpp"
	"imgui_draw.cpp"
    "imgui_tables.cpp"
	"imgui_widgets.cpp"
	"imgui_demo.cpp"
)
set_property(TARGET imgui PROPERTY CXX_STANDARD 17)