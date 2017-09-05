#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <unordered_map>

class AnimationComponent;


class Animation : public sf::Drawable, public sf::Transformable{
public:
	typedef std::vector<std::pair<sf::IntRect, sf::Time> > OrigList;
	enum Type{
		SingleSheet,
		SelectiveSheet,
		TypeCount
	};
public:
	Animation(Type type);
	Animation(Type type, const sf::Texture& texture);
	~Animation();

	void setAnimationComponent(AnimationComponent* anim_comp);
	void set_is_dead_anim(bool flag);

	

	void update(sf::Time dt);

	void setSelectiveList(const OrigList& list, bool flipX);

	void setForeignSprite(sf::Sprite* mPoint);

	void setFrameSize(sf::Vector2i newFrameSz);

	sf::Vector2i getFrameSize() const;

	void setNumFrames(std::size_t newNum);

	void setDuration(sf::Time newTime);
	
	void setTexture(const sf::Texture& texture);

	const sf::Texture* getTexture() const;

	void setRepeating(bool flag);

	bool isRepeating() const;

	void setFlipX(bool flag);

	void setIsEndToStart(bool flag);

	void restart();

	bool isFinished() const;

	sf::FloatRect getLocalBounds() const;

	sf::FloatRect getGlobalBounds() const;

	void setPause(bool flag);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;	//how big is the frame size in the spread sheet
	int mNumFrames;	//total number of frames to be animated
	int mCurrentFrame;	//tracking the current frame
	sf::Time mDuration;	//duration of the animation
	sf::Time mElapsedTime;	//tracking current time
	bool mRepeat;	//repeat or not
	bool mEndToStart; //if repeat and this var is true, run again from end to start, will affect mToStart
	bool mPause; //pause or not
	bool mFlipX; //flip the animation or not
	bool mToStart; //whether the current animation is from end to start

	Type mType;
	OrigList mOrigList; // exculsive to SelectiveSheet
	sf::Sprite* mForeignSprite; // exclusive to SelectiveSheet

	AnimationComponent* m_anim_comp;
	bool m_is_dead_anim;

};

