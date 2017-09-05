#ifndef UTILITY_H
#define UTILITY_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <random>
#include <cmath>
#include <ctime>
#include <cassert>
#include <iostream>
#include <set>
#include <cstdint>
#include <string>
#include <Box2D\Common\b2Draw.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Collision\Shapes\b2Shape.h>

class RealTimeInputComponent;
struct lua_State;
struct RotatedRect;
namespace
{
	inline std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

namespace Utility{
	struct CircleGeometry{
		CircleGeometry(const sf::Vector2f& centerPos, float radius);
		sf::Vector2f mCenterPos;
		float mRadius;
	};

	template<class Object>
	inline void centerOrigin(Object& sfObj)
	{
		sf::FloatRect bounds = sfObj.getLocalBounds();
		sfObj.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}

	template <class Pointer>
	inline std::uintptr_t convertPointerToAddress(Pointer* pointer)
	{
		std::uintptr_t address = reinterpret_cast<uintptr_t>(pointer);
		return address;
	}

	template<class Object>
	inline void centerOrigin(Object& sfObj, const sf::FloatRect& rect)
	{
		sfObj.setOrigin(rect.width / 2.f, rect.height / 2.f);
	}

	

	inline int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	//exclusiveMax will be minused 1 in the func
	inline int randomRange(int exclusiveMin, int exclusiveMax)
	{
		if (exclusiveMin == exclusiveMax) return exclusiveMin;
		if (exclusiveMin > exclusiveMax) std::swap(exclusiveMin, exclusiveMax);

		std::uniform_int_distribution<> distr(exclusiveMin, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	

	inline void printVector(const sf::Vector2f& vector, bool endLine)
	{
		std::cout << " " << vector.x << " " << vector.y << " ";
		if (endLine)
			printf("\n");
	}

	inline void printVector(const sf::Vector2i& vector, bool endLine)
	{
		std::cout << " " << vector.x << " " << vector.y << " ";
		if (endLine)
			printf("\n");
	}

	
	inline sf::Vector2i intRectTopLeftFormat(const sf::IntRect& intRect)
	{
		return sf::Vector2i(intRect.left, intRect.top);
	}

	inline sf::Vector2i intRectSizeFormat(const sf::IntRect& intRect)
	{
		return sf::Vector2i(intRect.width, intRect.height);
	}

	inline void printStringToCmd(const std::string& message, bool endLine = true)
	{
		std::cout << message;
		if (endLine) std::cout << std::endl;
	}

	bool offSetRect1AndCheckRect2(const sf::FloatRect& rect1, 
		const sf::Vector2f& offSet, const sf::FloatRect& rect2);

	/*Exclusvie Script Usage*/
	bool offSetRect1AndCheckRect2Script(const sf::FloatRect& rect1,
		const sf::Vector2f& offSetDir, float valOffSet, const sf::FloatRect& rect2);

	void drawRotatedRect(sf::RenderTarget& target, const RotatedRect* rotatedRect,
		const sf::Color& color = sf::Color());

	bool rotatedCollision(const RotatedRect& lhs, const RotatedRect& rhs);

	void drawFloatToInt(sf::RenderTarget& target, const sf::Vector2f& pos, const sf::Drawable& drawable);
	void drawFloatToInt(sf::RenderTarget& target, const sf::Transform& transform, const sf::Drawable& drawable);
	
	bool isPointInsideImaginaryCircle(const sf::Vector2f& circleCenterPos, float circleRadius,
		const sf::Vector2f& pointPos);

	bool isCircleIntersectsWithFloatRect(const CircleGeometry& circle, const sf::FloatRect& floatRect);

	//if reading the luaScirpt file was met with error, program will terminate together
	//with printing the error on cmd
	void readLuaScriptFile(const std::string& scriptDir, lua_State* luaState);

	float formatPixelToMeter(float argPixel);
	float formatMeterToPixel(float argMeter);

	b2Vec2 formatVecPixelToMeter(const sf::Vector2f& pixelVArg);
	sf::Vector2f formatVecMeterToPixel(const b2Vec2& meterVArg);

	sf::Transform formatb2TransformToSfTransform(const b2Transform& boxTransform);

	std::string transformStrToLowerCase(const std::string& argStr);
	sf::Color box2dColorToSfColor(const b2Color& colorToChange);
	b2Color sfColorToBox2dColor(const sf::Color& colorToChange);

	sf::FloatRect b2AABBToSfFloatRect(const b2AABB& b2aabb);

	bool isAnyMovementKeyDoubleTapped(RealTimeInputComponent* realTimeInputComponent);

	sf::Vector2f separateStrToVector2f(const std::string& str, const char* delimiters);
}

#endif