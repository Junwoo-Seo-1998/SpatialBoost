/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: phong_shading.frag
Purpose: shader source for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1, junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/

in VS_OUT{ 
    vec3 Entity;
	vec3 FragPos;
	vec3 NormalVector;
    vec2 UV;
} fs_in; 


uniform LightData Light[MAX_LIGHTS];
uniform int LightNumbers;

uniform vec3 globalAmbient;
uniform AttenuationData Attenuation;

uniform vec3 CameraPosition;
uniform MaterialData Material;
uniform FogData Fog;
out vec4 FragColor;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
//6 sides left right front back bottom top 
uniform sampler2D skybox[6];


uniform bool useTexture;
uniform BoundingBoxData BoundingBox;

uniform int UVType;
uniform bool UseCPU;
uniform bool NormalEntity;

uniform bool showReflect;
uniform bool showRefract;

uniform float fresnelPower;
uniform float Nt;
uniform float RGBRatio;

void main()
{
    vec3 NormalVector=normalize(fs_in.NormalVector);
    vec3 ViewVector=CameraPosition-fs_in.FragPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;

	vec3 Entity;
    if(NormalEntity)
    {
        Entity=NormalVector;
    }
    else
    {
        Entity=fs_in.Entity-BoundingBox.Center;
    }

    float Min_x=BoundingBox.Min.x-BoundingBox.Center.x;
    float Max_x=BoundingBox.Max.x-BoundingBox.Center.x;
    float Min_y=BoundingBox.Min.y-BoundingBox.Center.y;
    float Max_y=BoundingBox.Max.y-BoundingBox.Center.y;
    vec2 UV;
    if(UseCPU)
    {
        UV=fs_in.UV;
    }
    else
    {
        switch(UVType)
        {
            case 0:
                UV=ComputeCylindricalUV(Entity, Min_y, Max_y);
                break;
            case 1:
                UV=ComputeSphericalUV(Entity);
                break;
            case 2:
                UV=ComputeCubeMapUV(Entity);
                break;
            case 3:
                UV=ComputePlanarUV(Entity, Min_x, Max_x, Min_y, Max_y);
                break;
        }
    }

    MaterialData mat=Material;
    if(useTexture)
    {
        mat.Diffuse=vec3(texture(DiffuseTexture,UV));
        mat.Specular=vec3(texture(SpecularTexture,UV));
        mat.Shininess=mat.Specular.r*mat.Specular.r;
    }
    vec3 TotalColor=Material.Emissive;
    for(int i=0; i<LightNumbers; ++i)
    {
        switch (Light[i].LightType)
        {
            case 0:
                TotalColor+=ComputePointLight(Light[i], Attenuation, mat, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, mat, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, mat, fs_in.FragPos, NormalVector, ViewVector);
                break;
        }
    }
    
    if(showReflect&&!showRefract)
    {
        vec3 ReflectionVector=ComputeReflection(NormalVector, ViewVector);
        int textureIndex=0;
        vec2 uv=ComputeSkyBoxMapUV(ReflectionVector,textureIndex);
        TotalColor=mix(vec3(texture(skybox[textureIndex], uv)), TotalColor,0.01);
    }

    if(showRefract&&!showReflect)
    {
        vec3 RefractionVectorR=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt-RGBRatio);
        vec3 RefractionVectorG=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt);
        vec3 RefractionVectorB=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt+RGBRatio);
        int textureIndex=0;

        vec2 uv=ComputeSkyBoxMapUV(RefractionVectorR,textureIndex);
        float refractionR=texture(skybox[textureIndex], uv).r;
        uv=ComputeSkyBoxMapUV(RefractionVectorG,textureIndex);
        float refractionG=texture(skybox[textureIndex], uv).g;
        uv=ComputeSkyBoxMapUV(RefractionVectorB,textureIndex);
        float refractionB=texture(skybox[textureIndex], uv).b;

         vec3 refractionRGB=vec3(refractionR, refractionG, refractionB);
        TotalColor=mix(refractionRGB, TotalColor,0.01);
    }

    if(showReflect&&showRefract)
    {
        int textureIndex=0;
        vec3 ReflectionVector=ComputeReflection(NormalVector, ViewVector);
        vec2 uv=ComputeSkyBoxMapUV(ReflectionVector,textureIndex);

        vec3 reflectionRGB=texture(skybox[textureIndex], uv).rgb;

        vec3 RefractionVectorR=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt-RGBRatio);
        vec3 RefractionVectorG=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt);
        vec3 RefractionVectorB=ComputeRefraction(NormalVector, ViewVector, 1.0, Nt+RGBRatio);

        uv=ComputeSkyBoxMapUV(RefractionVectorR,textureIndex);
        float refractionR=texture(skybox[textureIndex], uv).r;
        uv=ComputeSkyBoxMapUV(RefractionVectorG,textureIndex);
        float refractionG=texture(skybox[textureIndex], uv).g;
        uv=ComputeSkyBoxMapUV(RefractionVectorB,textureIndex);
        float refractionB=texture(skybox[textureIndex], uv).b;

        vec3 refractionRGB=vec3(refractionR, refractionG, refractionB);
        
        float fresnelRatio=ComputeFresnel(NormalVector, ViewVector,  1.0, Nt, fresnelPower);
        vec3 fresnelColor=mix(refractionRGB, reflectionRGB, fresnelRatio);
        TotalColor=mix(fresnelColor, TotalColor,0.01);
    }
    
    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);
	FragColor = vec4(globalAmbient+TotalColor, 1.0);
} 
