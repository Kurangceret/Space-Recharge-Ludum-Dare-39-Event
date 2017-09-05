#include "Animation.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../Helper/Utility.h"
#include "AnimationComponent.h"

Animation::Animation(Type type)
:
mType(type),
mSprite(),
mFrameSize(),
mNumFrames(0),
mCurrentFrame(0),
mDuration(sf::Time::Zero),
mElapsedTime(sf::Time::Zero),
mRepeat(false),
mForeignSprite(nullptr),
mOrigList(),
mPause(false),
mFlipX(false),
mEndToStart(false),
mToStart(false),
m_anim_comp(nullptr)
{
}


Animation::Animation(Type type, const sf::Texture& texture)
: 
mType(type),
mSprite(texture),
mFrameSize(),
mNumFrames(0),
mCurrentFrame(0),
mDuration(sf::Time::Zero),
mElapsedTime(sf::Time::Zero),
mRepeat(false),
mForeignSprite(nullptr),
mOrigList(),
mPause(false),
mFlipX(false),
mEndToStart(false),
mToStart(false),
m_anim_comp(nullptr)
{
}




Animation::~Animation()
{
}

void Animation::setAnimationComponent(AnimationComponent* anim_comp)
{
	m_anim_comp = anim_comp;
}

void Animation::set_is_dead_anim(bool flag)
{
	m_is_dead_anim = flag;
}


void Animation::update(sf::Time dt)
{
	if (mPause)
		return;
	//we will draw all the animation frame in the texture based on the 'duration' time we wanted
	
	//this one will animate the whole sheet
	if (mType == Animation::SingleSheet){
		sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
		mElapsedTime += dt;


		sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
		sf::IntRect textureRect = mSprite.getTextureRect();

		if (mCurrentFrame == 0)
			textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);

		while (mElapsedTime >= timePerFrame && (mCurrentFrame <=
			mNumFrames || mRepeat)){
			textureRect.left += textureRect.width; // move texture rect to the right
			//if we reached the bounds, move down to new row (based on X)
			if (textureRect.left + textureRect.width > textureBounds.x){
				textureRect.left = 0;
				textureRect.top += textureRect.height;
			}

			mElapsedTime -= timePerFrame;
			//if repeat, we try to re do the animation by setting it back again to 0
			if (mRepeat){
				mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;

				if (mCurrentFrame == 0)
					textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
			}
			else{
				mCurrentFrame++;
			}
		}
		mSprite.setTextureRect(textureRect);
	}
	//will animate selective sheet based on the data given by data tables
	//it was stored in mOrigList
	if (mType == SelectiveSheet && !mOrigList.empty() && mForeignSprite){

		if (mCurrentFrame >= mNumFrames)
			return;

		sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
		if (mOrigList[mCurrentFrame].second != sf::seconds(0.f))
			timePerFrame = mOrigList[mCurrentFrame].second;

		mElapsedTime += dt;

		while (mElapsedTime >= timePerFrame && (mCurrentFrame <
			mNumFrames || mRepeat))
		{

			mElapsedTime -= timePerFrame;

			sf::Vector2i additionalOrigin(0, 0);
			sf::Vector2i changerSize(1, 1);

			sf::Vector2i frameSize = Utility::intRectSizeFormat(mOrigList[mCurrentFrame].first);
			if (frameSize == sf::Vector2i())
				frameSize = mFrameSize;

			if (mFlipX){
				additionalOrigin.x = frameSize.x;
				changerSize.x = -1;
			}

			mForeignSprite->setTextureRect(sf::IntRect(
				sf::Vector2i(Utility::intRectTopLeftFormat(mOrigList[mCurrentFrame].first) 
				+ additionalOrigin), 
				sf::Vector2i(frameSize.x * changerSize.x, frameSize.y * changerSize.y))
				);
			
			if (mRepeat && !mEndToStart && mCurrentFrame >= mNumFrames - 1)
				mCurrentFrame = -1;
			else if (mRepeat && mEndToStart && mCurrentFrame >= mNumFrames - 1)
				mToStart = true;
			else if (mRepeat && mEndToStart && mCurrentFrame <= 0)
				mToStart = false;
			
			if (mToStart)
				mCurrentFrame--;
			else
				mCurrentFrame++;
		}

		if (m_anim_comp && m_is_dead_anim && mCurrentFrame >= mNumFrames)
			m_anim_comp->set_has_dead_anim_played(true);
	}
	
}

void Animation::setSelectiveList(const OrigList& list, bool flipX)
{
	if (mType == SelectiveSheet){
		mOrigList.clear();
		mOrigList = list;
		if (mForeignSprite){
			sf::Vector2i additionalOrigin(0, 0);
			sf::Vector2i changerSize(1, 1);

			sf::Vector2i frameSize = Utility::intRectSizeFormat(mOrigList[0].first);
			if (frameSize == sf::Vector2i())
				frameSize = mFrameSize;

			if (flipX){
				additionalOrigin.x = frameSize.x;
				changerSize.x = -1;
			}
			mForeignSprite->setTextureRect(sf::IntRect(Utility::intRectTopLeftFormat(mOrigList[0].first) 
				+ additionalOrigin, 
				sf::Vector2i(frameSize.x * changerSize.x,
				mFrameSize.y * changerSize.y)));

			//Utility::centerOrigin(*mForeignSprite);
			if (list.size() > 1)
				mCurrentFrame = 1;
		}
	}
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}


void Animation::setForeignSprite(sf::Sprite* mPoint){ mForeignSprite = mPoint; }

void Animation::setFrameSize(sf::Vector2i newFrameSz){ mFrameSize = newFrameSz; }

sf::Vector2i Animation::getFrameSize() const { return mFrameSize; }

void Animation::setNumFrames(std::size_t newNum){ mNumFrames = newNum; }

void Animation::setDuration(sf::Time newTime){ mDuration = newTime; }

void Animation::setTexture(const sf::Texture& texture)	{ mSprite.setTexture(texture); }

const sf::Texture* Animation::getTexture() const{ return mSprite.getTexture(); }

void Animation::setRepeating(bool flag){ mRepeat = flag; }

bool Animation::isRepeating() const	{ return mRepeat; }

void Animation::setFlipX(bool flag){ mFlipX = flag; }

void Animation::setIsEndToStart(bool flag){ mEndToStart = flag; if (!flag) mToStart = false; }

void Animation::restart(){ mCurrentFrame = 0; mElapsedTime = sf::Time::Zero; }

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::setPause(bool flag){ mPause = flag; }