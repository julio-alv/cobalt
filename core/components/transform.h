#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
    glm::mat4 transform{glm::mat4(1.f)};
    glm::vec3 position{glm::vec3(0.f)};
    glm::quat rotation{glm::vec3(0.f)};
    glm::vec3 scale{glm::vec3(1.f)};

    Transform(
        const glm::vec3 &p,
        const glm::vec3 &r,
        const glm::vec3 &s) : position(p),
                              rotation(r),
                              scale(s)
    {
        Calculate();
    }

    void Calculate()
    {
        glm::mat4 p = glm::translate(transform, position);
        glm::mat4 r = glm::toMat4(rotation);
        glm::mat4 s = glm::scale(transform, scale);

        transform = p * r * s;
    }
    void Calculate(const glm::mat4 &parent)
    {
        glm::mat4 p = glm::translate(transform, position);
        glm::mat4 r = glm::toMat4(rotation);
        glm::mat4 s = glm::scale(transform, scale);

        transform = parent * p * r * s;
    }
};
