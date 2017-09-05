#include "GameOverState.h"
#include "MenuState.h"
#include "../Helper/Utility.h"
#include <SFML/Graphics/RectangleShape.hpp>

GameOverState::GameOverState(StateStack& stack, Context context, const std::string& message)
:State(stack, context)
{
	mStateID = States::GameOver;
	sf::Font& font = *context.mFont;
	sf::Vector2f viewSize = context.mWindow->getDefaultView().getSize();

	
	mText.setFont(font);
	mText.setString("Game Over");
	mText.setCharacterSize(70);

	mDescText.setFont(font);
	mDescText.setCharacterSize(30.0);
	mDescText.setString(message);
	Utility::centerOrigin(mText);
	Utility::centerOrigin(mDescText);
	mText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);
	mDescText.setPosition(0.5f * viewSize.x, 0.3f * viewSize.y);


	
}


GameOverState::~GameOverState()
{
}



void GameOverState::draw()
{
	sf::RenderWindow& window = *(getContext().mWindow);

	sf::View view = window.getView();
	//window size and position, the top left here indicates the current top left drawable window position
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f topLeftPos(viewPort.left, viewPort.top);

	sf::RenderStates states;
	states.transform.translate(topLeftPos);
	//window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(sf::Vector2f(window.getSize()));

	window.draw(backgroundShape, states);
	window.draw(mText, states);

	topLeftPos.x += viewPort.width * 0.5f;
	topLeftPos.y += viewPort.height * 0.7f;

	mDescText.setPosition(topLeftPos);
	window.draw(mDescText);
	
}
bool GameOverState::update(sf::Time dt)
{
	return false;
}
bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape){
		// Escape pressed, remove itself to return to the game
		requestStackClear();
		MenuState::Ptr menuState(new MenuState(*getStateStack(), getContext()));
		requestStackPush(std::move(menuState));
	}

	if (event.key.code == sf::Keyboard::BackSpace){
		requestStackClear();
		MenuState::Ptr menuState(new MenuState(*getStateStack(), getContext()));
		requestStackPush(std::move(menuState));
	}

	return false;
}