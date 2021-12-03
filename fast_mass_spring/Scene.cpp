#include "Scene.h"
#include "Config.h"
#include "Application.h"
#include "Cloth.h"

Scene::Scene()
{
	m_cloth = std::make_unique<Cloth>();
	m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 13.0f, CAMERA_INIT_POSITION));
}

Scene::~Scene()
{
}

void Scene::drawGround()
{
	/*int GRID_SIZE = 10;
	glBegin(GL_LINES);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		glVertex3f((float)i, 0, (float)-GRID_SIZE);
		glVertex3f((float)i, 0, (float)GRID_SIZE);

		glVertex3f((float)-GRID_SIZE, 0, (float)i);
		glVertex3f((float)GRID_SIZE, 0, (float)i);
	}
	glEnd();*/
}
