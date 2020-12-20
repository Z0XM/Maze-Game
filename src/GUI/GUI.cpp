#include "../GUI.hpp"
#include <iostream>
namespace gui {

	template <typename T>
	void renderGroup(std::map<std::string, T*>& map, sf::RenderTarget& renderTarget)
	{
		for (auto it = map.begin(); it != map.end(); it++) {
			it->second->render(renderTarget);
		}
	}

	void render(sf::RenderTarget& renderTarget)
	{
		renderGroup(Textbox::Group, renderTarget);
		renderGroup(Button::Group, renderTarget);
	}

	template <typename T>
	void deleteGroup(std::map<std::string, T*>& map)
	{
		for (auto it = map.begin(); it != map.end(); it++) {
			delete it->second;
		}
		map.clear();
	}

	void deleteAllGuiGroups()
	{
		deleteGroup(Textbox::Group);
		deleteGroup(Button::Group);
		Clickable::mouseHoveringOn = nullptr;
	}

	template<typename T>
	Clickable* searchForPointedButtonIn(std::map<std::string, T*>& map, sf::Vector2f& mousePos)
	{
		for (auto it = map.begin(); it != map.end(); it++){
			Clickable* clickable = it->second->isHit(mousePos);
			if (clickable != nullptr)return clickable;
		}
		return nullptr;
	}

	void updateMousePointer(sf::Vector2f&& mousePos)
	{
		if (Clickable::mouseHoveringOn == nullptr || Clickable::mouseHoveringOn != Clickable::mouseHoveringOn->isHit(mousePos)) {
			
			Clickable* currentMouseHoveringOn = nullptr;

			if (Clickable::mouseHoveringOn != nullptr && Clickable::mouseHoveringOn->contains(mousePos)) {
				currentMouseHoveringOn = Clickable::mouseHoveringOn->isHit(mousePos);
			}
			else {
				currentMouseHoveringOn = searchForPointedButtonIn(Button::Group, mousePos);
			}

			if (Clickable::mouseHoveringOn != currentMouseHoveringOn)
			{
				if (Clickable::mouseHoveringOn != nullptr)Clickable::mouseHoveringOn->deactivateHighlight();
				Clickable::mouseHoveringOn = currentMouseHoveringOn;
				if (Clickable::mouseHoveringOn != nullptr)Clickable::mouseHoveringOn->activateHighlight();
			}
		}
	}

	void registerClick(sf::Mouse::Button button)
	{
		Clickable::clicked = Clickable::mouseHoveringOn;
		Clickable::pressedMouseButton = button;
	}

	void unregisterClick(sf::Mouse::Button button, sf::Vector2f mousePos)
	{
		if (Clickable::clicked != nullptr && button == Clickable::pressedMouseButton)
		{
			Clickable::clicked->action();
		}
		Clickable::clicked = nullptr;
	}
}