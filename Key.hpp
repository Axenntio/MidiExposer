#pragma once
#include <SFML/Graphics.hpp>

class Key {
public:
	Key() = default;
	Key(unsigned short l_id, sf::IntRect l_rect);

	sf::IntRect GetRect() const;
	void Draw(sf::RenderWindow &window, bool black_key) const;
private:
	unsigned short m_id;
	sf::IntRect m_rect;
};
