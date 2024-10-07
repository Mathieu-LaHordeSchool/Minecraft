#include "Physics/BoxCollision.h"

bool BoxCollision::InCollision(glm::vec3 p)
{
	glm::vec3 point1 = p1 + position;
	glm::vec3 point2 = p2 + position;

    return InCollision(p, point1, point2);
}

bool BoxCollision::InIntersection(glm::vec3 start, glm::vec3 end, glm::vec3& intersectionPoint)
{
    glm::vec3 point1 = p1 + position;
    glm::vec3 point2 = p2 + position;

    return InInteraction(start, end, point1, point2, intersectionPoint);
}

bool BoxCollision::InInteraction(glm::vec3 start, glm::vec3 end, glm::vec3 point1, glm::vec3 point2, glm::vec3& intersection)
{
    glm::vec3 dir = end - start;
    float length = glm::length(dir);
    dir = glm::normalize(dir);

    // Perform ray-cube intersection test
    glm::vec3 invDir = 1.0f / dir;
    glm::vec3 tmin = (point1 - start) * invDir;
    glm::vec3 tmax = (point2 - start) * invDir;

    glm::vec3 realMin = glm::min(tmin, tmax);
    glm::vec3 realMax = glm::max(tmin, tmax);

    float tNear = glm::max(glm::max(realMin.x, realMin.y), realMin.z);
    float tFar = glm::min(glm::min(realMax.x, realMax.y), realMax.z);

    // If the ray intersects the cube
    if (tNear <= tFar && tFar >= 0 && tNear <= length) {
        intersection = start + dir * tNear;
        return true;
    }

    return false;
}
bool BoxCollision::InCollision(glm::vec3 point, glm::vec3 point1, glm::vec3 point2)
{
    // Vérifier si p est entre point1 et point2 dans chaque dimension
    bool betweenX = (point.x >= std::min(point1.x, point2.x)) && (point.x <= std::max(point1.x, point2.x));
    bool betweenY = (point.y >= std::min(point1.y, point2.y)) && (point.y <= std::max(point1.y, point2.y));
    bool betweenZ = (point.z >= std::min(point1.z, point2.z)) && (point.z <= std::max(point1.z, point2.z));

    // Retourner vrai si p est entre point1 et point2 dans toutes les dimensions
    return betweenX && betweenY && betweenZ;
}
