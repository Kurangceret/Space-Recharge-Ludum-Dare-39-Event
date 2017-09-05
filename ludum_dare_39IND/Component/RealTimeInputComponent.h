#pragma once
#include "Component.h"
#include <SFML/Window/Keyboard.hpp>
#include <vector>

class RealTimeInputSystem;
class RealTimeInputComponent :	public Component{
public:
	friend class RealTimeInputSystem;
public:
	RealTimeInputComponent(Entity* entity);
	virtual ~RealTimeInputComponent();
	
	bool isJumpKeyPressed() const;
	bool isDashKeyPressed() const;
	bool isAttackKeyPressed() const;

	bool isUpKeyPressed() const;
	bool isDownKeyPressed() const;
	bool isRightKeyPressed() const;
	bool isLeftKeyPressed() const;

	void emptyDashKeyPressed();
	void emptyJumpKeyPressed();
	void emptyAttackKeyPressed();

	bool isUpKeyCountEmpty() const;
	bool isDownKeyCountEmpty() const;
	bool isLeftKeyCountEmpty() const;
	bool isRightKeyCountEmpty() const;

	void emptyAllEventKey(std::size_t* exceptKey);

	sf::Keyboard::Key getCurDoubleTappedKey() const;
private:
	std::size_t mUpKeyCount;
	std::size_t mDownKeyCount;
	std::size_t mLeftKeyCount;
	std::size_t mRightKeyCount;

	std::size_t mJumpKeyPressedCount;
	std::size_t mDashKeyPressedCount;
	std::size_t mAttackKeyPressedCount;

	std::size_t mUpKeyPressedCount;
	std::size_t mDownKeyPressedCount;
	std::size_t mLeftKeyPressedCount;
	std::size_t mRightKeyPressedCount;

	sf::Keyboard::Key mCurDoubleTappedKey;

	std::vector<std::size_t*> mEventKeyList;
};

