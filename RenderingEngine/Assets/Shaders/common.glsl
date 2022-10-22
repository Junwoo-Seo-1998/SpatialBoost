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

struct MatrixData
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
    mat4 Normal;
};

struct MaterialData
{
    vec3 Emissive;
};

mat4 ComputeNormalMatrix(mat4 model)
{
    return transpose(inverse(model));
}