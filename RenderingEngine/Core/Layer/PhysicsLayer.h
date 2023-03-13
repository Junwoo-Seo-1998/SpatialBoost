#pragma once
#include <vector>

#include "Layer.h"
#include "Core/BoundingVolume/AABB.h"
#include "Core/BoundingVolume/Sphere.h"
#include "Core/BoundingVolume/Tree.h"
#include "Core/Scene/Entity.h"

enum class TopDownSelection
{
	Centers = 0,
	Extents,
	KSplits,
};

enum class BottomUpSelection
{
	Near = 0,
	Extents,
	KSplits,
};
class PhysicsLayer :public Layer
{
public:
	~PhysicsLayer() override;
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
	std::vector<entt::entity> entitiesSphere;
	std::vector<entt::entity> bottomUPEntities;
	std::vector<entt::entity> bottomUPEntitiesSphere;
	TreeNode<AABB, entt::entity>* head=nullptr;
	TreeNode<Sphere, entt::entity>* headSphere = nullptr;
	TreeNode<AABB, entt::entity>* bottomUphead = nullptr;
	TreeNode<Sphere, entt::entity>* bottomUpheadSphere = nullptr;
	void DrawNode(TreeNode<AABB, entt::entity>* tree, int level);
	void DrawNodeSphere(TreeNode<Sphere, entt::entity>* tree, int level);
	void ComputeTopDown(TreeNode<AABB, entt::entity>** tree, entt::entity* objects, int numObjects, int height);
	void ComputeTopDownSphere(TreeNode<Sphere, entt::entity>** tree, entt::entity* objects, int numObjects, int height);
	void ComputeBottomUp();
	void ComputeBottomUpSphere();
	void CleanTree(TreeNode<AABB, entt::entity>* head);
	int heightLevel = 1;
	std::vector<glm::vec3> colors;
	bool show_aabb = false;
	bool show_ritter = false;
	bool show_larsson = false;
	bool show_pca = false;
	bool show_topdown = false;
	bool show_bottomup = false;
	bool show_topdown_sphere = false;
	bool show_bottomup_sphere = false;

	TopDownSelection top_down_selection = TopDownSelection::Centers;
	
};
