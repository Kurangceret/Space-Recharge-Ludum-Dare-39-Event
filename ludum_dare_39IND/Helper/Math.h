#ifndef MATH_H
#define MATH_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <assert.h>
#include <cmath>

namespace Math {
	inline float toDegree(float radian)
	{
		return 180.f / 3.141592653589793238462643383f * radian;
	}

	/*
	There are π radians in a half circle
	And also 180° in a half circle

	So π radians = 180°

	So 1 radian = 180°/π = 57.2958° (approximately)*/
	inline float toRadian(float degree)
	{
		return 3.141592653589793238462643383f / 180.f * degree;
	}

	inline sf::Vector2f degreeToVector(float degree)
	{
		float radians = toRadian(degree);

		sf::Vector2f v;
		if (std::abs(degree) != 270.f && std::abs(degree) != 90.f)
			v.x = std::cos(radians);
		if (std::abs(degree) != 180.f && std::abs(degree) != 360.f)
			v.y = std::sin(radians);

		//if (std::abs(v.x) > 0.f)v.x = 0.f;
		//if (std::abs(v.y) > 0.f)v.y = 0.f;

		return v;
	}

	inline float returnTheLargest(const float v1, const float v2)
	{
		assert(v1 != v2);
		return (v1 > v2) ? v1 : v2;
	}

	inline float vectorToDegree(const sf::Vector2f& vector, bool fullArcResult = true)
	{
		float initial = toDegree(std::atan2(vector.y, vector.x));
		if (fullArcResult && initial < 0.f)
			initial = 360.f - std::abs(initial);
		return initial;
	}

	inline float fullArcDegreeToSfDegree(float degree)
	{
		if (degree >= 180.f) degree -= 360.f;
		return degree;
	}

	//

	inline float plusDegree(float deg1, float deg2)
	{
		float finalDeg = deg1 + deg2;
		while (finalDeg >= 360.0)
			finalDeg -= 360.0;
		return finalDeg;
	}

	inline float minusDegree(float deg1, float deg2)
	{
		float finalDeg = deg1 - deg2;
		if (finalDeg < 0.f)
			finalDeg = 360.0 + finalDeg;
		return finalDeg;
	}

	inline float getDotProduct(const sf::Vector2f& left, const sf::Vector2f& right)
	{
		return ((left.x * right.x) + (left.y * right.y));
	}

	inline float getDotProductDegree(const sf::Vector2f& firstDir, const sf::Vector2f& secondDir)
	{
		return Math::toDegree(std::acos(Math::getDotProduct(firstDir, secondDir)));
	}

	inline sf::Vector2i absoluteVectori(const sf::Vector2i& vec)
	{
		sf::Vector2i newV;
		newV.x = std::abs(vec.x);
		newV.y = std::abs(vec.y);
		return newV;
	}

	inline sf::Vector2f absoluteVectorf(const sf::Vector2f& vec)
	{
		sf::Vector2f newV;
		newV.x = std::abs(vec.x);
		newV.y = std::abs(vec.y);
		return newV;
	}

	inline float vectorLength(sf::Vector2f vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y);
		//return Q_rsqrt(vector.x * vector.x + vector.y * vector.y);
	}

	inline sf::Vector2f minusVector(const sf::Vector2f& vector1,
		const sf::Vector2f& vector2)
	{
		return vector1 - vector2;
	}

	inline sf::Vector2i minusVectorI(const sf::Vector2i& vector1,
		const sf::Vector2i& vector2)
	{
		return vector1 - vector2;
	}

	inline sf::Vector2f plusVector(const sf::Vector2f& vector1,
		const sf::Vector2f& vector2)
	{
		return vector1 + vector2;
	}

	inline sf::Vector2i plusVectorI(const sf::Vector2i& vector1,
		const sf::Vector2i& vector2)
	{
		return vector1 + vector2;
	}

	inline sf::Vector2f multiplyVector(const sf::Vector2f& vector1,
		const sf::Vector2f& vector2)
	{
		return sf::Vector2f(vector1.x * vector2.x, vector1.y * vector2.y);
	}

	inline sf::Vector2f multiplyVectorByFloat(const sf::Vector2f& vector1,
		float length)
	{
		return vector1 * length;
	}

	inline float hattanLength(const sf::Vector2f& vector)
	{
		return std::abs(vector.x) + std::abs(vector.y);
	}

	inline sf::Vector2f unitVector(const sf::Vector2f& vector)
	{
		if (vector == sf::Vector2f(0.f, 0.f))
			return vector;
		return vector / vectorLength(vector);
	}

	inline sf::Vector2f unitVector(const sf::Vector2f& vector, float vectorLength)
	{
		if (vector == sf::Vector2f(0.f, 0.f))
			return vector;
		return vector / vectorLength;
	}

	inline sf::Vector2f unitVectorScript(const sf::Vector2f& vector)
	{
		return Math::unitVector(vector);
	}

	inline sf::Vector2f unitVector(const sf::Vector2i& vector)
	{
		sf::Vector2f vectorF(vector);
		if (vector == sf::Vector2i(0, 0))
			return vectorF;

		return vectorF / vectorLength(vectorF);
	}


	inline sf::Vector2f getMidOfRect(const sf::FloatRect& rect)
	{
		return sf::Vector2f(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
	}

	inline sf::Vector2f roundVectorToOne(const sf::Vector2f& v)
	{
		sf::Vector2f newV = v;
		if (newV.x != 0.f)	newV.x = newV.x > 0.f ? 1.f : -1.f;
		if (newV.y != 0.f) newV.y = newV.y > 0.f ? 1.f : -1.f;
		return newV;
	}

	sf::Vector2f adjustVectorWithCombination(const sf::Vector2f& vecToAdjust, float divideVal = 1.4f);

	inline sf::Vector2f transformPointWithAngle(const sf::Vector2f& point, float angle)
	{
		sf::Transform transform;
		transform.rotate(angle);
		return transform.transformPoint(point);
	}

	inline sf::Vector2f down_perpendicular(const sf::Vector2f& vector)
	{
		return sf::Vector2f(vector.y, -vector.x);
	}

	inline sf::Vector2f up_perpendicular(const sf::Vector2f& vector)
	{
		return sf::Vector2f(-vector.y, vector.x);
	}
}
#endif