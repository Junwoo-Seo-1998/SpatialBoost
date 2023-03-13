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

PhysicsLayer::~PhysicsLayer()
{
	CleanTree(head);
	CleanTree(bottomUphead);
}

void PhysicsLayer::OnAttach()
{
	colors = Math::RandomVec3(7, glm::vec3{ 0.5f }, glm::vec3{ 1.f });
}

void PhysicsLayer::OnDetach()
{
	CleanTree(head);
	CleanTree(bottomUphead);
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

	auto spheres = registry.view<PCASphereComponent>();
	for (auto& entity : spheres)
	{
		entitiesSphere.push_back(entity);
		bottomUPEntitiesSphere.push_back(entity);
	}
	
	ComputeTopDown(&head, entities.data(), entities.size(), 1);
	ComputeTopDownSphere(&headSphere, entitiesSphere.data(), entitiesSphere.size(), 1);
	ComputeBottomUp();
	ComputeBottomUpSphere();
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

void PhysicsLayer::DrawNodeSphere(TreeNode<Sphere, entt::entity>* tree, int level)
{
	if (tree == nullptr)
		return;

	if (heightLevel == level)
	{
		DebugRenderer::DrawSphere(tree->bv.center, tree->bv.radius, colors[heightLevel - 1]);
	}

	DrawNodeSphere(tree->left, level + 1);
	DrawNodeSphere(tree->right, level + 1);
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
	if (show_topdown_sphere)
	{
		DrawNodeSphere(headSphere, 1);
	}

	if (show_bottomup)
	{
		DrawNode(bottomUphead, 1);
	}

	if(show_bottomup_sphere)
	{
		DrawNodeSphere(bottomUpheadSphere, 1);
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
	ImGui::Checkbox("Show TopDown Sphere", &show_topdown_sphere);
	ImGui::Checkbox("Show BottomUP Sphere", &show_bottomup_sphere);
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

		//compute optimal plane
		std::vector<glm::vec3> points;
		points.reserve(numObjects);
		for (int i = 0; i < numObjects; ++i)
		{
			points.emplace_back(registry.get<AABBComponent>(objects[i]).center);
		}
		glm::vec3 optimalVector = Math::ComputePCAVector(points);
		optimalVector = glm::abs(optimalVector);
		int axis = 0;
		if (optimalVector[axis] < optimalVector[1])
		{
			axis = 1;
		}
		if (optimalVector[axis] < optimalVector[2])
		{
			axis = 2;
		}

		int partition_index = 0;
		if (top_down_selection == TopDownSelection::Centers)
		{
			//median center
			std::sort(objects, objects + numObjects, [&](entt::entity a, entt::entity b)
			{
				auto& A = registry.get<AABBComponent>(a);
				auto& B = registry.get<AABBComponent>(b);

				return A.center[axis] > B.center[axis];
			});
			partition_index = numObjects / 2;
		}
		else if(top_down_selection==TopDownSelection::Extents) //median extends
		{
			std::vector<float> extends;
			extends.reserve(numObjects * 2);
			for(int i=0; i<numObjects; ++i)
			{
				auto& aabb = registry.get<AABBComponent>(objects[i]);
				float min = aabb.center[axis] - aabb.extend[axis];
				float max = aabb.center[axis] + aabb.extend[axis];
				extends.emplace_back(min);
				extends.emplace_back(max);
			}

			std::sort(std::begin(extends), std::end(extends));
			float mid_val = extends[extends.size() / 2];
			auto middle = std::partition(objects, objects + numObjects, [&](entt::entity entity)
			{
				auto& aabb = registry.get<AABBComponent>(entity);
				return aabb.center[axis] < mid_val;
			});

			int i = 0;
			while (&objects[i] != middle)
			{
				++i;
			}
			partition_index = i;
		}
		else //k even
		{
			int k = 16;
			float split_val = min[axis] + (max[axis] - min[axis]) / 2.f;
			auto middle = std::partition(objects, objects + numObjects, [&](entt::entity entity)
			{
				auto& aabb = registry.get<AABBComponent>(entity);
				return aabb.center[axis] < split_val;
			});

			int i = 0;
			while (&objects[i] != middle)
			{
				++i;
			}
			partition_index = i;
		}
		ComputeTopDown(&(pNode->left), &objects[0], partition_index, height+1);
		ComputeTopDown(&(pNode->right), &objects[partition_index], numObjects - partition_index, height+1);
	}

}

void PhysicsLayer::ComputeTopDownSphere(TreeNode<Sphere, entt::entity>** tree, entt::entity* objects, int numObjects,
	int height)
{
	//compue bv
	constexpr  int MIN_OBJECTS_PER_LEAF = 1;
	constexpr  int MAX_HEIGHT = 7;
	TreeNode<Sphere, entt::entity>* pNode = new TreeNode<Sphere, entt::entity>;
	*tree = pNode;

	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();

	glm::vec3 min{ std::numeric_limits<float>().max() }, max{ std::numeric_limits<float>().lowest() };

	std::vector<Sphere> spheres;
	spheres.reserve(numObjects);
	for (int i = 0; i < numObjects; ++i)
	{
		auto& sphere = registry.get<PCASphereComponent>(objects[i]);
		spheres.emplace_back(Sphere{ sphere.center, sphere.radius });
	}

	pNode->bv = Math::CreateEnclosingSphere(spheres);

	if (numObjects <= MIN_OBJECTS_PER_LEAF || height >= MAX_HEIGHT)
	{
		pNode->type = TreeNodeType::Leaf;
		pNode->numObjects = numObjects;
		pNode->data = &objects[0];
	}
	else {
		pNode->type = TreeNodeType::Internal;

		//compute optimal plane
		std::vector<glm::vec3> points;
		points.reserve(numObjects);
		for (int i = 0; i < numObjects; ++i)
		{
			points.emplace_back(registry.get<PCASphereComponent>(objects[i]).center);
		}
		glm::vec3 optimalVector = Math::ComputePCAVector(points);
		optimalVector = glm::abs(optimalVector);
		int axis = 0;
		if (optimalVector[axis] < optimalVector[1])
		{
			axis = 1;
		}
		if (optimalVector[axis] < optimalVector[2])
		{
			axis = 2;
		}

		int partition_index = 0;
		if (top_down_selection == TopDownSelection::Centers)
		{
			//median center
			std::sort(objects, objects + numObjects, [&](entt::entity a, entt::entity b)
				{
					auto& A = registry.get<PCASphereComponent>(a);
			auto& B = registry.get<PCASphereComponent>(b);

			return A.center[axis] > B.center[axis];
				});
			partition_index = numObjects / 2;
		}
		else if (top_down_selection == TopDownSelection::Extents) //median extends
		{
			std::vector<float> extends;
			extends.reserve(numObjects * 2);
			for (int i = 0; i < numObjects; ++i)
			{
				auto& aabb = registry.get<AABBComponent>(objects[i]);
				float min = aabb.center[axis] - aabb.extend[axis];
				float max = aabb.center[axis] + aabb.extend[axis];
				extends.emplace_back(min);
				extends.emplace_back(max);
			}

			std::sort(std::begin(extends), std::end(extends));
			float mid_val = extends[extends.size() / 2];
			auto middle = std::partition(objects, objects + numObjects, [&](entt::entity entity)
				{
					auto& aabb = registry.get<AABBComponent>(entity);
					return aabb.center[axis] < mid_val;
				});

			int i = 0;
			while (&objects[i] != middle)
			{
				++i;
			}
			partition_index = i;
		}
		else if (top_down_selection == TopDownSelection::Extents)//k even
		{
			int k = 16;
			float split_val = min[axis] + (max[axis] - min[axis]) / 2.f;
			auto middle = std::partition(objects, objects + numObjects, [&](entt::entity entity)
				{
					auto& aabb = registry.get<AABBComponent>(entity);
					return aabb.center[axis] < split_val;
				});

			int i = 0;
			while (&objects[i] != middle)
			{
				++i;
			}
			partition_index = i;
		}
		ComputeTopDownSphere(&(pNode->left), &objects[0], partition_index, height + 1);
		ComputeTopDownSphere(&(pNode->right), &objects[partition_index], numObjects - partition_index, height + 1);
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
		std::vector<std::tuple<int, int>> nearestPair;
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
					nearestPair.push_back({ first, second });
				}
				else if(min_len == len)
				{
					nearestPair.push_back({ i, j });
				}
			}
		}
		TreeNode<AABB, entt::entity>* parent = new TreeNode<AABB, entt::entity>();
		if (nearestPair.size() == 1)
		{
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
		}
		else
		{
			std::vector<std::tuple<int, int>> smallestCombinedPair;
			float smallest_volume = std::numeric_limits<float>::max();
			for (const auto& pair : nearestPair)
			{
				auto [index1, index2] = pair;
				auto aabb = Math::CreateEnclosingAABB({ nodes[index1]->bv, nodes[index2]->bv });
				const float volume = Math::ComputeAABBVolume(aabb);
				if (volume < smallest_volume) {
					smallest_volume = volume;
				}
				else if (volume == smallest_volume)
				{
					smallestCombinedPair.push_back(pair);
				}
			}

			if (smallestCombinedPair.size() == 1)
			{
				auto [index1, index2] = smallestCombinedPair[0];
				first = index1;
				second = index2;
			}
			else
			{
				float smallest_relative_increase = std::numeric_limits<float>::max();
				for (const auto& pair : smallestCombinedPair)
				{
					auto [index1, index2] = pair;
					const float vol1 = Math::ComputeAABBVolume(nodes[index1]->bv);
					const float vol2 = Math::ComputeAABBVolume(nodes[index2]->bv);
					auto aabb = Math::CreateEnclosingAABB({ nodes[index1]->bv, nodes[index2]->bv });
					const float volume = Math::ComputeAABBVolume(aabb);
					const float relative_increase = volume - (vol1 + vol2);

					if (relative_increase < smallest_relative_increase) {
						smallest_relative_increase = relative_increase;
						first = index1;
						second = index2;
					}
				}
			}

			parent->left = nodes[first];
			parent->right = nodes[second];
			parent->bv = Math::CreateEnclosingAABB({ nodes[first]->bv, nodes[second]->bv });
			if (first > second)
				std::swap(first, second);
		}

		nodes[first] = parent;
		nodes[second] = nodes[nodes.size() - 1];
		nodes.pop_back();
	}
	bottomUphead = nodes[0]; // return the root node
}

void PhysicsLayer::ComputeBottomUpSphere()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	std::vector<TreeNode<Sphere, entt::entity>*> nodes;
	int size = bottomUPEntitiesSphere.size();
	for (int i = 0; i < size; ++i)
	{
		TreeNode<Sphere, entt::entity>* node = new TreeNode<Sphere, entt::entity>();
		node->type = TreeNodeType::Leaf;
		node->data = &bottomUPEntitiesSphere[i];
		node->numObjects = 1;
		auto& sphere = registry.get<PCASphereComponent>(bottomUPEntitiesSphere[i]);
		node->bv = Sphere{ sphere.center, sphere.radius };
		nodes.push_back(node);
	}

	while (nodes.size() > 1)
	{
		int first = 0, second = 0;
		float min_len = std::numeric_limits<float>().max();
		std::vector<std::tuple<int, int>> nearestPair;
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
					nearestPair.push_back({ first, second });
				}
				else if (min_len == len)
				{
					nearestPair.push_back({ i, j });
				}
			}
		}
		TreeNode<Sphere, entt::entity>* parent = new TreeNode<Sphere, entt::entity>();
		if (nearestPair.size() == 1)
		{
			parent->left = nodes[first];
			parent->right = nodes[second];
			
			parent->bv = Math::MergeTwoSphere(nodes[first]->bv, nodes[second]->bv);

			if (first > second)
				std::swap(first, second);
		}
		else
		{
			std::vector<std::tuple<int, int>> smallestCombinedPair;
			float smallest_rad = std::numeric_limits<float>::max();
			for (const auto& pair : nearestPair)
			{
				auto [index1, index2] = pair;
				auto sphere = Math::MergeTwoSphere(nodes[index1]->bv, nodes[index2]->bv);
				const float rad = sphere.radius;
				if (rad < smallest_rad) {
					smallest_rad = rad;
				}
				else if (rad == smallest_rad)
				{
					smallestCombinedPair.push_back(pair);
				}
			}

			if (smallestCombinedPair.size() == 1)
			{
				auto [index1, index2] = smallestCombinedPair[0];
				first = index1;
				second = index2;
			}
			else
			{
				float smallest_relative_increase = std::numeric_limits<float>::max();
				for (const auto& pair : smallestCombinedPair)
				{
					auto [index1, index2] = pair;
					const float rad1 = nodes[index1]->bv.radius;
					const float rad2 = nodes[index2]->bv.radius;
					auto sphere = Math::MergeTwoSphere(nodes[index1]->bv, nodes[index2]->bv);
					const float rad = sphere.radius;
					const float relative_increase = rad - (rad1 + rad2);

					if (relative_increase < smallest_relative_increase) {
						smallest_relative_increase = relative_increase;
						first = index1;
						second = index2;
					}
				}
			}

			parent->left = nodes[first];
			parent->right = nodes[second];
			parent->bv = Math::CreateEnclosingSphere({ nodes[first]->bv, nodes[second]->bv });
			if (first > second)
				std::swap(first, second);
		}

		nodes[first] = parent;
		nodes[second] = nodes[nodes.size() - 1];
		nodes.pop_back();
	}
	bottomUpheadSphere = nodes[0]; // return the root node
}

void PhysicsLayer::CleanTree(TreeNode<AABB, entt::entity>* head)
{
	if (head == nullptr)
		return;

	auto left = head->left;
	auto right = head->right;

	delete head;

	CleanTree(left);
	CleanTree(right);
}

