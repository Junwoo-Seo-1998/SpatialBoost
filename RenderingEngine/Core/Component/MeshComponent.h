#pragma once
#include <string>

#include "Core/UUID.h"

struct MeshComponent
{
	MeshComponent(const std::string& asset);
	UUID uuid;
};