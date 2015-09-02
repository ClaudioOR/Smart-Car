#ifndef COLLISIONDETECTION
#define COLLISIONDETECTION

#include <vector>
#include "../Rendering/VertexFormat.h"

bool intersectTriangles(std::vector<Rendering::VertexFormat>& vertsA, glm::mat4& modelA, std::vector<Rendering::VertexFormat>& vertsB, glm::mat4& modelB);

#endif