#include "Keyboard.hpp"
#include "defines.hpp"

Keyboard::Keyboard(unsigned short l_size, unsigned char l_pitch) : m_size(l_size)
{
	unsigned short index = 0;
	unsigned char left;
	unsigned char height;
	unsigned char width;

	for (unsigned char i = 0; i < this->m_size; i++) {
		height = NOTE_HEIGHT;
		width = NOTES_WIDTH;
		left = 0;
		if (i % 12 == (1 + l_pitch) || i % 12 == (4 + l_pitch) || i % 12 == (6 + l_pitch) || i % 12 == (9 + l_pitch) || i % 12 == (11 + l_pitch)) {
			height -= NOTE_HEIGHT / 3;
			width -= NOTES_WIDTH / 2;
			left = NOTES_WIDTH / 4;
		}
		this->m_keys.push_back(Key(i, sf::IntRect(index * (NOTES_WIDTH + NOTES_SEP) - left, HEIGHT - NOTE_HEIGHT, width, height)));
		if (!(i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11))
			index++;
	}
	this->m_window_width = index * (NOTES_WIDTH + NOTES_SEP);
}

Key Keyboard::GetKey(unsigned short l_id) const
{
	return this->m_keys[l_id];
}

unsigned short Keyboard::GetSize() const
{
	return this->m_size;
}

unsigned short Keyboard::GetWindowWidth() const
{
	return this->m_window_width;
}

void Keyboard::Draw(sf::RenderWindow &window) const
{
	for (unsigned char i = 0; i < this->m_size; i++) {
		this->m_keys[i].Draw(window, false);
	}
	for (unsigned char i = 0; i < this->m_size; i++) {
		this->m_keys[i].Draw(window, true);
	}
}
