/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>

#include "Core/Scene/Scene.h"
#include "Core/Utils/Math.h"
#include "DemoEnums/DemoEnums.h"
#include "Core/Data/UV.h"
#include "Core/Data/Light.h"
#include "Core/Graphics/FrameBuffer.h"
class Layer;
class VertexBuffer;
class Shader;
class VertexArray;

class Scenario_1 : public Scene
{
public:
	Scenario_1(Application& app);
	void Awake() override;
	void OnEnable() override;
	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void OnDisable() override;
	void OnDestroy() override;
	void OnEvent(Event& event) override;
private:

	Entity demo_ctrl;
	Entity demo_mesh;
	Entity orbit;
};
