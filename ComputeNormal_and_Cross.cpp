

#include <limits>
#include <cmath>
#include <iostream>
template <typename T>
class Vec3 {
public:
	Vec3(){
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}
	Vec3(T _x,T _y, T _z){
		x = _x;
		y = _y;
		z = _z;
	}
	T x, y, z;
};

template <typename T>
inline Vec3<T> Normalize(Vec3<T> v) {

	T length = sqrt(v.x * v.x+ v.y * v.y + v.z * v.z);
	if (abs(length - std::numeric_limits<T>::epsilon()) <= static_cast<T>(0)) {
		length = 1.0f;
	}
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return v;
}
template <typename T>
inline Vec3<T> Cross(Vec3<T> x, Vec3<T> y) {
	Vec3<T> normal;
	normal.x = x.y * y.z - y.y * x.z;
	normal.y = x.z * y.x - y.z * x.x;
	normal.z = x.x * y.y - y.x * x.y;
	return normal;
}

template <typename T>
inline void ComputeNormal(const Vec3<T> &p0,
						  const Vec3<T> &p1, 
						  const Vec3<T> &p2,
						  Vec3<T> &out) {
	Vec3<T> u, v;
	u.x = p1.x - p0.x;
	u.y = p1.y - p0.y;
	u.z = p1.z - p0.z;
	v.x = p2.x - p0.x;
	v.y = p2.y - p0.y;
	v.z = p2.z - p0.z;

	out = Cross(u, v);
	out = Normalize(out);

}
int main(){
	Vec3<float> pos1(0.0f,2.0f,0.0f),pos2(-1.0f,0.0f,-1.0f),pos3(1.0f,0.0f,-1.0f);
	Vec3<float> out;
	ComputeNormal(pos1,pos2,pos3,out);
	std::cout << out.x << " " << out.y << " " << out.z ;
}
