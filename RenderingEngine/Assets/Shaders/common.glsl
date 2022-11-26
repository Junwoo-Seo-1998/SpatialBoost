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
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
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

struct BoundingBoxData
{
    vec3 Min;
    vec3 Max;
    vec3 Center;
};

vec3 ComputeReflection(vec3 normalVector, vec3 lightVector)
{
    return 2*(dot(normalVector,lightVector))*normalVector-lightVector;
}
vec3 ComputeHalfVector(vec3 viewVector, vec3 lightVector)
{
    return normalize(viewVector+lightVector);
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
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    return att*(ambient+diffuse+specular);
}

vec3 ComputeDirectionLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=normalize(-light.Direction);

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);
    return ambient+diffuse+specular;
}

vec3 ComputeSpotLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    float spotLightEffect=ComputeSpotLightEffect(light, lightVector);
    return att*(ambient+spotLightEffect*(diffuse+specular));
}





vec3 ComputeBlinnPointLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeHalfVector(viewVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    return att*(ambient+diffuse+specular);
}

vec3 ComputeBlinnDirectionLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=normalize(-light.Direction);

	vec3 reflection=ComputeHalfVector(viewVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);
    return ambient+diffuse+specular;
}

vec3 ComputeBlinnSpotLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeHalfVector(viewVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),0.f), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    float spotLightEffect=ComputeSpotLightEffect(light, lightVector);
    return att*(ambient+spotLightEffect*(diffuse+specular));
}







vec2 ComputePlanarUV(vec3 TextureEntity, float min_x, float max_x, float min_y, float max_y)
{
    float u=(TextureEntity.x-min_x)/(max_x-min_x);
    float v=(TextureEntity.y-min_y)/(max_y-min_y);

    return vec2(u,v);
}



vec2 ComputeCylindricalUV(vec3 TextureEntity, float min_y, float max_y)
{
    float theta = degrees(atan(TextureEntity.x, TextureEntity.z)); 
    //since atan will return [-180, 180] and i want to map this to [0,360]
    theta += 180.f; 
    float y = TextureEntity.y; 
    return vec2(theta / 360.f, (y - min_y) / (max_y - min_y));
}

vec2 ComputeSphericalUV(vec3 TextureEntity)
{
    float theta = degrees(atan(TextureEntity.x, TextureEntity.z)); 
    //since atan will return [-180, 180] and i want to map this to [0,360]
    theta += 180.f; 

    float pi = 180.f - degrees(acos(TextureEntity.y / length(TextureEntity)));

    return vec2(theta / 360.f, pi / 180.f);
}

vec2 ComputeCubeMapUV(vec3 TextureEntity)
{
    float u=0.f;
    float v=0.f;

    vec3 Abs=abs(TextureEntity);

    //-+x
    if(Abs.x >= Abs.y && Abs.x >= Abs.z)
    {
        //left or right
        u=(TextureEntity.x < 0.f) ? TextureEntity.z : -TextureEntity.z;
        u=u/Abs.x;
        v=TextureEntity.y/Abs.x;
    }

    //-+z
    if(Abs.z >= Abs.x && Abs.z >= Abs.y)
    {
        //back or front
        u=(TextureEntity.z < 0.f) ? -TextureEntity.x : TextureEntity.x;
        u=u/Abs.z;
        v=TextureEntity.y/Abs.z;
    }

    //-+y
    if(Abs.y >= Abs.x && Abs.y >= Abs.z)
    {
        u=TextureEntity.x/Abs.y;

        //bottom or top
        v=(TextureEntity.y < 0.f) ? TextureEntity.z : -TextureEntity.z;
        v=v/Abs.y;
    }

    //-1 to 1 to 0 to 1
    u = 0.5f * (u + 1.0f);
    v = 0.5f * (v + 1.0f);
    return vec2(u,v);
}

vec2 ComputeCubeMapUV(vec3 TextureEntity, out int index)
{
    index=0;
    float u=0.f;
    float v=0.f;
    vec3 Abs=abs(TextureEntity);

    //-+x
    if(Abs.x >= Abs.y && Abs.x >= Abs.z)
    {
        //left or right
        if(TextureEntity.x < 0.f)
        {
            u=TextureEntity.z;
            index=0;//left
        }
        else
        {
            u=-TextureEntity.z;
            index=1;//right
        } 
        u=u/Abs.x;
        v=TextureEntity.y/Abs.x;
    }

    //-+z
    if(Abs.z >= Abs.x && Abs.z >= Abs.y)
    {
        //back or front
        if(TextureEntity.z < 0.f)
        {
            u=-TextureEntity.x;
            index=2;//front
        }
        else
        {
            u=TextureEntity.x;
            index=3;//back
        }
        u=u/Abs.z;
        v=TextureEntity.y/Abs.z;
    }

    //-+y
    if(Abs.y >= Abs.x && Abs.y >= Abs.z)
    {
        u=TextureEntity.x/Abs.y;

        //bottom or top
        if(TextureEntity.y < 0.f)
        {
            v=TextureEntity.z;
            index=4;//bottom
        }
        else
        {
            v=-TextureEntity.z;
            index=5;//top
        }
        v=v/Abs.y;
    }

    //-1 to 1 to 0 to 1
    u = 0.5f * (u + 1.0f);
    v = 0.5f * (v + 1.0f);
    return vec2(u,v);
}