#pragma once

#include "GUI/Clickable.hpp"
#include "GUI/Textbox.hpp"
#include "GUI/Button.hpp"

namespace gui {
	void render(sf::RenderTarget& renderTarget);

	void updateMousePointer(sf::Vector2f&& mousPos);

	void registerClick(sf::Mouse::Button button);

	void unregisterClick(sf::Mouse::Button button, sf::Vector2f mousePos);

	void deleteAllGuiGroups();
}