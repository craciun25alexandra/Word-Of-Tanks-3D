#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{


    camera = new implemented::Camera();
    timer = 0;
    rotateBody = 0;
    rotateTun = 0;
    tankPosition = glm::vec3(0, 0, 7);
    camera->Set(tankPosition + glm::vec3(0, tankPosition.y + 0.3f, 3), tankPosition, glm::vec3(0, 1, 0));
    lastProiectilAdded = -1;

    t = new Tank(0, 0, 0);
    tunPosition = glm::vec3(0, 0, 0);
    //generare cladiri
    while (cladiri.size() < 15) {
        cladiri.push_back(new Cladiri(rand() % 13 - 6, 0, rand() % 11 - 4));
    }
    cladiri.push_back(new Cladiri(0, 0, 0));
    //generare inamici
    //coord random pt inamici, timp random de mers fata/spate
    while (inamici.size() < 5) {
        float x = rand() % 13 - 6;
        float z = rand()%6 - 7;
        float randomTimeForward = rand() % 4 + 3;
        float randomTimeRotate = rand() % 2 + 1;
        for (Cladiri* cladire : cladiri)
			if (cladire->InBuiliding(x, z, 5 * 0.05f))
				continue;
        for (TankInamic* i : inamici)
            if (i->tankPosition.x == x) continue;
        inamici.push_back(new TankInamic(x, 0, z, glm::vec3(0, 0, 1), cladiri, randomTimeForward, randomTimeRotate));
    }
    {
        Mesh* mesh = new Mesh("senilaST");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senilaST.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("senilaDR");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senilaDR.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("cladireCap");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladireCap.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cladireCorpBun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladireCorpBun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("cap");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cap.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Mesh* mesh = new Mesh("proiectil");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 2, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
        
    }
    tankHP = 3;
    tr = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    tr->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
bool Tema2::IsInBorder(glm::vec3 tankPosition) {
    return (tankPosition.x > 7 || tankPosition.x < -7 || tankPosition.z < -7 || tankPosition.z > 7);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if(!start) {
		tr->RenderText("Press space to start", 250, 300, 0.5f);
		return;
	}
    else 
        start = true;
   
    if (!t->alive || timer > 100 || score == 5) {
        if(score< 5 )
            tr->RenderText("Game Over", 300 , 100 , 1);
        else 
            tr->RenderText("You Won!^^", 300, 100, 1);
        tr->RenderText("score : ", 400, 400, 0.5f);
        switch (score) {
        case 0:
            tr->RenderText("0/5", 700, 400, 0.5f);
            break;
        case 1:
            tr->RenderText("1/5", 700, 400, 0.5f);
            break;
        case 2:
            tr->RenderText("2/5", 700, 400, 0.5f);
            break;
        case 3:
            tr->RenderText("3/5", 700, 400, 0.5f);
            break;
        case 4:
            tr->RenderText("4/5", 700, 400, 0.5f);
            break;
        case 5:
            tr->RenderText("5/5", 700, 400, 0.5f);
            break;
        }
        tr->RenderText("For exit press ESC", 300, 500, 0.5f);
        return;
    }
     //powertea  
    if (power_up_alive == false && timer - power_up_time > 15) {
		x_powerup = -7;
		z_powerup = 7;
		power_up_alive = true;
	}
    else if (power_up_alive) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x_powerup, tankPosition.y, z_powerup));
        RenderSimpleMesh(meshes["teapot"], shaders["LabShader"], modelMatrix, glm::vec3(0.60000, 1.00000, 0.20000), 3);

    }
    float distance = glm::distance(glm::vec3(x_powerup, tankPosition.y, z_powerup), tankPosition);
    if (distance < razaTank + 0.1f) {
        power_up_alive = false;
        if (t->hp < 10)
            t->hp += 3;
        power_up_time = timer;
    }
    
    lightPosition = tankPosition + glm::vec3(0,2,0); //lumina pe tank
    timer+= deltaTimeSeconds;
    for (Proiectil* proiectil : proiectile) {
        //coleziunea proiectilului cu cladirea
        for (Cladiri* cladire : cladiri) {
			float distance = glm::distance(glm::vec3(proiectil->x, proiectil->y, proiectil->z), glm::vec3(cladire->x, cladire->y, cladire->z));
            if (distance < 2 * 0.2f + 0.05 + 0.1f) {
				proiectil->alive = false;
			}
		}
        //coleziunea proiectilului cu inamicul
        for(TankInamic* t : inamici) {
        	float distance = glm::distance(glm::vec3(proiectil->x, proiectil->y, proiectil->z), glm::vec3(t->tankPosition.x, t->tankPosition.y, t->tankPosition.z));
            if (distance < razaTank + 0.1f) {
				proiectil->alive = false;
				t->hp --;
                if(t->hp == 0)
					t->alive = false;
			}
        }
		proiectil->time -= deltaTimeSeconds;
        if (proiectil->time > 0 && proiectil->alive) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(proiectil->x, proiectil->y, proiectil->z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
            RenderSimpleMesh(meshes["proiectil"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0), 3);
            proiectil->x -= sin(proiectil->rotate) * deltaTimeSeconds *2.0f;
            proiectil->z -= cos(proiectil->rotate) * deltaTimeSeconds *2.0f;
        }
        else proiectil->alive = false;

	}
    for (Proiectil* proiectil : proiectile) {
        if (!proiectil->alive) {
            proiectile.remove(proiectil);
            break;
        }
	}
    //randare cladiri
    for (Cladiri* cladire : cladiri) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(cladire->x, cladire->y, cladire->z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cladireCorpBun"], shaders["LabShader"], modelMatrix, glm::vec3(0.80000, 0.20000, 0.00000), 3);
	}
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cladireCap"], shaders["LabShader"], modelMatrix, glm::vec3(0.40000, 0.20000, 0.00000), 3);
    }
    {
        if (t->alive) {
            //randare tank
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, tankPosition);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
            modelMatrix = glm::rotate(modelMatrix, rotateBody, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["senilaDR"], shaders["LabShader"], modelMatrix, glm::vec3(0.949, 0.706, 0.894),  t->hp);
            RenderSimpleMesh(meshes["senilaST"], shaders["LabShader"], modelMatrix, glm::vec3(0.949, 0.706, 0.894),  t->hp);
            RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, glm::vec3(0.949, 0.706, 0.894), t->hp);

   
            glm::mat4 modelMatrix2 = glm::mat4(1);
            modelMatrix2 = glm::translate(modelMatrix2, tankPosition);
            modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.05f));
            modelMatrix2 = glm::rotate(modelMatrix2, rotateTun - RADIANS(90) + rotateBody, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix2, glm::vec3(0.949, 0.706, 0.894), t->hp);
            RenderSimpleMesh(meshes["cap"], shaders["LabShader"], modelMatrix2, glm::vec3(1.00000, 1, 0), t->hp);
        }
    };

    {
  
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0,0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 0.7), 3);
    }
    for (TankInamic* inamic : inamici) {
        if(!inamic->alive)
			continue;
        float distance = glm::distance(tankPosition, inamic->tankPosition);
        //daca se afla la dist la tank mai mica de 3 ataca
        if (distance < 3 && t->alive) {
            glm::vec3 direction = glm::normalize(tankPosition - inamic->tankPosition);
            float angle = acos(glm::dot(direction, glm::vec3(0, 0, 1)));
            if (tankPosition.x < inamic->tankPosition.x)
                angle = -angle;
            inamic->rotateTun = angle;
            if (timer - inamic->proiectilLastAdded >= 2) {
                glm::vec3 endCoords;
                endCoords.x = inamic->tankPosition.x - 4 * 0.05 * 2 * glm::sin(inamic->rotateTun - RADIANS(180));
                endCoords.y = inamic->tankPosition.y + 4 * 0.05;
                endCoords.z = inamic->tankPosition.z - 4 * 0.05 * 2 * glm::cos(inamic->rotateTun - RADIANS(180));
                proiectileInamice.push_back(new Proiectil(endCoords.x, endCoords.y, endCoords.z, inamic->rotateTun - RADIANS(180)));
                inamic->proiectilLastAdded = timer;
            }
        }
        //randare inamici
        glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, inamic->tankPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
		modelMatrix = glm::rotate(modelMatrix, inamic->rotateBody, glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, inamic->color, inamic->hp);
        RenderSimpleMesh(meshes["senilaDR"], shaders["LabShader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f), 5-inamic->hp);
        RenderSimpleMesh(meshes["senilaST"], shaders["LabShader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f), 5 - inamic->hp);
        glm::mat4 modelMatrix2 = glm::mat4(1);
        modelMatrix2 = glm::translate(modelMatrix2, inamic->tankPosition);
        modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.05f));
        modelMatrix2 = glm::rotate(modelMatrix2, inamic->rotateTun - RADIANS(180), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix2, glm::vec3(0.5f, 0.5f, 0.5f), inamic->hp);
        RenderSimpleMesh(meshes["cap"], shaders["LabShader"], modelMatrix2, inamic->color, inamic->hp);
        
        if (inamic->lastMove.x == 1)
            if(inamic->lastMove.y <= inamic->randomForward )
                inamic->MoveForward(deltaTimeSeconds);
            else
                inamic->RotateLeft(deltaTimeSeconds); //daca se blocheaza se duc in stanga
            
        else if (inamic->lastMove.x == 3) //daca e pe stanga
            if (inamic->lastMove.y <= inamic->randomRotate)  //max 2 sec
                inamic->RotateLeft(deltaTimeSeconds);
            else
                inamic->MoveForward(deltaTimeSeconds); //dupa se duce in spate
    }
    for (TankInamic* inamic : inamici) {
        if (!inamic->alive) {
            score++;
			inamici.remove(inamic);
			break;
		}
	}
    for (Proiectil* proiectil : proiectileInamice) {
        //coleziune proiectilInamic-cladire
        for (Cladiri* cladire : cladiri) {
            float distance = glm::distance(glm::vec3(proiectil->x, proiectil->y, proiectil->z), glm::vec3(cladire->x, cladire->y, cladire->z));
            if (distance < 2 * 0.2f + 0.05 + 0.1f) {
                proiectil->alive = false;
            }
        }
        //coleziune proiectilInamic-tank
        float distance = glm::distance(glm::vec3(proiectil->x, proiectil->y, proiectil->z), tankPosition);
        if (distance < razaTank + 0.1f && proiectil->alive) {
            t->hp--;
            if(t->hp == 0 )
                t->alive = false;
            proiectil->alive = false;
		}
        proiectil->time -= deltaTimeSeconds;
        //randare proiectilInamic
        if (proiectil->time > 0 && proiectil->alive) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(proiectil->x, proiectil->y, proiectil->z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
            RenderSimpleMesh(meshes["proiectil"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0.4f), 3);
            proiectil->x -= sin(proiectil->rotate) * deltaTimeSeconds * 2.0f;
            proiectil->z -= cos(proiectil->rotate) * deltaTimeSeconds * 2.0f;
        }
        else proiectil->alive = false;
    }
    for (Proiectil* proiectil : proiectileInamice) {
        if (!proiectil->alive) {
            proiectile.remove(proiectil);
            break;
        }
    }
    //coleziune tank-tank
    for (TankInamic* t1 : inamici) {
        for (TankInamic* t2 : inamici)
            if (t1 != t2) {
                float distance = glm::distance(t1->tankPosition, t2->tankPosition);
                if (distance < 2 * razaTank) {
                    glm::vec3 dif = (t2->tankPosition - t1->tankPosition);
                    float moduloP = abs(2 * razaTank - distance);
                    glm::vec3 p = moduloP * glm::normalize(dif);
                    if (!IsInBorder(t1->tankPosition - p * 0.3f))
						t1->tankPosition += p * -0.5f;
                    else {
                        t1->RotateLeft(deltaTimeSeconds);
                    }
                    if (!IsInBorder(t2->tankPosition + p * 0.3f))
                           t2->tankPosition += p * 0.5f;
                    else {
                        t2->RotateLeft(deltaTimeSeconds);
                    }
                }
            }
        float distance = glm::distance(t1->tankPosition, tankPosition);
        if (distance < 2 * razaTank) {
            glm::vec3 dif = (t1->tankPosition - tankPosition);
            float moduloP = abs(2 * razaTank - distance);
            glm::vec3 p = moduloP * glm::normalize(dif);
            if(!IsInBorder(t1->tankPosition + p * 0.5f))
                t1->tankPosition += p * 0.5f;
            else t1->RotateLeft(deltaTimeSeconds);
            if (!IsInBorder(tankPosition + p * -0.5f))
                tankPosition += p * -0.5f;
            
        }
    }

    camera->SetDependingTank(tankPosition);
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 1.5f;
    float cameraSpeed = 1.5f;
    if (window->KeyHold(GLFW_KEY_W)) {
        glm::vec3 direction = glm::vec3(sin(M_PI - rotateBody), 0, -cos(M_PI - rotateBody));
        tankPosition -= glm::normalize(direction) * deltaTime * speed;
        for (Cladiri* cladire : cladiri) {
            float distance = glm::distance(tankPosition, glm::vec3(cladire->x, cladire->y, cladire->z));
            if (distance < razaTank + razaCladire) {
                glm::vec3 dif = (tankPosition - glm::vec3(cladire->x, cladire->y, cladire->z));
                float moduloP = abs(razaTank + razaCladire - distance);
                glm::vec3 p = moduloP * glm::normalize(dif);
                tankPosition += p * 0.3f;
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        float initialRot = rotateBody;
        rotateBody += deltaTime * cameraSpeed;
        camera->RotateThirdPerson_OY(rotateBody - initialRot);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        glm::vec3 direction = glm::vec3(sin(M_PI - rotateBody), 0, -cos(M_PI - rotateBody));
        tankPosition += glm::normalize(direction) * deltaTime * speed;
        for (Cladiri* cladire : cladiri) {
            float distance = glm::distance(tankPosition, glm::vec3(cladire->x, cladire->y, cladire->z));
            if (distance < razaTank + razaCladire) {
                glm::vec3 dif = (tankPosition - glm::vec3(cladire->x, cladire->y, cladire->z));
                float moduloP = abs(razaTank + razaCladire - distance);
                glm::vec3 p = moduloP * glm::normalize(dif);
                tankPosition += p * 0.3f;
            }
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        float initialRot = rotateBody;
        rotateBody -= deltaTime * cameraSpeed;
        camera->RotateThirdPerson_OY(rotateBody - initialRot);
    }
    
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        start = true;
        timer = 0;
    }
    if(key == GLFW_KEY_ESCAPE)
        exit(0);
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
        }
    }
    else {

        // Add mouse move event
        int mouse_y = window->GetResolution().y - mouseY;
       
        //vreau sa se miste tunul unde e mouse ul folosindu ma de rotatetun
        rotateTun = atan2((mouse_y - window->GetResolution().y*3/4) * 3.5, mouseX - window->GetResolution().x / 2);
        
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    //adauga eveniment daca se apasa click stanga
    if (button == 1) {
        //adauga proiectil jucator
        if (timer - lastProiectilAdded >= 1) {
            glm::vec3 endCoords;
            endCoords.x = tankPosition.x - 4 * 0.05 * 2* glm::sin(rotateTun - RADIANS(90) + rotateBody);
            endCoords.y = tankPosition.y + 4 * 0.05; 
            endCoords.z = tankPosition.z - 4 * 0.05 * 2 * glm::cos(rotateTun - RADIANS(90) + rotateBody);
            proiectile.push_back(new Proiectil(endCoords.x, endCoords.y, endCoords.z, rotateTun - RADIANS(90) + rotateBody));
            lastProiectilAdded = timer;
        }
        
	}
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int tankHP)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // TODO(student): Set light position uniform
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));


    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
    materialShininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(materialShininess, materialShininess);

    int tankHPUniformLocation = glGetUniformLocation(shader->program, "tankHP");
    glUniform1i(tankHPUniformLocation, tankHP);


    materialShininess = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(materialShininess, materialKd);

    materialShininess = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(materialShininess, materialKs);

    GLint loc_object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc_object_color, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}
