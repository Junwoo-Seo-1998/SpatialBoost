#pragma once
#include <vector>

#include "Layer.h"
#include "Core/BoundingVolume/AABB.h"
#include "Core/BoundingVolume/Tree.h"
#include "Core/Scene/Entity.h"

class PhysicsLayer :public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnGuiRender() override;
	void OnEvent(Event& event) override;
private:
	std::vector<entt::entity> entities;
	std::vector<entt::entity> bottomUPEntities;
	TreeNode<AABB, entt::entity>* head=nullptr;
	TreeNode<AABB, entt::entity>* bottomUphead = nullptr;
	void DrawNode(TreeNode<AABB, entt::entity>* tree, int level);
	void ComputeTopDown(TreeNode<AABB, entt::entity>** tree, entt::entity* objects, int numObjects, int height);
	void ComputeBottomUp();
	int heightLevel = 1;
	std::vector<glm::vec3> colors;
	bool show_aabb = false;
	bool show_ritter = false;
	bool show_larsson = false;
	bool show_pca = false;
	bool show_topdown = false;
	bool show_bottomup = false;
};
