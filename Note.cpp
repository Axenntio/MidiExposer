#include "Note.hpp"
#include "defines.hpp"

Note::Note(unsigned char l_id, unsigned char l_strength, sf::Color l_color) : m_id(l_id), m_strength(l_strength), m_color(l_color), m_size(1), m_moved(0)
{
}

unsigned char Note::GetId() const
{
	return this->m_id;
}

unsigned char Note::GetStrength() const
{
	return this->m_strength;
}

void Note::SizeUp()
{
	this->m_size += SPEED;
}

unsigned short Note::GetSize() const
{
	return this->m_size;
}

void Note::MoveUp()
{
	this->m_moved += SPEED;
}

unsigned short Note::GetMove() const
{
	return this->m_moved;
}

sf::Color Note::GetColor() const
{
	return this->m_color;
}
