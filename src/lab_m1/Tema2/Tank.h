namespace m1 {
	class Tank
	{
		public:
			float x, y, z;
			float hp = 10;
			bool alive = true;
		Tank(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}	
	};
}