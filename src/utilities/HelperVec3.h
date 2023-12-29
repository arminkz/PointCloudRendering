#pragma once
#include "stdafx.h"

namespace Vector3Helper {

	static bool epsilonEquality(const float f1, const float f2)
	{
		return std::abs(f1 - f2) < glm::epsilon<float>();
	}

	static bool equals(const glm::vec3& v1, const glm::vec3& v2) 
	{
		return epsilonEquality(v1.x, v2.x) && epsilonEquality(v1.y, v2.y) && epsilonEquality(v1.z, v2.z);
	}

}