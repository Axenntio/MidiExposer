#pragma once
#include <SFML/Graphics.hpp>

class Note {
public:
	Note(unsigned char l_id, unsigned char l_strength, sf::Color l_color);

	unsigned char GetId() const;
	unsigned char GetStrength() const;
	void SizeUp();
	unsigned short GetSize() const;
	void MoveUp();
	unsigned short GetMove() const;
	sf::Color GetColor() const;
private:
	unsigned char m_id;
	unsigned char m_strength;
	sf::Color m_color;
	unsigned short m_size;
	unsigned short m_moved;
};
