#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>

class BaseGuiComponent{
public:
	typedef std::shared_ptr<BaseGuiComponent> Ptr;
public:
	BaseGuiComponent();
	virtual ~BaseGuiComponent();

	void handleMouseEvent(const sf::Event& event, 
		const sf::Vector2f& mousePosRelativeToWindow);
		
	virtual void update(sf::RenderWindow& renderWindow, sf::Time dt);

	//which means the pos is relative to window size
	void setPositionOnWindow(const sf::Vector2f& posRelativeToWindow);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states);


	void setIsRemoved(bool flag);
	void setIsHidden(bool flag);

	bool isRemoved() const;
	bool isHidden() const;

	sf::Vector2f getPosRelativeToWindow() const;
protected:
	void pushDrawableObj(const sf::Drawable& drawable);
private:
	virtual void handleOnClick(sf::Mouse::Button button, 
		const sf::Vector2f& mousePosRelativeToWindow) = 0;

	virtual void handleMouseHover(const sf::Vector2f& mousePosRelativeToWindow);

	virtual void finalizeDrawableGuiList() = 0;
private:
	std::vector<const sf::Drawable*> mDrawableGuiList;
	//sf::Sprite mGuiSprite;
	sf::Vector2f mPosRelativeToWindow;

	//if set to true, gui manager will delete this obj
	bool mIsRemoved;
	//if set to true, gui manager will not draw this obj
	//and also no handleEvent to be triggered
	//obj will still remain alive
	bool mIsHidden;
};

