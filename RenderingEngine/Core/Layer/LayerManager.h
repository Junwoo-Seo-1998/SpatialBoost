#pragma once
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
#include <memory>
#include <queue>
class Application;
class Layer;

class LayerManager
{
public:
	LayerManager() = default;
	~LayerManager();

	void PushLayer(std::shared_ptr<Layer> layer);
	void PushOverlay(std::shared_ptr<Layer> overlay);
	void PopLayer(std::shared_ptr<Layer> layer);
	void PopOverlay(std::shared_ptr<Layer> overlay);



	std::vector<std::shared_ptr<Layer>>& GetLayers() { return m_Layers; }
	std::vector<std::shared_ptr<Layer>>& GetOverLays() { return m_Overlays; }
private:
	friend Application;
	void ClearDeleteQueue();
	void PopLayerDirect(std::shared_ptr<Layer> layer);
	void PopOverlayDirect(std::shared_ptr<Layer> overlay);
private:
	std::vector<std::shared_ptr<Layer>> m_Layers;
	std::vector<std::shared_ptr<Layer>> m_Overlays;


	std::queue< std::shared_ptr<Layer>> m_LayerDeleteQueue;
	std::queue< std::shared_ptr<Layer>> m_OverlayDeleteQueue;
};

