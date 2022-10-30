#version 460 core
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: common.glsl
Purpose: shader source for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1, junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#define MAX_LIGHTS 16
struct LightData
{
    int LightType;
    vec3 PosOrDir;
};

struct AttenuationData
{
    float c1;
    float c2;
    float c3;
};

struct MatrixData
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
    mat4 Normal;
};

struct MaterialData
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
    vec3 Emissive;
};

struct FogData
{
    float Near;
    float Far;
    vec3 Color;
};

vec3 ComputeReflection(vec3 normalVector, vec3 lightVector)
{
    return 2*(dot(normalVector,lightVector))*normalVector-lightVector;
}

float ComputeAttenuation(in AttenuationData attenuation, float lightDistance)
{
    return min(1.f/(attenuation.c1+attenuation.c2*lightDistance+attenuation.c3*pow(lightDistance,2)),1.f);
}

vec3 CalcFog(in FogData fog,vec3 color, float viewDistance)
{
    float s=max(fog.Far-viewDistance,0.0001f)/max(fog.Far-fog.Near,0.0000001f);
    return s*color+(1-s)*fog.Color;
}

vec3 CalcPointLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.PosOrDir-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*vec3(1.f, 1.f, 1.f);
	vec3 diffuse = material.Diffuse*vec3(1.f, 1.f, 1.f)*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*vec3(1.f, 1.f, 1.f)*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    return att*(ambient+diffuse+specular);
}
