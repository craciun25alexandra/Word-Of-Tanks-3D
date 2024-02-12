namespace m1 {
	class Proiectil
	{
	public:
		float x, y, z;
		float time;
		float rotate;
		bool alive = true;
		Proiectil(float x, float y, float z, float rotate) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->time = 3;
			this->rotate = rotate;
		}
	};

}