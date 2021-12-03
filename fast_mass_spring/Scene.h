#pragma once
#include <memory>
#include "Utility/Camera.h"

class Cloth;

class Scene
{
public:
	Scene();
	~Scene();
	
	void drawGround();

	inline Cloth* getColth()
	{
		return m_cloth.get();
	}

	inline Camera* getCamera()
	{
		return m_camera.get();
	}
private:
	std::unique_ptr<Cloth> m_cloth;
	std::unique_ptr<Camera> m_camera;
};