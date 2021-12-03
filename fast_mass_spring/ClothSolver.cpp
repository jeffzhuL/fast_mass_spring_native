#include "ClothSolver.h"
#include "Config.h"
#include <vector>

ClothSolver::ClothSolver(int nn, int cn, float* pos, Constraint* cons, float* mass) :
	m_currState(pos, nn * 3), m_prevState(nn * 3), m_nodeNum(nn), m_constraintNum(cn), m_mass(mass), m_constrains(cons),
	m_pos(pos), d(cn * 3), y(nn * 3), b(nn * 3), f_ext(nn * 3)
{
	m_fixIndex0 = 0;
	m_fixIndex1 = CLOTH_ROW_NUM * (CLOTH_ROW_NUM - 1);
	m_fixPos0[0] = pos[m_fixIndex0 * 3 + 0];
	m_fixPos0[1] = pos[m_fixIndex0 * 3 + 1];
	m_fixPos0[2] = pos[m_fixIndex0 * 3 + 2];

	m_fixPos1[0] = pos[m_fixIndex1 * 3 + 0];
	m_fixPos1[1] = pos[m_fixIndex1 * 3 + 1];
	m_fixPos1[2] = pos[m_fixIndex1 * 3 + 2];

	m_prevState = m_currState;

	for (int i = 0; i < nn; i++)
	{
		f_ext[i * 3 + 1] = m_mass[i] * GRAVITY;
	}

	// now we pre-calculate the M, L, J
	M.resize(3 * m_nodeNum, 3 * m_nodeNum);
	std::vector<Eigen::Triplet<float>> MTriplets;
	for (int i = 0; i < m_nodeNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			MTriplets.push_back(Eigen::Triplet<float>(3 * i + j, 3 * i + j, m_mass[i]));
		}
	}
	M.setFromTriplets(MTriplets.begin(), MTriplets.end());

	L.resize(3 * m_nodeNum, 3 * m_nodeNum);
	std::vector<Eigen::Triplet<float>> LTriplets;
	for (int i = 0; i < m_constraintNum; i++)
	{
		Constraint tConstraint = m_constrains[i];
		int si = tConstraint.m_startIndex;
		int ei = tConstraint.m_endIndex;
		float k = tConstraint.m_stiffness;

		for (int j = 0; j < 3; j++)
		{
			LTriplets.push_back(Eigen::Triplet<float>(3 * si + j, 3 * si + j, k));
			LTriplets.push_back(Eigen::Triplet<float>(3 * si + j, 3 * ei + j, -k));
			LTriplets.push_back(Eigen::Triplet<float>(3 * ei + j, 3 * ei + j, k));
			LTriplets.push_back(Eigen::Triplet<float>(3 * ei + j, 3 * si + j, -k));
		}
	}
	L.setFromTriplets(LTriplets.begin(), LTriplets.end());

	J.resize(3 * m_nodeNum, 3 * m_constraintNum);
	std::vector<Eigen::Triplet<float>> JTriplets;
	for (int i = 0; i < m_constraintNum; i++)
	{
		Constraint tConstraint = m_constrains[i];
		int si = tConstraint.m_startIndex;
		int ei = tConstraint.m_endIndex;
		float k = tConstraint.m_stiffness;

		for (int j = 0; j < 3; j++)
		{
			JTriplets.push_back(Eigen::Triplet<float>(3 * si + j, 3 * i + j, -k));
			JTriplets.push_back(Eigen::Triplet<float>(3 * ei + j, 3 * i + j, k));
		}
	}
	J.setFromTriplets(JTriplets.begin(), JTriplets.end());

	float h2 = (DELTA_TIME) * (DELTA_TIME);
	Eigen::SparseMatrix<float> Q = M + h2 * L;
	m_cholesky.compute(Q);
}

void ClothSolver::localSolver()
{
	for (int i = 0; i < m_constraintNum; i++)
	{
		Constraint tConstraint = m_constrains[i];
		int si = tConstraint.m_startIndex;
		int ei = tConstraint.m_endIndex;
		float rl = tConstraint.m_restLength;

		float d0 = m_pos[ei * 3 + 0] - m_pos[si * 3 + 0];
		float d1 = m_pos[ei * 3 + 1] - m_pos[si * 3 + 1];
		float d2 = m_pos[ei * 3 + 2] - m_pos[si * 3 + 2];

		Eigen::Vector3f dir;
		dir << d0, d1, d2;
		dir.normalize();

		d[i * 3 + 0] = dir[0] * rl;
		d[i * 3 + 1] = dir[1] * rl;
		d[i * 3 + 2] = dir[2] * rl;
	}
}

void ClothSolver::globalSolver()
{
	float h2 = (DELTA_TIME) * (DELTA_TIME);
	b = h2 * J * d + y + h2 * f_ext;

	m_currState = m_cholesky.solve(b);
}

void ClothSolver::physicsSimulation()
{
	y = M * ((1 + DAMPING) * m_currState - DAMPING * m_prevState);
	m_prevState = m_currState;

	for (int i = 0; i < ITERATION_NUM; i++)
	{
		localSolver();
		globalSolver();
	}
	
	m_currState[m_fixIndex0 * 3 + 0] = m_fixPos0[0];
	m_currState[m_fixIndex0 * 3 + 1] = m_fixPos0[1];
	m_currState[m_fixIndex0 * 3 + 2] = m_fixPos0[2];

	m_currState[m_fixIndex1 * 3 + 0] = m_fixPos1[0];
	m_currState[m_fixIndex1 * 3 + 1] = m_fixPos1[1];
	m_currState[m_fixIndex1 * 3 + 2] = m_fixPos1[2];
}