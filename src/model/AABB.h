#pragma once
#include "stdafx.h"

class AABB
{
protected:
	glm::vec3 _min, _max;

public:

	AABB(const glm::vec3& min = glm::vec3(INFINITY), const glm::vec3& max = glm::vec3(-INFINITY));

	AABB(const AABB& aabb);

	virtual ~AABB();

	void update(const glm::vec3& point);

	bool contains(const glm::vec3& point);

	bool testAgainstFrustum(const glm::mat4& MVP);


	glm::vec3 center() const { return (_min + _max) / 2.f; }

	glm::vec3 min() const { return _min; }

	glm::vec3 max() const { return _max; }
};