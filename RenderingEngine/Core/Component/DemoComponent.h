#pragma once
struct DemoComponent
{
	bool enable = true;
	std::string meshName = "bunny";
};

struct DemoControlComponent
{
	bool StopRotation = false;
	int LightNumber = 8;
};
