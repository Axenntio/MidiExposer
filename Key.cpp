#include "Key.hpp"
#include "defines.hpp"

Key::Key(unsigned short l_id, sf::IntRect l_rect) : m_id(l_id), m_rect(l_rect)
{
}

sf::IntRect Key::GetRect() const
{
	return this->m_rect;
}

void Key::Draw(sf::RenderWindow &window, bool black_key) const
{
	sf::RectangleShape rectangle;

	rectangle.setSize(sf::Vector2f(this->m_rect.width, this->m_rect.height));
	rectangle.setPosition(this->m_rect.left, this->m_rect.top);
	if (this->m_rect.height != NOTE_HEIGHT) {
		rectangle.setFillColor(sf::Color(0, 0, 0, 255));
		if (black_key)
			window.draw(rectangle);
	}
	else {
		rectangle.setFillColor(sf::Color(255, 255, 255, 255));
		if (!black_key)
			window.draw(rectangle);
	}
}
