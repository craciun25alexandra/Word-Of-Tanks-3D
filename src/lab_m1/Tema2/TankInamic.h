#include "Cladiri.h"

namespace m1 {
	class TankInamic
	{
	public:
		glm::vec3 tankPosition;
		glm::vec3 color;
		float rotateBody, rotateTun;
		float randomRotate, randomForward;
		int hp;
		bool alive = true;
		float time;
		float proiectilLastAdded = -1;
		glm::vec2 lastMove = glm::vec2 (3, 0);
		float razaTank = 5 * 0.05f;
		float razaCladire = 2 * 0.2f + 0.15f;
		std ::list<Cladiri*> cladiri;
		TankInamic(float x, float y, float z, glm::vec3 color, std::list<Cladiri*> cladiri,
			float randomForward, float randomRotate) {
			tankPosition.x = x;
			tankPosition.y = y;
			tankPosition.z = z;
			this->color = color;
			this->rotateBody = 0;
			this->rotateTun = 0;
			this->time = 0;
			this->cladiri = cladiri;
			this->randomForward = randomForward;
			this->randomRotate = randomRotate;
			this->hp = 3;
		}

		void MoveForward(float deltaTime) {
			glm::vec3 direction = glm::vec3(sin(M_PI - rotateBody), 0, -cos(M_PI - rotateBody));
			tankPosition -= glm::normalize(direction) * deltaTime;//baga *2.0f
			bool ok = 0;
			if (tankPosition.x > 7 || tankPosition.x < -7 || tankPosition.z < -7 || tankPosition.z > 7)
				ok = true;
			for (Cladiri* cladire : cladiri) {
				float distance = glm::distance(tankPosition, glm::vec3(cladire->x, cladire->y, cladire->z));
				if (distance < razaTank + razaCladire) {
					glm::vec3 dif = (tankPosition - glm::vec3(cladire->x, cladire->y, cladire->z));
					float moduloP = razaTank + razaCladire - distance;
					glm::vec3 p = moduloP * glm::normalize(dif);
					tankPosition += p * 0.3f;
				}
			}
			if (!ok) {
				if (lastMove.x == 1)
					lastMove.y += deltaTime;
				else {
					lastMove.x = 1;
					lastMove.y = 0;

				}

			}
			else {
				lastMove.y++;
				tankPosition += glm::normalize(direction) * deltaTime;
				lastMove.x = 1;
				RotateLeft(2);
				MoveForward(deltaTime);
			}

			
		}
		
		void RotateLeft(float deltaTime) {
			rotateBody += deltaTime * 1.5f;
			if (lastMove.x == 3)
				lastMove.y += deltaTime;
			else {
				lastMove.x = 3;
				lastMove.y = 0;

			}
		}
		

	};
}