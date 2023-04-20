#ifndef _MATH_VECTOR_H
#define _MATH_VECTOR_H

typedef struct vec2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float asArray[2];
	};

	vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	vec2(float a, float b)
	{
		x = a;
		y = b;
	}

	float& operator[] (int i)
	{
		return asArray[i];
	}
} Vec2;

typedef struct vec3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float asArray[3];
	};

	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}

	float& operator[] (int i)
	{
		return asArray[i];
	}
} Vec3;

Vec2 operator+(const Vec2& l, const Vec2& r);
Vec3 operator+(const Vec3& l, const Vec3& r);
Vec2 operator-(const Vec2& l, const Vec2& r);
Vec3 operator-(const Vec3& l, const Vec3& r);
Vec2 operator*(const Vec2& l, const Vec2& r);
Vec3 operator*(const Vec3& l, const Vec3& r);
Vec2 operator*(const Vec2& l, float r);
Vec3 operator*(const Vec3& l, float r);
bool operator==(const Vec2& l, const Vec2& r);
bool operator==(const Vec3& l, const Vec3& r);
bool operator!=(const Vec2& l, const Vec2& r);
bool operator!=(const Vec3& l, const Vec3& r);

// If the dot product is positive, the vectors are pointing in the same direction
// If the dot product is negative, the vectors point in opposing directions
// If the dot product is 0, the vectors are perpendicular
float Dot(const Vec2& l, const Vec2& r);
float Dot(const Vec3& l, const Vec3& r);
// length == magnitude
float Magnitude(const Vec2& v);
float Magnitude(const Vec3& v);
float MagnitudeSq(const Vec2& v);
float MagnitudeSq(const Vec3& v);
float Distance(const Vec2& p1, const Vec2& p2);
float Distance(const Vec3& p1, const Vec3& p2);
void Normalize(Vec2& v);
void Normalize(Vec3& v);
Vec2 Normalized(const Vec2& v);
Vec3 Normalized(const Vec3& v);
Vec3 Cross(const Vec3& l, const Vec3& r);
float Angle(const Vec2& l, const Vec2& r);
float Angle(const Vec3& l, const Vec3& r);
Vec2 Project(const Vec2& length, const Vec2& direction);
Vec3 Project(const Vec3& length, const Vec3& direction);
Vec2 Perpendicular(const Vec2& len, const Vec2& dir);
Vec3 Perpendicular(const Vec3& len, const Vec3& dir);
Vec2 Reflection(const Vec2& vec, const Vec2& normal);
Vec3 Reflection(const Vec3& vec, const Vec3& normal);
#endif // _MATH_VECTOR_H