#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(const Frustrum &frustrum, const glm::vec3 &position, float rotation)
{
    this->CameraFrustrum = frustrum;
    this->Rotation = rotation;
    this->Position = position;
    this->RecalculateMatrix();
}

void OrthographicCamera::RecalculateMatrix()
{
    // Projection matrix
    this->ProjectionMatrix = glm::ortho()
}
