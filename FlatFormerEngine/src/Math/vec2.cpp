#include "Math/vec2.h"

namespace FlatFormer {

	vec2& vec2::Add(const vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	vec2& vec2::Sub(const vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vec2& vec2::Mul(const vec2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	vec2& vec2::Mul(double factor)
	{
		x *= factor;
		y *= factor;
		return *this;
	}

	vec2& vec2::Div(const vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	vec2& vec2::Div(double divisor)
	{
		x /= divisor;
		y /= divisor;
		return *this;
	}

	double vec2::Length() const
	{
		return sqrt(x * x + y * y);
	}

	double vec2::LengthSquared() const
	{
		return x * x + y * y;
	}

	vec2 operator+(vec2 left, const vec2& right)
	{
		return left.Add(right);
	}

	vec2 operator-(vec2 left, const vec2& right)
	{
		return left.Sub(right);
	}

	vec2 operator-(vec2 vector)
	{
		return vector.Mul(-1);
	}

	vec2 operator*(vec2 left, const vec2& right)
	{
		return left.Mul(right);
	}

	vec2 operator*(vec2 left, double right)
	{
		return left.Mul(right);
	}

	vec2 operator*(double left, vec2 right)
	{
		return right.Mul(left);
	}

	vec2 operator/(vec2 left, const vec2& right)
	{
		return left.Div(right);
	}

	vec2 operator/(vec2 left, double right)
	{
		return left.Div(right);
	}

	bool vec2::operator==(const vec2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool vec2::operator!=(const vec2& other) const
	{
		return !(*this == other);
	}

	bool vec2::operator==(const vec2& other)
	{
		return x == other.x && y == other.y;
	}

	bool vec2::operator!=(const vec2& other)
	{
		return !(*this == other);
	}

	vec2& vec2::operator+=(const vec2& other)
	{
		return Add(other);
	}

	vec2& vec2::operator-=(const vec2& other)
	{
		return Sub(other);
	}

	vec2& vec2::operator*=(const vec2& other)
	{
		return Mul(other);
	}

	vec2& vec2::operator*=(double factor)
	{
		return Mul(factor);
	}

	vec2& vec2::operator/=(const vec2& other)
	{
		return Div(other);
	}

	vec2& vec2::operator/=(double divisor)
	{
		return Div(divisor);
	}

	std::ostream& operator<<(std::ostream& stream, const vec2& vector)
	{
		stream << "vec2 : (" << vector.x << ", " << vector.y << ")";
		return stream;
	}

	double vec2::Dot(const vec2& v1, const vec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	double vec2::Cross(const vec2& v1, const vec2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	vec2 vec2::Cross(const vec2& v, double a)
	{
		return vec2(a * v.y, -a * v.x);
	}

	vec2 vec2::Cross(double a, const vec2& v)
	{
		return vec2(-a * v.y, a * v.x);
	}

	double vec2::Dist(const vec2& v1, const vec2& v2)
	{
		return (v2 - v1).Length();
	}

	double vec2::Angle(const vec2& v1, const vec2& v2)
	{
		if (v1.Length() == 0 || v2.Length() == 0)
			return 0;

		vec2 tmp(round(v1.x * 10000) / 10000, round(v1.y * 10000) / 10000);
		double angle = Dot(tmp, v2) / (v1.Length() * v2.Length());

		return acos(round(angle * 1000) / 1000);
	}

	vec2 vec2::Normalize(const vec2& v)
	{
		if (v.Length() == 0)
			return vec2();
		return (vec2(v) / v.Length());
	}
	
	vec2 vec2::Abs(const vec2& v)
	{
		return vec2(abs(v.x), abs(v.y));
	}

	vec2 vec2::GetLeftNormal(const vec2& v)
	{
		if (v.Length() == 0)
			return vec2();
		return vec2(-v.y, v.x) / v.Length();
	}

	vec2 vec2::GetRightNormal(const vec2& v)
	{
		if (v.Length() == 0)
			return vec2();
		return vec2(v.y, -v.x) / v.Length();
	}

	vec2 vec2::Rotate(const vec2& v, double rad)
	{
		double c = cos(rad);
		double s = sin(rad);
		return vec2(v.x * c - v.y * s, v.x * s + v.y * c);
	}

	vec2 vec2::RotateToAxis(const vec2& v, vec2 axis)
	{
		if (v.Length() == 0)
			return vec2();

		return vec2(Normalize(axis) * v.Length());
	}

	vec2 vec2::Zero() { return vec2(0, 0); };

	vec2 vec2::One() { return vec2(1, 1); };

	vec2 vec2::Up() { return vec2(0, 1); };

	vec2 vec2::Down() { return vec2(0, -1); };

	vec2 vec2::Right() { return vec2(1, 0); };

	vec2 vec2::Left() { return vec2(-1, 0); };

}