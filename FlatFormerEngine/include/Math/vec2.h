#pragma once

#include <iostream>
#include <cmath>

namespace FlatFormer {

	using FFString = std::string;

	struct vec2
	{
		double x, y;

		vec2() : x(0.0f), y(0.0f) {}
		vec2(const vec2& v) : x(v.x), y(v.y) {}
		vec2(double x, double y = 0.0f) : x(x), y(y) {}

		vec2& Add(const vec2& other);
		vec2& Sub(const vec2& other);
		vec2& Mul(const vec2& other);
		vec2& Mul(double factor);
		vec2& Div(const vec2& other);
		vec2& Div(double divisor);

		double Length() const;
		double LengthSquared() const;

		friend vec2 operator+ (vec2 left, const vec2& right);
		friend vec2 operator- (vec2 left, const vec2& right);
		friend vec2 operator- (vec2 vector);
		friend vec2 operator* (vec2 left, const vec2& right);
		friend vec2 operator* (vec2 left, double right);
		friend vec2 operator* (double left, vec2 right);
		friend vec2 operator/ (vec2 left, const vec2& right);
		friend vec2 operator/ (vec2 left, double right);

		friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);

		bool operator==	(const vec2& other) const;
		bool operator!=	(const vec2& other) const;

		bool operator==	(const vec2& other);
		bool operator!=	(const vec2& other);

		vec2& operator+= (const vec2& other);
		vec2& operator-= (const vec2& other);
		vec2& operator*= (const vec2& other);
		vec2& operator*= (double factor);
		vec2& operator/= (const vec2& other);
		vec2& operator/= (double divisor);


		static double Dot(const vec2& v1, const vec2& v2);
		static double Cross(const vec2& v1, const vec2& v2);
		static vec2 Cross(const vec2& v1, double a);
		static vec2 Cross(double a, const vec2& v1);
		static double Dist(const vec2& v1, const vec2& v2);
		static double Angle(const vec2& v1, const vec2& v2);

		static vec2 Abs(const vec2& v);
		static vec2 Normalize(const vec2& v);
		static vec2 GetLeftNormal(const vec2& v);
		static vec2 GetRightNormal(const vec2& v);
		static vec2 Rotate(const vec2& v, double radians);
		static vec2 RotateToAxis(const vec2& v, vec2 axis);

		static vec2 Zero();
		static vec2 One();
		static vec2 Up();
		static vec2 Down();
		static vec2 Right();
		static vec2 Left();

		template<typename T> struct hash;

		template<>
		struct hash<vec2> {
			size_t operator() (const vec2 & arg) const
			{

				size_t h1 = std::hash<double>{}(arg.x);
				size_t h2 = std::hash<double>{}(arg.y);

				return h1 ^ (h2 >> 1) ^ (h2 << (sizeof(size_t)*CHAR_BIT - 1));
			}
		};

		template<typename T> struct comp;

		template<>
		struct comp<vec2> {
			bool operator() (const vec2& a, const vec2& b) const
			{
				return std::make_pair(a.x, a.y) > std::make_pair(b.x, b.y);
			}

		};
	};

}