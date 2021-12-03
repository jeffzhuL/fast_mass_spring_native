#include "Cloth.h"
#include "Utility/Camera.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

Cloth::Cloth()
{
	float avgpos = (0 + CLOTH_ROW_NUM - 1)* clothStepSize * 0.5f;
	// first, we init the position of each node
	for (int i = 0; i < clothRowNum; i++)
	{
		for (int j = 0; j < clothRowNum; j++)
		{
			int index = i * clothRowNum + j;

			m_pos[index * 3 + 0] = i * clothStepSize - avgpos;
			m_pos[index * 3 + 1] = 10.0f;
			m_pos[index * 3 + 2] = -j * clothStepSize + avgpos;
		}
	}
	//memcpy(m_backwardPos, m_pos, sizeof(m_pos));

	// second, we init the indices for triangles
	for (int i = 0; i < clothRowNum - 1; i++)
	{
		for (int j = 0; j < clothRowNum - 1; j++)
		{
			unsigned int index0 = i * clothRowNum + j;
			unsigned int index1 = index0 + 1;
			unsigned int index2 = index1 + clothRowNum;
			unsigned int index3 = index0 + clothRowNum;
			 
			int index = i * (clothRowNum - 1) + j;

			m_indices[index * 6 + 0] = index0;
			m_indices[index * 6 + 1] = index1;
			m_indices[index * 6 + 2] = index2;

			m_indices[index * 6 + 3] = index2;
			m_indices[index * 6 + 4] = index3;
			m_indices[index * 6 + 5] = index0;

			// now config the constraints
			float restLength0 = calculateRestLength(index0, index1);
			float restLength1 = calculateRestLength(index1, index2);
			float restLength2 = calculateRestLength(index2, index3);
			float restLength3 = calculateRestLength(index3, index0);
			float restLength4 = calculateRestLength(index0, index2);
			float restLength5 = calculateRestLength(index1, index3);

			m_constrains[index * 6 + 0] = Constraint(restLength0, index0, index1, SPRING_STIFFNESS);
			m_constrains[index * 6 + 1] = Constraint(restLength1, index1, index2, SPRING_STIFFNESS);
			m_constrains[index * 6 + 2] = Constraint(restLength2, index2, index3, SPRING_STIFFNESS);
			m_constrains[index * 6 + 3] = Constraint(restLength3, index3, index0, SPRING_STIFFNESS);
			m_constrains[index * 6 + 4] = Constraint(restLength4, index0, index2, SPRING_STIFFNESS);
			m_constrains[index * 6 + 5] = Constraint(restLength5, index1, index3, SPRING_STIFFNESS);
		}
	}

	//third we should init the inverse mass
	for (int i = 0; i < clothRowNum; i++)
	{
		for (int j = 0; j < clothRowNum; j++)
		{
			int index = i * clothRowNum + j;
			m_mass[index] = MASS;
		}
	}

	m_solver = std::make_unique<ClothSolver>(m_totalNodeNum, m_totalConstrainsNum, m_pos, m_constrains, m_mass);
}

void Cloth::initStates(float dt)
{
	m_shader = std::make_unique<Shader>("Shader/cloth.vs", "Shader/cloth.fs");

	glGenVertexArrays(1, &m_clothVAO);
	glGenBuffers(1, &m_clothVBO);
	glGenBuffers(1, &m_clothEBO);
}

void Cloth::drawCloth(Camera& camera)
{
	glBindVertexArray(m_clothVAO);
	//config the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_clothVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_pos), m_pos, GL_DYNAMIC_DRAW);
	//config the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_clothEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shader->use();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	m_shader->setMat4("projection", projection);

	glm::mat4 view = camera.GetViewMatrix();
	m_shader->setMat4("view", view);

	glm::mat4 model(1.0f);
	m_shader->setMat4("model", model);

	/*glBindVertexArray(m_clothVAO);
	glDrawElements(GL_TRIANGLES, (clothRowNum - 1) * (clothRowNum - 1) * 2 * 3, GL_UNSIGNED_INT, 0);*/
	glPointSize(3);
	glBegin(GL_POINTS);
	for (int i = 0; i < m_totalNodeNum; i++)
	{
		glVertex3f(m_pos[i * 3 + 0], m_pos[i * 3 + 1], m_pos[i * 3 + 2]);
	}
	glEnd();
}

Cloth::~Cloth()
{
	glDeleteVertexArrays(1, &m_clothVAO);
	glDeleteBuffers(1, &m_clothVBO);
	glDeleteBuffers(1, &m_clothEBO);
}

float Cloth::calculateRestLength(int startIndex, int endIndex)
{
	float x0, y0, z0;
	float x1, y1, z1;
	x0 = m_pos[startIndex * 3 + 0];
	y0 = m_pos[startIndex * 3 + 1];
	z0 = m_pos[startIndex * 3 + 2];

	x1 = m_pos[endIndex * 3 + 0];
	y1 = m_pos[endIndex * 3 + 1];
	z1 = m_pos[endIndex * 3 + 2];

	float sum = 0.0f;
	sum += (x0 - x1) * (x0 - x1);
	sum += (y0 - y1) * (y0 - y1);
	sum += (z0 - z1) * (z0 - z1);

	return std::sqrtf(sum);
}