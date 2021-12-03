#pragma once

class Constraint
{
public:
	Constraint() :
		m_restLength(0.0f), m_startIndex(0.0f), m_endIndex(0.0f), m_stiffness(0.0f)
	{
	}
	Constraint(float rl, int si, int ei, int k) :
		m_restLength(rl), m_startIndex(si), m_endIndex(ei), m_stiffness(k)
	{
	}
public:
	float m_restLength;
	int m_startIndex;
	int m_endIndex;
	float m_stiffness;
};