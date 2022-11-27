# CS300
#Name - Junwoo Seo
#Digipen ID - junwoo.seo 
#Student Num - 0055213

#How to use parts of your user interface that is NOT specified in the assignment description.
It will be pretty straightforward to use it. since i made guis based on TA's Demo.

#Any assumption that you make on how to use the application that, if violated, might cause
the application to fail.
- For now, we need hardware that supports OpenGL version 4.6 and Windows Only. (using vs2022)

#Which part of the assignment has been completed? 
- I believe I completed all requirements.

#Which part of the assignment has NOT been completed (not done, not working, etc.) and
explanation on why those parts are not completed?
- I believe I completed all requirements.

#Where the relevant source codes (both C++ and shaders) for the assignment are located.
#Specify the file path (folder name), file name, and function name (or line number).

in Assets/Shaders folder there are shader sources for vertex and fragment shaders to draw Phong shaders and blinn shaders.
in RenderingEngine/Core/Graphics Folder for graphics stuff like FrameBuffers.
in RenderingEngine/Core/Data Folder for graphics data types.
For Shader files (Assets/Shaders/*.*)
For Construct the 6 texture maps for cube mapping algorithm see AssetManager GenerateSkybox function and Each scene update function parts(Core/AssetManager.cpp)
For Scene setups Scene/*.cpp and Scene/*.h
For FrameBuffer Update see UpdateFrameBuffers and DrawEnv functions in scenario files.
For Implement the environment mapping algorithm in the fragment shader see (Assets/Shaders/common.glsl and Assets/Shaders/*.frag)

#Which machine did you test your application on. 
The application was tested on my machine.
OS: Windows11 Pro
GPU: NVIDIA GeForce GTX 1050/PCIe/
OpenGL Version: 4.6 NVIDIA 516.93

#The average number of hours you spent on the assignment, on a weekly basis
I spent 10 hours per week.

#Any other useful information pertaining to the application 
The default visual studio solution is for vs2022. however, you can make solutions for vs2019 and vs2022. 
I made a script that generates it with premake5.