#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

#define HEIGHT 800

#define SPEED 5

#define SHOW_KEYBOARD true
#define KEYBOARD_KEY 57

#define START_NOTE 21
#define NOTES_NB 88

#define NOTE_DISAPPEAR 30

#define NOTES_WIDTH 32
#define NOTE_HEIGHT (10 + 90 * SHOW_KEYBOARD)
#define NOTES_SEP 1

bool key[NOTES_NB];

sf::Keyboard::Key keyboard_key[KEYBOARD_KEY] = {
	sf::Keyboard::Hyphen,
	sf::Keyboard::Num1,
	sf::Keyboard::Num2,
	sf::Keyboard::Num3,
	sf::Keyboard::Num4,
	sf::Keyboard::Num5,
	sf::Keyboard::Num6,
	sf::Keyboard::Num7,
	sf::Keyboard::Num8,
	sf::Keyboard::Num9,
	sf::Keyboard::Num0,
	sf::Keyboard::Equal,
	sf::Keyboard::Backspace,
	sf::Keyboard::Tab,
	sf::Keyboard::Q,
	sf::Keyboard::W,
	sf::Keyboard::E,
	sf::Keyboard::R,
	sf::Keyboard::T,
	sf::Keyboard::Y,
	sf::Keyboard::U,
	sf::Keyboard::I,
	sf::Keyboard::O,
	sf::Keyboard::P,
	sf::Keyboard::LBracket,
	sf::Keyboard::RBracket,
	sf::Keyboard::Backslash,
	sf::Keyboard::A,
	sf::Keyboard::S,
	sf::Keyboard::D,
	sf::Keyboard::F,
	sf::Keyboard::G,
	sf::Keyboard::H,
	sf::Keyboard::J,
	sf::Keyboard::K,
	sf::Keyboard::L,
	sf::Keyboard::Semicolon,
	sf::Keyboard::Quote,
	sf::Keyboard::Return,
	sf::Keyboard::LShift,
	sf::Keyboard::Z,
	sf::Keyboard::X,
	sf::Keyboard::C,
	sf::Keyboard::V,
	sf::Keyboard::B,
	sf::Keyboard::N,
	sf::Keyboard::M,
	sf::Keyboard::Comma,
	sf::Keyboard::Period,
	sf::Keyboard::Slash,
	sf::Keyboard::RShift,
	sf::Keyboard::LControl,
	sf::Keyboard::LAlt,
	sf::Keyboard::LSystem,
	sf::Keyboard::Space,
	sf::Keyboard::RSystem,
	sf::Keyboard::RAlt
};

class Key {
public:
	Key()
	{
	}
	
	Key(unsigned short l_id, sf::IntRect l_rect) : m_id(l_id), m_rect(l_rect)
	{
	}
	
	sf::IntRect GetRect() const
	{
		return this->m_rect;
	}
	
	void Draw(sf::RenderWindow &window, bool black_key) const
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
	
private:
	unsigned short m_id;
	sf::IntRect m_rect;
};

class Keyboard {
public:
	Keyboard(unsigned short l_size) : m_size(l_size)
	{
		unsigned short index = 0;
		unsigned char left;
		unsigned char height;
		unsigned char width;
		
		for (unsigned char i = 0; i < this->m_size; i++) {
			height = NOTE_HEIGHT;
			width = NOTES_WIDTH;
			left = 0;
			if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11) {
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
	
	Key GetKey(unsigned short l_id) const
	{
		return this->m_keys[l_id];
	}
	
	unsigned short GetSize() const
	{
		return this->m_size;
	}
	
	unsigned short GetWindowWidth() const
	{
		return this->m_window_width;
	}
	
	void Draw(sf::RenderWindow &window) const
	{
		for (unsigned char i = 0; i < this->m_size; i++) {
			this->m_keys[i].Draw(window, false);
		}
		for (unsigned char i = 0; i < this->m_size; i++) {
			this->m_keys[i].Draw(window, true);
		}
	}
private:
	unsigned short m_window_width;
	unsigned short m_size;
	std::vector<Key> m_keys;
};

class Note {
public:
	Note(unsigned char l_id, unsigned char l_strength) : m_id(l_id), m_strength(l_strength), m_size(1), m_moved(0)
	{
	}

	unsigned char GetId() const
	{
		return this->m_id;
	}
	
	unsigned char GetStrength() const
	{
		return this->m_strength;
	}

	void SizeUp()
	{
		this->m_size += SPEED;
	}

	unsigned short GetSize() const
	{
		return this->m_size;
	}

	void MoveUp()
	{
		this->m_moved += SPEED;
	}

	unsigned short GetMove() const
	{
		return this->m_moved;
	}
private:
	unsigned char m_id;
	unsigned char m_strength;
	unsigned short m_size;
	unsigned short m_moved;
};

std::vector<Note> notes;

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
	if (message->at(0) == 144) {
		key[message->at(1) - START_NOTE] = true;
		notes.push_back(Note(message->at(1) - START_NOTE, float(message->at(2)) / 100 * 255));
	}
	else if (message->at(0) == 128) {
		key[message->at(1) - START_NOTE] = false;
	}
	else {
		unsigned int nBytes = message->size();
		std::cout << "Unknow action: ";
		for (unsigned int i = 0; i < nBytes; i++)
	    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	  if (nBytes > 0)
	    std::cout << "stamp = " << deltatime << std::endl;//*/
	}
}

int main()
{
	Keyboard keyboard(NOTES_NB);
	sf::RenderWindow window(sf::VideoMode(keyboard.GetWindowWidth(), HEIGHT), "Midi Exposer");
	sf::Event event;
	sf::RectangleShape rectangle;
  RtMidiIn *midi = new RtMidiIn();
  unsigned int nPorts = midi->getPortCount();
	unsigned char key_display[NOTES_NB];

  if (nPorts != 0) {
		std::cout << nPorts << " ports available!" << std::endl;
  	midi->openPort(nPorts - 1);
  	midi->setCallback(&midiCallback);
  	midi->ignoreTypes(false, false, false);
	}
	else
		std::cout << "No ports available!" << std::endl;
	for (unsigned char i = 0; i < NOTES_NB; i++) {
		key[i] = false;
		key_display[i] = 0;
	}
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	while (window.isOpen()) {
      while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed)
              window.close();
					else if (event.type == sf::Event::KeyPressed) {
						for (unsigned short i = 0; i < KEYBOARD_KEY; i ++) {
							if (event.key.code == keyboard_key[i]) {
								if (key[i] != true) {
									key[i] = true;
									notes.push_back(Note(i, 255));
								}
							}
						}
					}
					else if (event.type == sf::Event::KeyReleased) {
						for (unsigned short i = 0; i < KEYBOARD_KEY; i ++) {
							if (event.key.code == keyboard_key[i])
								key[i] = false;
						}
					}
      }
			for (unsigned char i = 0; i < NOTES_NB; i++) {
				if (key[i]) {
					key_display[i] = 255;
					for (auto &note : notes) {
						if (i == note.GetId() && !note.GetMove())
							note.SizeUp();
					}
				}
				else {
					for (auto &note : notes) {
						if (i == note.GetId() && !note.GetMove())
							note.MoveUp();
					}
					if (key_display[i])
						key_display[i] -= (key_display[i] < NOTE_DISAPPEAR) ? key_display[i] : NOTE_DISAPPEAR;
				}
			}
			for (auto it = notes.begin(); it < notes.end(); it++) {
				if (it->GetMove() > HEIGHT - NOTE_HEIGHT)
					it = notes.erase(it);
			}
			window.clear(sf::Color::Black);
			if (SHOW_KEYBOARD)
				keyboard.Draw(window);
			for (unsigned char i = 0; i < NOTES_NB; i++) {
				sf::IntRect rect = keyboard.GetKey(i).GetRect();
				rectangle.setPosition(rect.left, HEIGHT - NOTE_HEIGHT);
				rectangle.setSize(sf::Vector2f(rect.width, (SHOW_KEYBOARD) ? rect.height : NOTE_HEIGHT));
				rectangle.setFillColor(sf::Color(255 - float(i) / NOTES_NB * 255, 0, float(i) / NOTES_NB * 255, key_display[i]));
				window.draw(rectangle);
			}
			for (auto &note : notes) {
				sf::IntRect rect = keyboard.GetKey(note.GetId()).GetRect();
				
				rectangle.setFillColor(sf::Color(0, 0, 0));
				rectangle.setPosition(rect.left - NOTES_SEP, HEIGHT - NOTE_HEIGHT - note.GetMove() - note.GetSize() - NOTES_SEP);
				rectangle.setSize(sf::Vector2f(rect.width + NOTES_SEP * 2, note.GetSize() + NOTES_SEP * 2));
				window.draw(rectangle);
				
				rectangle.setFillColor(sf::Color(255 - float(note.GetId()) / NOTES_NB * 255, 0, float(note.GetId()) / NOTES_NB * 255, note.GetStrength()));
				rectangle.setPosition(rect.left, HEIGHT - NOTE_HEIGHT - note.GetMove() - note.GetSize());
				rectangle.setSize(sf::Vector2f(rect.width, note.GetSize()));
				if (note.GetMove())
					note.MoveUp();
				window.draw(rectangle);
			}
      window.display();
  }
  delete midi;
  return 0;
}
