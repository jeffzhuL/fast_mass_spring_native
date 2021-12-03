#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include "Config.h"
#include "Utility/ShaderManager.h"
#include "Utility/Camera.h"
#include "Constraint.h"
#include "ClothSolver.h"

class Cloth
{
public:
	friend class ClothSolver;

	Cloth();
	~Cloth();

	void initStates(float dt);
	void drawCloth(Camera& camera);

	inline ClothSolver* getClothSolver()
	{
		return m_solver.get();
	}
private:
	const int clothRowNum = CLOTH_ROW_NUM;
	const float clothStepSize = CLOTH_STEPSIZE;
	const float clothInitHeight = CLOTH_INIT_HEIGHT;
	const int m_totalNodeNum = CLOTH_ROW_NUM * CLOTH_ROW_NUM;
	const int m_totalConstrainsNum = (CLOTH_ROW_NUM - 1) * (CLOTH_ROW_NUM - 1) * 6;

	unsigned int m_clothVBO;
	unsigned int m_clothVAO;
	unsigned int m_clothEBO;

	float calculateRestLength(int startIndex, int endIndex);

	std::unique_ptr<Shader> m_shader;

	unsigned int m_indices[(CLOTH_ROW_NUM - 1) * (CLOTH_ROW_NUM - 1) * 2 * 3];
	float m_mass[CLOTH_ROW_NUM * CLOTH_ROW_NUM];
	Constraint m_constrains[(CLOTH_ROW_NUM - 1) * (CLOTH_ROW_NUM - 1) * 6];
	float m_pos[CLOTH_ROW_NUM * CLOTH_ROW_NUM * 3]; // q(n)

	std::unique_ptr<ClothSolver> m_solver;
};