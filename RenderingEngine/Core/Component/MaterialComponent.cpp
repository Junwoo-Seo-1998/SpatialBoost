#include "MaterialComponent.h"
#include "Core/AssetManager.h"
MaterialComponent::MaterialComponent(const std::string& shader)
	:shader(AssetManager::GetUUID(shader)), material()
{
}

MaterialData& MaterialComponent::operator[](const std::string& key)
{
	return material[key];
}
