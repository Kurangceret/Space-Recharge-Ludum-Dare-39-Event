#include "Utility.h"
#include "Math.h"
#include <SFML/Graphics/VertexArray.hpp>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include "Constant.h"
#include <algorithm>
#include <cctype>
#include "../Component/RealTimeInputComponent.h"

void Utility::drawFloatToInt(sf::RenderTarget& target,	const sf::Vector2f& pos, 
	const sf::Drawable& drawable)
{
	/*sf::RenderStates states;
	states.transform.translate(pos);
	
	sf::Vector2i v;
	v.x = static_cast<int> (pos.x);
	v.y = static_cast<int> (pos.y);

	sf::Vector2f newV;
	newV.x = static_cast<float>(v.x) - pos.x;
	newV.y = static_cast<float>(v.y) - pos.y;
	states.transform.translate(newV);

	target.draw(drawable, states);*/
	sf::Transform transform;
	transform.translate(pos);
	drawFloatToInt(target, transform, drawable);
}

void Utility::drawFloatToInt(sf::RenderTarget& target, const sf::Transform& transform, const sf::Drawable& drawable)
{
	//Utility::drawFloatToInt(target, transform.transformPoint(sf::Vector2f()), drawable);
	sf::RenderStates states;
	states.transform = transform;

	sf::Vector2f pos = transform.transformPoint(sf::Vector2f());
	sf::Vector2i v;
	v.x = static_cast<int> (pos.x);
	v.y = static_cast<int> (pos.y);

	sf::Vector2f newV;
	newV.x = static_cast<float>(v.x) - pos.x;
	newV.y = static_cast<float>(v.y) - pos.y;
	states.transform.translate(newV);

	target.draw(drawable, states);
}

bool Utility::isPointInsideImaginaryCircle(const sf::Vector2f& circlePos, float circleRadius,
	const sf::Vector2f& pointPos)
{
	if (pointPos.x >= circlePos.x - circleRadius
		&& pointPos.x <= circlePos.x + circleRadius
		&&	pointPos.y >= circlePos.y - circleRadius
		&& pointPos.y <= circlePos.y + circleRadius)
	{
		float dx = circlePos.x - pointPos.x;
		float dy = circlePos.y - pointPos.y;

		dx *= dx;
		dy *= dy;

		float distanceSquared = dx + dy;
		float radiusSquared = circleRadius * circleRadius;

		return distanceSquared <= radiusSquared;
	}

	return false;
}
Utility::CircleGeometry::CircleGeometry(const sf::Vector2f& centerPos, float radius)
{
	mCenterPos = centerPos;
	mRadius = radius;
}

bool Utility::isCircleIntersectsWithFloatRect(const Utility::CircleGeometry& circle, 
	const sf::FloatRect& floatRect)
{
	sf::Vector2f midOfRect = Math::getMidOfRect(floatRect);
	sf::Vector2f circleDistance(std::abs(circle.mCenterPos.x - midOfRect.x) ,
		std::abs(circle.mCenterPos.y - midOfRect.y));

	if (circleDistance.x > (floatRect.width / 2.f + circle.mRadius)) return false;
	if (circleDistance.y > (floatRect.height / 2.f + circle.mRadius)) return false;

	if (circleDistance.x <= (floatRect.width / 2.f)) return true;
	if (circleDistance.y <= (floatRect.height / 2.f)) return true;

	float cornerDistance = std::pow((circleDistance.x - floatRect.width / 2.f), 2.f) +
		std::pow((circleDistance.y - floatRect.height / 2.f), 2.f);

	return (cornerDistance <= std::pow(circle.mRadius, 2));
}




bool Utility::offSetRect1AndCheckRect2(const sf::FloatRect& rect1,
	const sf::Vector2f& offSet, const sf::FloatRect& rect2)
{
	sf::FloatRect newOffSetRect = rect1;
	newOffSetRect.top += offSet.y;
	newOffSetRect.left += offSet.x;

	return newOffSetRect.intersects(rect2);
}


bool Utility::offSetRect1AndCheckRect2Script(const sf::FloatRect& rect1,
	const sf::Vector2f& offSetDir,
	float valOffSet, const sf::FloatRect& rect2)
{
	return offSetRect1AndCheckRect2(rect1, offSetDir * valOffSet, rect2);
}

void Utility::readLuaScriptFile(const std::string& scriptDir, lua_State* luaState)
{
	if (luaL_dofile(luaState, scriptDir.c_str()) != 0)
	{
		std::cout << lua_tostring(luaState, -1) << std::endl;
		assert(false == true);
	}
}


float Utility::formatPixelToMeter(float argPixel)
{
	return argPixel / Constant::pixelToMeterDiff;
}

float Utility::formatMeterToPixel(float argMeter)
{
	return argMeter * Constant::pixelToMeterDiff;
}

b2Vec2 Utility::formatVecPixelToMeter(const sf::Vector2f& pixelVArg)
{
	return b2Vec2(Utility::formatPixelToMeter(pixelVArg.x), Utility::formatPixelToMeter(pixelVArg.y));
}

sf::Vector2f Utility::formatVecMeterToPixel(const b2Vec2& meterVArg)
{
	return sf::Vector2f(Utility::formatMeterToPixel(meterVArg.x), 
		Utility::formatMeterToPixel(meterVArg.y));
}

std::string Utility::transformStrToLowerCase(const std::string& argStr)
{
	std::string cStr = argStr;
	std::transform(cStr.begin(), cStr.end(), cStr.begin(), std::tolower);
	return cStr;
}

sf::Color Utility::box2dColorToSfColor(const b2Color& colorToChange)
{
	return sf::Color(colorToChange.r * 255, colorToChange.g * 255, 
		colorToChange.b * 255, colorToChange.a * 255);
}

b2Color Utility::sfColorToBox2dColor(const sf::Color& colorToChange)
{
	return b2Color(((float)colorToChange.r) / 255.f, ((float)colorToChange.g) / 255.f,
		((float)colorToChange.b) / 255.f, ((float)colorToChange.a) / 255.f);
}

sf::Transform Utility::formatb2TransformToSfTransform(const b2Transform& boxTransform)
{
	sf::Transform transform;
	transform.translate(Utility::formatVecMeterToPixel(boxTransform.p));
	transform.rotate(Math::toDegree(boxTransform.q.GetAngle()));
	return transform;
}



sf::FloatRect Utility::b2AABBToSfFloatRect(const b2AABB& b2aabb)
{
	sf::Vector2f bottomRight(Utility::formatVecMeterToPixel(b2aabb.upperBound));
	sf::Vector2f topLeft(Utility::formatVecMeterToPixel(b2aabb.lowerBound));
		
	sf::FloatRect finalRect(topLeft, bottomRight - topLeft);
	finalRect.top = std::ceil(finalRect.top);
	finalRect.left = std::ceil(finalRect.left);
	finalRect.width = std::floor(finalRect.width);
	finalRect.height = std::floor(finalRect.height);

	return finalRect;
}


bool Utility::isAnyMovementKeyDoubleTapped(RealTimeInputComponent* realTimeInputComp)
{
	sf::Keyboard::Key key = realTimeInputComp->getCurDoubleTappedKey();
	return key == sf::Keyboard::Up || key == sf::Keyboard::Left || key == sf::Keyboard::Right ||
		sf::Keyboard::Down;
}


sf::Vector2f Utility::separateStrToVector2f(const std::string& str, const char* delimiters)
{
	char newStr[8];
	char* ch;

	std::strcpy(newStr, str.c_str());
	sf::Vector2f finalDir;
	int i = 0;

	ch = strtok(newStr, delimiters);
	while (ch != NULL){
		if (i == 0) finalDir.x = std::atoi(ch);
		else if (i == 1) finalDir.y = std::atoi(ch);
		ch = strtok(NULL, delimiters);
		i++;
	}

	return finalDir;
}