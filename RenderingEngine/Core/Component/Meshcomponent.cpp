#include "MeshComponent.h"
#include "Core/AssetManager.h"
MeshComponent::MeshComponent(const std::string& asset)
	:uuid(AssetManager::GetUUID(asset))
{
}
