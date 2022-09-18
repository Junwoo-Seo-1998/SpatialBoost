#pragma once
class Scene
{
public:
	virtual void Awake() = 0;
	virtual void OnEnable() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;
};

