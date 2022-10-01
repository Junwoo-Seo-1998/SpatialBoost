# CS300
#Name - Junwoo Seo
#Digipen ID - junwoo.seo 
#Student Num - 0055213

#How to use parts of your user interface that is NOT specified in the assignment description.
It will be pretty straightforward to use it. for the mesh part, you can select which mesh to render. the meshes will be the followings.
4Sphere - Assets/4Sphere.obj
bunny - Assets/bunny_high_poly.obj
cube - Assets/cube2.obj
sphere - Assets/sphere.obj
sphere_modified - Assets/sphere_modified.obj
triangle - Assets/triangle.obj
GeneratedSphere - Procedurally created sphere.
and there are options for culling back face and drawing normal lines.
also, there are options for drawing with vertex normal and drawing with face normal.
change color options that can change the color of mesh and lines.

#Any assumption that you make on how to use the application that, if violated, might cause
the application to fail.
- For now, we need hardware that supports OpenGL version 4.6 and Windows Only. (using vs2022)

#Which part of the assignment has been completed? 
- I believe I completed all requirements.
#1. Scene Setup - done
{
	Be able to load an OBJ file from disk - done

	Apply appropriate transform to scale the model to the range [-1,1] - done

	Center the object at the origin of the [-1,1] box - done

	Render 8 spheres, equally spaced on a circle around the object. The diameter of the
	spheres should be 0.2 units. Keep the spheres at a sufficient distance (2.0-3.0 units)
	away from the model - done

	Central object (OBJ file – drawn using GL_TRIANGLES) - done

	Spheres (Generate procedurally – drawn using GL_TRIANGLES) - done

	The orbit of the spheres (drawn using line segments – GL_LINES) - done
}
#2. Geometry Operations -done 
{
	OBJ importer is able to read the following files
	(4sphere, bunny, cube, sphere, and sphere_modified) - done
	
	For face records with more than 3 vertices, use the “triangle-fan” approach - done
	
	Calculate the per-vertex normal from the face normal (accounts for corner-cases, Ignore normals that are parallel) - done
	
	Display face and vertex normals (Use GL_LINES to draw the normal, place it at the centroid of the triangle) - done
	
	Your program should provide a menu-driven functionality to toggle between displaying the vertex normal, the face normal, or none. - done
}
#3. The rendering function was completely implemented by me - done
#4. Rendering must be done using OpenGL rendering functions as discussed in class - done
#5. The ambient and diffuse lighting - done
#6. Procedurally creating a sphere - done


#Which part of the assignment has NOT been completed (not done, not working, etc.) and
explanation on why those parts are not completed?
- I believe I completed all requirements.

#Where the relevant source codes (both C++ and shaders) for the assignment are located.
#Specify the file path (folder name), file name, and function name (or line number).
in Assets/Shaders folder there are shader sources for vertex and fragment shaders to draw Phong light and normal lines.
in RenderingEngine/Core/Graphics Folder
Shader.h, Shader.cpp for Shaders (Use program, set uniform, compile, and creation of shader functionalities are in here)
Buffer.h, Buffer.cpp for Vertex Buffer Object (Bind, BufferData, CreateBuffer functionalities are in here)
VertexArray.h, VertexArray.cpp for Vertex Array Object (bind and attibpointer functionalities are in here)

###shader, obj loading and Generate Sphere,Orbit ###
in RenderingEngine/Core Folder
AssetManager.h, AssetManager.cpp This will manage all data that used in my application and save the data in std::map (Shader, Loaded obj Meshs,Generated Meshs)
AssetManager will invoke MeshGenerator functions in order to reuse generated and parsed data from the function and do best to reuse those.
in RenderingEngine/Core/Utils Folder
MeshGenerator.h, MeshGenerator.cpp  This has all functions of making mesh(GenerateSpherePointsWithIndices, GenerateFaceNormalMesh, GenerateFaceNormalLineMesh, GenerateVertexNormalMesh, GenerateVertexNormalLineMesh
, GenerateFaceNormals, GenerateVertexNormals, GenerateOrbit)
MeshGenerator will use parsed data when it generates meshs.
the parsed data come from ObjParser class.
in RenderingEngine/Core/Utils Folder 
ObjParser.h, ObjParser.cpp will parse data from obj file. it will make scale to [-1,1] and center to origin with triangle-fan approach on load.

###easy look up list###
Be able to load an OBJ file from disk - description above(###shader, obj loading and Generate Sphere,Orbit ###, if it means only parse then look ParseFile in ObjParser.h, ObjParser.cpp)

Apply appropriate transform to scale the model to the range [-1,1] - ObjParser.h, ObjParser.cpp in ParseFile function

Center the object at the origin of the [-1,1] box - ObjParser.h, ObjParser.cpp in ParseFile function

Render 8 spheres, equally spaced on a circle around the object. The diameter of the
spheres should be 0.2 units. Keep the spheres at a sufficient distance (2.0-3.0 units)
away from the model - for sphere it uses generated sphere and orbit so, look MeshGenerator::GenerateSpherePointsWithIndices, MeshGenerator::GenerateOrbit function(in RenderingEngine/Core/Utils/MeshGenerator.*) and
in the main.cpp i do parenting of transform to make it rotate around with distance.

Central object (OBJ file – drawn using GL_TRIANGLES) - it uses all the functions in MeshGenerator and Loaded with using AssetManager and ObjParser. and for drawing part there is abstracted class that it does
so you will have to look DrawType and Mesh class (in RenderingEngine/Core/Data/Mesh.*),RendererComponent classes(in RenderingEngine/Core/Component/RendererComponent.*), Update function at main.cpp(draw call), Shader.*,Buffer.* and VertexArray.* files.

Spheres (Generate procedurally – drawn using GL_TRIANGLES) - main procedural generate function is in MeshGenerator::GenerateSpherePointsWithIndices (in RenderingEngine/Core/Utils/MeshGenerator.*) and the generated points and indices will be passed
to Generate Mesh functions to draw by AssetManager.

The orbit of the spheres (drawn using line segments – GL_LINES) - it generated by MeshGenerator::GenerateOrbit function.

OBJ importer is able to read the following files
(4sphere, bunny, cube, sphere, and sphere_modified) - explanation at ###shader, obj loading and Generate Sphere,Orbit ###

For face records with more than 3 vertices, use the “triangle-fan” approach - look ObjParser::ParseFile function (RenderingEngine/Core/Utils/ObjParser.*)

Calculate the per-vertex normal from the face normal (accounts for corner-cases, Ignore normals that are parallel) - look MeshGenerator::GenerateVertexNormals and MeshGenerator::GenerateFaceNormals function. (RenderingEngine/Core/Utils/MeshGenerator.*)

Display face and vertex normals (Use GL_LINES to draw the normal, place it at the centroid of the triangle) - look MeshGenerator::GenerateVertexNormalLineMesh and MeshGenerator::GenerateFaceNormalLineMesh to see how it's created and it will be 
drawn at the end of Update. by collecting render Components with drawing types and draw.

Your program should provide a menu-driven functionality to toggle between displaying the vertex normal, the face normal, or none. - you can find imgui implementation for it at LateUpdate function in main.cpp

The ambient and diffuse lighting - in shader file (Assets/Shaders/light.*)

Procedurally creating a sphere - i noticed that naive method that you shown in pdf will make a lot of duplicated vertices(top, bottom). so i managed it by not making duplicated vertices. 
(look MeshGenerator::GenerateSpherePointsWithIndices in RenderingEngine/Core/Utils/MeshGenerator.*)



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