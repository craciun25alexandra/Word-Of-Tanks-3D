namespace m1 {
	class Cladiri
	{
		public:
			float x, y, z;
			Mesh* corp;
			float razaCladire = 2 * 0.2f + 0.05;

			Cladiri(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
			int InBuiliding(float x, float z, float raza) {
				float distance = glm::distance(glm::vec3(x, 0, z), glm::vec3(this->x, this->y, this->z));
				if (distance < raza + razaCladire)
					return 1;
				return 0;
			}
	};

}