#version 460 core
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: light.frag
Purpose: shader source for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/

struct LightData
{
    vec3 PosOrDir;
};

uniform LightData light;

in vec3 Normal;
in vec3 FragPos;
uniform vec4 BaseColor;
out vec4 FragColor;
void main()
{
	vec3 LightDir=normalize(light.PosOrDir-FragPos);
	float diff=max(dot(Normal,LightDir),0.f);
	vec4 diffuse=diff*vec4(1.f, 1.f, 1.f, 1.f);
			
	float ambientStrength=0.1;
	vec4 ambient = ambientStrength * vec4(1.f, 1.f, 1.f, 1.f);
	FragColor = (ambient+diffuse)*BaseColor;
} 