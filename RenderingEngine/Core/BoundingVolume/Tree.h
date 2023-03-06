#pragma once

enum class TreeNodeType
{
	Internal,
	Leaf
};

template<typename BoundingVolume, typename Data>
struct TreeNode
{
	TreeNodeType type;
	Data* data;
	int numObjects;
	TreeNode* left=nullptr;
	TreeNode* right=nullptr;
	BoundingVolume bv;
};