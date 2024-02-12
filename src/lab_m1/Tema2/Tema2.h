#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera2.h"
#include "Tank.h"
#include "TankInamic.h"
#include "Proiectil.h"
#include "components/transform.h"
#include "text.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), int tankHP = 3);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool IsInBorder(glm::vec3 tankPosition);
     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool is_ortho;
        int score = 0;
        gfxc::TextRenderer* tr;
        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        glm::vec3 tankPosition;
        glm::vec3 tunPosition;
        std:: list<Cladiri*> cladiri;
        std:: list<Proiectil*> proiectile;
        std::list<Proiectil*> proiectileInamice;
        int tankHP = 10;
        std:: list<TankInamic*> inamici;
        Tank* t;
        float timer;
        float razaTank = 5 * 0.05f;
        float razaCladire = 2 * 0.2f + 0.15f;
        float TunX, TunY, TunZ;
        float rotateBody, rotateTun;
        float razaDistanta;
        float lastProiectilAdded;
        float x_powerup, z_powerup;
        bool power_up_alive = false;
        float power_up_time = 0;
 
        bool start = false;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
