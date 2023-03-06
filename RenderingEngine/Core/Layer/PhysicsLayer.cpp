#include "PhysicsLayer.h"

#include "imgui.h"
#include "Core/Component/BoundingVolumeComponent.h"
#include "Core/Graphics/DebugRenderer.h"
#include "Core/Application.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Utils/Log.h"
#include "glm/gtx/string_cast.hpp"

void PhysicsLayer::OnAttach()
{
	colors = Math::RandomVec3(7, glm::vec3{ 0.5f }, glm::vec3{ 1.f });
}

void PhysicsLayer::OnDetach()
{
	Layer::OnDetach();
}

void PhysicsLayer::OnStart()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	auto aabbs = registry.view<AABBComponent>();
	for (auto& entity : aabbs)
	{
		entities.push_back(entity);
		bottomUPEntities.push_back(entity);
	}
	
	ComputeTopDown(&head, entities.data(), entities.size(), 1);
	ComputeBottomUp();
}

void PhysicsLayer::OnUpdate()
{
	Layer::OnUpdate();
}

void PhysicsLayer::OnPreRender()
{
	Layer::OnPreRender();
}

void PhysicsLayer::OnRender()
{
	Layer::OnRender();
}
void PhysicsLayer::DrawNode(TreeNode<AABB, entt::entity>* tree, int level)
{
	if(tree==nullptr)
		return;

	if (heightLevel == level)
	{
		DebugRenderer::DrawBox(tree->bv.center, tree->bv.extend, colors[heightLevel - 1]);
	}

	DrawNode(tree->left, level + 1);
	DrawNode(tree->right, level + 1);
}

void PhysicsLayer::OnPostRender()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();
	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());
	//debug drawing

	DebugRenderer::DrawBegin(world_to_cam, camComp.GetPerspective());
	if (show_aabb)
	{
		auto aabbs = registry.view<AABBComponent>();
		for (auto& entity : aabbs)
		{
			auto& aabb = aabbs.get<AABBComponent>(entity);
			DebugRenderer::DrawBox(aabb.center, aabb.extend, { 1.0,0,0 });
		}
	}

	if (show_ritter)
	{
		auto RitterSpheres = registry.view<RitterSphereComponent>();
		for (auto& entity : RitterSpheres)
		{
			auto& sphere = RitterSpheres.get<RitterSphereComponent>(entity);
			DebugRenderer::DrawSphere(sphere.center, sphere.radius, { 1.0,1.0,0 });
		}
	}

	if (show_larsson)
	{
		auto LarssonSpheres = registry.view<LarssonSphereComponent>();
		for (auto& entity : LarssonSpheres)
		{
			auto& sphere = LarssonSpheres.get<LarssonSphereComponent>(entity);
			DebugRenderer::DrawSphere(sphere.center, sphere.radius, { 1.0,0.0,1.0 });
		}
	}

	if (show_pca)
	{
		auto PCASpheres = registry.view<PCASphereComponent>();
		for (auto& entity : PCASpheres)
		{
			auto& sphere = PCASpheres.get<PCASphereComponent>(entity);
			DebugRenderer::DrawSphere(sphere.center, sphere.radius, { 0,1.0,1.0 });
		}
	}

	if (show_topdown)
	{
		DrawNode(head, 1);
	}

	if (show_bottomup)
	{
		DrawNode(bottomUphead, 1);
	}

	DebugRenderer::DrawBox({ 0,0,0 }, { 3,3,3 }, { 1.0,0,0 });
	DebugRenderer::DrawSphere({ 0,0,0 }, 3.f, { 0.25f,0.25f,0.25f });
	DebugRenderer::DrawEnd();
}

void PhysicsLayer::OnGuiRender()
{
	ImGui::Begin("Debug Bounding Box");
	ImGui::DragInt("Height", &heightLevel, 1, 1, 7);
	ImGui::Checkbox("Show TopDown", &show_topdown);
	ImGui::Checkbox("Show BottomUP", &show_bottomup);
	ImGui::Checkbox("Show AABB", &show_aabb);
	ImGui::Checkbox("Show Ritter", &show_ritter);
	ImGui::Checkbox("Show Larsson", &show_larsson);
	ImGui::Checkbox("Show PCA", &show_pca);
	ImGui::End();
}

void PhysicsLayer::OnEvent(Event& event)
{
	Layer::OnEvent(event);
}

void PhysicsLayer::ComputeTopDown(TreeNode<AABB, entt::entity>** tree, entt::entity* objects, int numObjects, int height)
{
	//compue bv
	constexpr  int MIN_OBJECTS_PER_LEAF = 1;
	constexpr  int MAX_HEIGHT = 7;
	TreeNode<AABB, entt::entity>* pNode = new TreeNode<AABB, entt::entity>;
	*tree = pNode;

	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();

	glm::vec3 min{ std::numeric_limits<float>().max() }, max{ std::numeric_limits<float>().lowest() };

	for (int i=0; i<numObjects; ++i)
	{
		auto& aabb = registry.get<AABBComponent>(objects[i]);
		auto aabb_min = aabb.center - aabb.extend;
		auto aabb_max = aabb.center + aabb.extend;
		min.x = std::min(min.x,aabb_min.x);
		min.y = std::min(min.y,aabb_min.y);
		min.z = std::min(min.z,aabb_min.z);

		max.x = std::max(max.x,aabb_max.x);
		max.y = std::max(max.y,aabb_max.y);
		max.z = std::max(max.z,aabb_max.z);
	}
	auto center = (min + max) / 2.f;
	pNode->bv = { center, max - center };

	if (numObjects <= MIN_OBJECTS_PER_LEAF || height >= MAX_HEIGHT)
	{
		pNode->type = TreeNodeType::Leaf;
		pNode->numObjects = numObjects;
		pNode->data = &objects[0]; 
	}
	else {
		pNode->type = TreeNodeType::Internal;

		int axis = height % 3;
		if(axis==0)
		{
			std::sort(objects, objects + numObjects, [&](entt::entity a, entt::entity b)
				{
					auto& A=registry.get<AABBComponent>(a);
					auto& B=registry.get<AABBComponent>(b);
					
					return A.center.x > B.center.x;
				});
		}
		else if(axis==1)
		{
			std::sort(objects, objects+numObjects, [&](entt::entity a, entt::entity b)
				{
					auto& A = registry.get<AABBComponent>(a);
			auto& B = registry.get<AABBComponent>(b);

			return A.center.z > B.center.z;
				});
		}
		else
		{
			std::sort(objects, objects + numObjects, [&](entt::entity a, entt::entity b)
				{
					auto& A = registry.get<AABBComponent>(a);
			auto& B = registry.get<AABBComponent>(b);

			return A.center.y > B.center.y;
				});
		}
		
		int k = numObjects / 2;

		ComputeTopDown(&(pNode->left), &objects[0], k, height+1);
		ComputeTopDown(&(pNode->right), &objects[k], numObjects - k, height+1);
	}

}

void PhysicsLayer::ComputeBottomUp()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	std::vector<TreeNode<AABB, entt::entity>*> nodes;
	int size = bottomUPEntities.size();
	for(int i=0; i<size; ++i)
	{
		TreeNode<AABB, entt::entity>* node = new TreeNode<AABB, entt::entity>();
		node->type = TreeNodeType::Leaf;
		node->data = &bottomUPEntities[i];
		node->numObjects = 1;
		auto& aabb = registry.get<AABBComponent>(bottomUPEntities[i]);
		node->bv = AABB{ aabb.center, aabb.extend };
		nodes.push_back(node);
	}
	
	while (nodes.size() > 1)
	{
		int first = 0, second = 0;

		float min_len = std::numeric_limits<float>().max();
		for (int i = 0; i < nodes.size(); ++i)
		{
			for (int j = i + 1; j < nodes.size(); ++j)
			{
				float len = glm::distance(nodes[j]->bv.center, nodes[i]->bv.center);
				if (min_len > len)
				{
					min_len = len;
					first = i;
					second = j;
				}
			}
		}
		TreeNode<AABB, entt::entity>* parent = new TreeNode<AABB, entt::entity>();
		parent->left = nodes[first];
		parent->right = nodes[second];
		glm::vec3 min{ std::numeric_limits<float>().max() }, max{ std::numeric_limits<float>().lowest() };

		{
			auto& aabb = nodes[first]->bv;
			auto aabb_min = aabb.center - aabb.extend;
			auto aabb_max = aabb.center + aabb.extend;
			min.x = std::min(min.x, aabb_min.x);
			min.y = std::min(min.y, aabb_min.y);
			min.z = std::min(min.z, aabb_min.z);

			max.x = std::max(max.x, aabb_max.x);
			max.y = std::max(max.y, aabb_max.y);
			max.z = std::max(max.z, aabb_max.z);

			auto& aabb2 = nodes[second]->bv;
			aabb_min = aabb2.center - aabb2.extend;
			aabb_max = aabb2.center + aabb2.extend;
			min.x = std::min(min.x, aabb_min.x);
			min.y = std::min(min.y, aabb_min.y);
			min.z = std::min(min.z, aabb_min.z);

			max.x = std::max(max.x, aabb_max.x);
			max.y = std::max(max.y, aabb_max.y);
			max.z = std::max(max.z, aabb_max.z);
		}
		auto center = (min + max) / 2.f;
		parent->bv = { center, max - center };

		if (first > second)
			std::swap(first, second);

		nodes[first] = parent;
		nodes[second] = nodes[nodes.size() - 1];
		nodes.pop_back();
	}
	bottomUphead = nodes[0]; // return the root node
}

