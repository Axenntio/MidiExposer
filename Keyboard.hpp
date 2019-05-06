#pragma once
#include <SFML/Graphics.hpp>
#include "Key.hpp"

class Keyboard {
public:
	Keyboard(unsigned short l_size, unsigned char l_pitch);

	Key GetKey(unsigned short l_id) const;
	unsigned short GetSize() const;
	unsigned short GetWindowWidth() const;
	void Draw(sf::RenderWindow &window) const;
private:
	unsigned short m_window_width;
	unsigned short m_size;
	std::vector<Key> m_keys;
};
