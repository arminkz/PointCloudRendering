#include "AABB.h"

AABB::AABB(const glm::vec3& min, const glm::vec3& max) :
	_max(max), _min(min)
{
}

AABB::AABB(const AABB& aabb) :
	_max(aabb._max), _min(aabb._min)
{
}

AABB::~AABB()
{
}

void AABB::update(const glm::vec3& point)
{
	if (point.x < _min.x) { _min.x = point.x; }
	if (point.y < _min.y) { _min.y = point.y; }
	if (point.z < _min.z) { _min.z = point.z; }

	if (point.x > _max.x) { _max.x = point.x; }
	if (point.y > _max.y) { _max.y = point.y; }
	if (point.z > _max.z) { _max.z = point.z; }
}

bool AABB::contains(const glm::vec3& point)
{
	return (point.x >= _min.x) && (point.x <= _max.x) &&
		   (point.y >= _min.y) && (point.y <= _max.y) &&
		   (point.z >= _min.z) && (point.z <= _max.z);
}

bool AABB::testAgainstFrustum(const glm::mat4& MVP)
{
	glm::vec4 corners[8] = {
		{_min.x, _min.y, _min.z, 1.0}, // x y z
		{_max.x, _min.y, _min.z, 1.0}, // X y z
		{_min.x, _max.y, _min.z, 1.0}, // x Y z
		{_max.x, _max.y, _min.z, 1.0}, // X Y z

		{_min.x, _min.y, _max.z, 1.0}, // x y Z
		{_max.x, _min.y, _max.z, 1.0}, // X y Z
		{_min.x, _max.y, _max.z, 1.0}, // x Y Z
		{_max.x, _max.y, _max.z, 1.0}, // X Y Z
	};

	bool inside = false;

	for (size_t corner_idx = 0; corner_idx < 8; corner_idx++) {
		// Transform vertex
		glm::vec4 corner = MVP * corners[corner_idx];
		// Check vertex against clip space bounds
		inside = inside ||
			(-corner.w < corner.x && corner.x < corner.w) &&
			(-corner.w < corner.y && corner.y < corner.w) &&
			(0.0f < corner.z && corner.z < corner.w);
	}
	return inside;
}
