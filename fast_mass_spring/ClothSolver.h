#pragma once

#define EIGEN_USE_MKL_ALL
#define EIGEN_VECTORIZE_SSE4_2

#include "Eigen/Core"
#include "Eigen/Sparse"
#include "Eigen/Dense"

#include "Constraint.h"

class ClothSolver
{
public:
	ClothSolver(int nn, int cn, float* pos, Constraint* cons, float* mass);

	void physicsSimulation();
private:
	void localSolver();
	void globalSolver();

	int m_nodeNum;
	int m_constraintNum;
	float* m_pos;
	Eigen::Map<Eigen::VectorXf> m_currState;
	Eigen::VectorXf m_prevState;
	Constraint* m_constrains;
	float* m_mass;

	// some prefactor matrix
	Eigen::SparseMatrix<float> L; // totalNodeNum * 3, totalNodeNum * 3
	Eigen::SparseMatrix<float> M; // totalNodeNum * 3, totalNodeNum * 3
	Eigen::SparseMatrix<float> J; // totalNodeNum * 3, totalConstrainsNum * 3
	Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> m_cholesky;
	Eigen::VectorXf d;
	Eigen::VectorXf y;
	Eigen::VectorXf b;
	Eigen::VectorXf f_ext;

	// two fix points index and pos
	int m_fixIndex0;
	int m_fixIndex1;
	float m_fixPos0[3];
	float m_fixPos1[3];
};