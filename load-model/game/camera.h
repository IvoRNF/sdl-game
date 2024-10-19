#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
class FPSCamera
{

    
public:
    FPSCamera()
        : cameraPos{glm::vec3(0.0f, 0.0f, -10.0f)},
          cameraFront{glm::vec3(0.0f, 0.0f, 4.0f)},
          cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)},
          yaw{90.0f},
          pitch{0.0f}
    {
        cameraRight = glm::cross(cameraFront, cameraUp);
    }

    glm::mat4 getViewMatrix()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        direction.y = sin(glm::radians(this->pitch));
        direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
        glm::vec3 target = cameraPos + cameraFront;

        return glm::lookAt(cameraPos, target, cameraUp);
    }

    void processInput(const uint8_t *state)
    {
        this->yawAndPitchMouse();
        const float cameraSpeed = 0.05f; // adjust accordingly
        if (state[SDL_SCANCODE_W])
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (state[SDL_SCANCODE_S])
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (state[SDL_SCANCODE_A])
        {
            cameraPos -= cameraRight * cameraSpeed;
        }
        if (state[SDL_SCANCODE_D])
        {
            cameraPos += cameraRight * cameraSpeed;
        }
        if (state[SDL_SCANCODE_R])
        {
            cameraPos += cameraSpeed * cameraUp;
        }
        if (state[SDL_SCANCODE_F])
        {
            cameraPos -= cameraSpeed * cameraUp;
        }

        if (state[SDL_SCANCODE_T])
        {
            this->yaw += 1.0f;
        }
        if (state[SDL_SCANCODE_G])
        {
            this->yaw -= 1.0f;
        }
        if (state[SDL_SCANCODE_Y])
        {
            this->pitch += 1.0f;
        }
        if (state[SDL_SCANCODE_H])
        {
            this->pitch -= 1.0f;
        }
        if (this->pitch > 89.0f)
        {
            this->pitch = 89.0f;
        }
        if (this->pitch < -89.0f)
        {
            this->pitch = -89.0f;
        }
    }

private:
    void yawAndPitchMouse()
    {
        int mouseX = 0;
        int mouseY = 0;
        SDL_GetRelativeMouseState(&mouseX, &mouseY);
        if (this->firstMouse)
        {
            this->lastMouseX = mouseX;
            this->lastMouseY = mouseY;
            this->firstMouse = false;
        }
        float sensitivity = 0.03f;
        this->mouseChangeX = (mouseX - this->lastMouseX) * sensitivity;
        this->mouseChangeY = (mouseY - this->lastMouseY) * sensitivity;

        this->pitch += -this->mouseChangeY;
        this->yaw += this->mouseChangeX;

        if (this->pitch > 89.0f)
        {
            this->pitch = 89.0f;
        }
        if (this->pitch < -89.0f)
        {
            this->pitch = -89.0f;
        }
    }
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    float yaw;
    float pitch;
    float mouseChangeX;
    float mouseChangeY;

    int lastMouseX;
    int lastMouseY;

    bool firstMouse;
};