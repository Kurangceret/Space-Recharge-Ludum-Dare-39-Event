#include "MenuState.h"
#include "../Helper/Utility.h"
#include "../Helper/Constant.h"
#include "LoadingState.h"
#include <vector>

MenuState::MenuState(StateStack& stack, Context context)
:State(stack, context), mOptionIndex(0)
{
	mStateID = States::Menu;
	sf::Text playText;
	
	playText.setString("Play");
	playText.setFont(*context.mFont);
	playText.setColor(sf::Color::Black);
	Utility::centerOrigin(playText);
	playText.setPosition(context.mWindow->getView().getSize() / 2.f);
	mTexts.push_back(playText);

	sf::Text exitText;
	exitText.setString("Exit");
	exitText.setFont(*context.mFont);
	exitText.setColor(sf::Color::Black);
	Utility::centerOrigin(exitText);
	exitText.setPosition(playText.getPosition().x, playText.getPosition().y + context.mWindow->getView().getSize().y * 0.05);
	mTexts.push_back(exitText);


	
	m_logo_texture.loadFromFile(Constant::textureDir + "logo.png");

	m_logo_sprite.setTexture(m_logo_texture);
	Utility::centerOrigin(m_logo_sprite);
	m_logo_sprite.setPosition(context.mWindow->getView().getSize() / 2.f);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;

	window.setView(window.getDefaultView());

	window.draw(m_logo_sprite);

	for (sf::Text& text : mTexts)
		window.draw(text);
		
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Return){
		if (mOptionIndex == Play){
			requestStackPop();

			std::unique_ptr<LoadingState> loadingState(new LoadingState(*getStateStack(), getContext()));
			requestStackPush(std::move(loadingState));
		}
		else if (mOptionIndex == Exit){
			// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up){
		// Decrement and wrap-around
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mTexts.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down){
		// Increment and wrap-around
		if (mOptionIndex < mTexts.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		updateOptionText();
	}

	return true;
}
void MenuState::updateOptionText()
{
	if (mTexts.empty())
		return;

	// White all texts
	for (sf::Text& text : mTexts)
		text.setColor(sf::Color::White);

	// Red the selected text
	mTexts[mOptionIndex].setColor(sf::Color::Red);
}
MenuState::~MenuState()
{
}
