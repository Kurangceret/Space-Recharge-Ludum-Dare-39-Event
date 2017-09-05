#include "Camera.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"

Camera::Camera(sf::RenderTarget& window, float width, float height, float speed,
	sf::Vector2f mapSize)
:mWindow(window),
mSize(width, height),
mSpeed(speed),
//mView(sf::Vector2f(width / 2, height / 2), mSize),
mView(window.getDefaultView()),
mTarget(0.f, 0.f),
mPosition(mView.getCenter()),
mMapSize(mapSize)
{
	if (mSpeed < 0.f)
		mSpeed = 0.f;
	else if (mSpeed > 1.f)
		mSpeed = 1.f;

	mView.setCenter(sf::Vector2f(width / 2, height / 2));
	mView.zoom(1.f);
	mPosition = mView.getCenter();
	mTarget = mPosition;
	mSpeed = 5.f; //8.f
}


Camera::~Camera()
{
}

void Camera::setCameraTo(float x, float y)
{
	mPosition.x = x;
	mPosition.y = y;
	mTarget.x = x;
	mTarget.y = y;
}

void Camera::moveCenter(float x, float y)
{
	x = x - (mSize.x / 2);
	y = y - (mSize.y / 2);

	mPosition.x = x;
	mPosition.y = y;
	mTarget.x = x;
	mTarget.y = y;
}

void Camera::moveCamera(const sf::Vector2f& velocity, float cameraSpeed, sf::Time dt)
{
	//mTarget = mPosition;
	mPosition += velocity * cameraSpeed * dt.asSeconds();
}

void Camera::moveCamera(float vX, float vY, float cameraSpeed, sf::Time dt)
{
	//mTarget = mPosition;
	mPosition += sf::Vector2f(vX, vY) * cameraSpeed * dt.asSeconds();
}

void Camera::goTo(float x, float y)
{
	mTarget.x = static_cast<int>(x);
	mTarget.y = static_cast<int>(y);

	//std::cout << mTarget.x << std::endl;
}

void Camera::goToCenter(float x, float y)
{
	x = x - (mSize.x / 2);
	y = y - (mSize.y / 2);

	mTarget.x = x;
	mTarget.y = y;
}

void Camera::update(sf::Time dt)
{
	mPosition.x = std::ceil(mPosition.x);
	mPosition.y = std::ceil(mPosition.y);

	mView.setCenter(mPosition);
	if ((mPosition.x - mSize.x / 2) <= 0.f)
		mPosition.x = mSize.x / 2;
	else if ((mPosition.x + mSize.x / 2) >= mMapSize.x)
		mPosition.x = mMapSize.x - mSize.x / 2.f;

	if ((mPosition.y - mSize.y / 2.f) <= 0.f)
		mPosition.y = mSize.y / 2.f;
	else if ((mPosition.y + mSize.y / 2.f) >= mMapSize.y)
		mPosition.y = mMapSize.y - mSize.y / 2.f;

	//return;

	float x = mTarget.x - mPosition.x;
	float y = mTarget.y - mPosition.y;
	//if distance is less than 1 pixel or sort of
	if ((x * x + y * y) <= 1.f){
		mPosition = mTarget;
		mPosition.x = std::ceil(mPosition.x);
		mPosition.y = std::ceil(mPosition.y);
		mView.setCenter(mPosition);
		//mWindow.setView(mView);
		return;
	}

	float vx, vy, v;

	float dist = Math::vectorLength(mTarget - mPosition);

	v = (dist * mSpeed) / 60.f;

	if (v < 1.0f)
		v = 1.0f;

	vx = x * (v / dist);
	vy = y * (v / dist);

	mPosition.x += vx;
	mPosition.y += vy;


	if ((mPosition.x - mSize.x / 2) <= 0.f)
		mPosition.x = mSize.x / 2;
	else if ((mPosition.x + mSize.x / 2) >= mMapSize.x)
		mPosition.x = mMapSize.x - mSize.x / 2.f;

	if ((mPosition.y - mSize.y / 2.f) <= 0.f)
		mPosition.y = mSize.y / 2.f;
	else if ((mPosition.y + mSize.y / 2.f) >= mMapSize.y)
		mPosition.y = mMapSize.y - mSize.y / 2.f;

	mPosition.x = std::floor(mPosition.x);
	mPosition.y = std::floor(mPosition.y);
	mView.setCenter(mPosition);
	//mView.setViewport(sf::FloatRect(mPosition.x, mPosition.y, mSize.x, mSize.y));
		//mWindow.setView(mView);
	
}

sf::FloatRect Camera::getBoundingRect() const
{
	sf::FloatRect cameraBound;
	cameraBound.width = mView.getSize().x;
	cameraBound.height = mView.getSize().y;
	cameraBound.left = mView.getCenter().x - cameraBound.width / 2;
	cameraBound.top = mView.getCenter().y - cameraBound.height / 2;

	return cameraBound;
}

const sf::View& Camera::getView() const
{
	return mView;
}