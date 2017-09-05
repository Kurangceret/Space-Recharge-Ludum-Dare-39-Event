#include "PauseState.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "../Helper/Utility.h"
#include "MenuState.h"

PauseState::PauseState(StateStack& stack, Context context)
:State(stack, context),
pausedText(),
backgroundSprite(),
instructionText()
{
	mStateID = States::Pause;
	sf::Font& font = *context.mFont;
	sf::Vector2f viewSize = context.mWindow->getDefaultView().getSize();

	pausedText.setFont(font);
	pausedText.setString("Game Paused");
	pausedText.setCharacterSize(70);
	Utility::centerOrigin(pausedText);
	pausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	instructionText.setFont(font);
	instructionText.setString("(Press Backspace to return to the main menu)");
	Utility::centerOrigin(instructionText);
	instructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
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
	window.draw(pausedText, states);
	window.draw(instructionText, states);
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape){
		// Escape pressed, remove itself to return to the game
		requestStackPop();
	}

	if (event.key.code == sf::Keyboard::BackSpace){
		requestStackClear();
		MenuState::Ptr menuState(new MenuState(*getStateStack(), getContext()));
		requestStackPush(std::move(menuState));
		//getContext().generalData->clearAllData();
	}
	return false;
}

bool PauseState::update(sf::Time)
{
	return false;
}
PauseState::~PauseState()
{
}