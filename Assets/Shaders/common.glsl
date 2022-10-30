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
    vec3 Position;
    vec3 Direction;
    float InnerAngle;
    float OuterAngle;
    float FallOff;
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

float ComputeSpotLightEffect(in LightData light, vec3 lightVector)
{
    vec3 spotLightVector=normalize(light.Direction);
    vec3 fromLightToVertex=-lightVector;

    float cos_Angle=dot(spotLightVector,fromLightToVertex);
    float cos_Outer=cos(light.OuterAngle);
    float cos_Inner=cos(light.InnerAngle);
    if(cos_Angle<cos_Outer)
        return 0.f;//no effect
    if(cos_Angle>cos_Inner)
        return 1.f;//max effect
    
    return pow((cos_Angle-cos_Outer)/(cos_Inner-cos_Outer), light.FallOff);
}

vec3 ComputeFog(in FogData fog,vec3 color, float viewDistance)
{
    float s=max(fog.Far-viewDistance,0.0001f)/max(fog.Far-fog.Near,0.0000001f);
    return s*color+(1-s)*fog.Color;
}

vec3 ComputePointLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
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

vec3 ComputeDirectionLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=normalize(-light.Direction);

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*vec3(1.f, 1.f, 1.f);
	vec3 diffuse = material.Diffuse*vec3(1.f, 1.f, 1.f)*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*vec3(1.f, 1.f, 1.f)*pow(max(dot(reflection,viewVector),0.f), material.Shininess);
    return ambient+diffuse+specular;
}

vec3 ComputeSpotLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*vec3(1.f, 1.f, 1.f);
	vec3 diffuse = material.Diffuse*vec3(1.f, 1.f, 1.f)*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*vec3(1.f, 1.f, 1.f)*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    float spotLightEffect=ComputeSpotLightEffect(light, lightVector);
    return att*(ambient+spotLightEffect*(diffuse+specular));
}
