/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: light.vert
Purpose: shader source for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform MatrixData Matrix;
out vec3 Normal;
out vec3 FragPos;
void main()
{
	Normal=mat3(Matrix.Normal) * aNormal;
	FragPos=vec3(Matrix.Model*vec4(aPos,1.0));
	gl_Position = Matrix.Projection*Matrix.View*Matrix.Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}