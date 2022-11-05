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
layout (location = 2) in vec2 aUV;

out VS_OUT{ 
    vec3 outColor; 
} vs_out; 

uniform MatrixData Matrix;
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

uniform bool useTexture;
uniform BoundingBoxData BoundingBox;

uniform int UVType;
uniform bool UseCPU;
uniform bool NormalEntity;
void main()
{
	gl_Position = Matrix.Projection*Matrix.View*Matrix.Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);

	//
	vec3 WorldPos = vec3(Matrix.Model*vec4(aPos,1.0));
	vec3 NormalVector=normalize(mat3(Matrix.Normal) * aNormal);
	vec3 ViewVector=CameraPosition-WorldPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;


 	vec3 Entity;
    if(NormalEntity)
    {
        Entity=aNormal;
    }
    else
    {
        Entity=aPos-BoundingBox.Center;
    }

    float Min_x=BoundingBox.Min.x-BoundingBox.Center.x;
    float Max_x=BoundingBox.Max.x-BoundingBox.Center.x;
    float Min_y=BoundingBox.Min.y-BoundingBox.Center.y;
    float Max_y=BoundingBox.Max.y-BoundingBox.Center.y;
    vec2 UV;
    if(UseCPU)
    {
        UV=aUV;
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
                TotalColor+=ComputePointLight(Light[i], Attenuation, mat, WorldPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, mat, WorldPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, mat, WorldPos, NormalVector, ViewVector);
                break;
        }
    }
    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);

	vs_out.outColor=TotalColor+globalAmbient;
}