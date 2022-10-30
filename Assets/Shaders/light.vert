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


out VS_OUT{ 
    vec3 outColor; 
} vs_out; 

uniform MatrixData Matrix;
uniform LightData Light[MAX_LIGHTS];

uniform AttenuationData Attenuation;

uniform vec3 CameraPosition;
uniform MaterialData Material;

uniform FogData Fog;

void main()
{
	gl_Position = Matrix.Projection*Matrix.View*Matrix.Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);

	//
	vec3 WorldPos = vec3(Matrix.Model*vec4(aPos,1.0));
	vec3 NormalVector=normalize(mat3(Matrix.Normal) * aNormal);
	vec3 ViewVector=CameraPosition-WorldPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;

	vec3 TotalColor=ComputePointLight(Light[0], Attenuation, Material, WorldPos, NormalVector, ViewVector);
	TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);

	vs_out.outColor=TotalColor;
}