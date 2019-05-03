#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

#define HEIGHT 1600

#define SPEED 3

#define START_NOTE 21
#define NOTES_NB 88

#define NOTE_DISAPPEAR 30

#define NOTES_WIDTH 32
#define NOTE_HEIGHT 10
#define NOTES_SEP 2

bool key[NOTES_NB];

class Note {
public:
	Note(unsigned char l_id) : m_id(l_id), m_size(1), m_moved(0)
	{
	}

	unsigned char getId() const
	{
		return this->m_id;
	}

	void sizeUp()
	{
		this->m_size += SPEED;
	}

	unsigned short getSize() const
	{
		return this->m_size;
	}

	void moveUp()
	{
		this->m_moved += SPEED;
	}

	unsigned short getMove() const
	{
		return this->m_moved;
	}
private:
	unsigned char m_id;
	unsigned short m_size;
	unsigned short m_moved;
};

std::vector<Note> notes;

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
  //unsigned int nBytes = message->size();

	if (message->at(0) == 144) {
		key[message->at(1) - START_NOTE] = true;
		notes.push_back(Note(message->at(1) - START_NOTE));
	}
	if (message->at(0) == 128)
		key[message->at(1) - START_NOTE] = false;
  /*for (unsigned int i=0; i < nBytes; i++)
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if (nBytes > 0)
    std::cout << "stamp = " << deltatime << std::endl;//*/
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(NOTES_NB * (NOTES_WIDTH + NOTES_SEP), HEIGHT), "Midi Exposer");
	sf::Event event;
	sf::RectangleShape rectangle;
  RtMidiIn *midi = new RtMidiIn();
  unsigned int nPorts = midi->getPortCount();
	unsigned char key_display[NOTES_NB];

  if (nPorts != 0) {
  	midi->openPort(0);
  	midi->setCallback(&midiCallback);
  	midi->ignoreTypes(false, false, false);
	}
	else
		std::cout << "No ports available!\n";
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
						switch (event.key.code) {
							case sf::Keyboard::Q:
								key[40] = true;
								notes.push_back(Note(40));
								break;
							default:
								break;
						}
					}
					else if (event.type == sf::Event::KeyReleased) {
						switch (event.key.code) {
							case sf::Keyboard::Q:
								key[40] = false;
								break;
							default:
								break;
						}
					}
      }
			for (unsigned char i = 0; i < NOTES_NB; i++) {
				if (key[i]) {
					key_display[i] = 255;
					for (auto &note : notes) {
						if (i == note.getId() && !note.getMove())
							note.sizeUp();
					}
				}
				else {
					for (auto &note : notes) {
						if (i == note.getId() && !note.getMove())
							note.moveUp();
					}
					if (key_display[i])
						key_display[i] -= (key_display[i] < NOTE_DISAPPEAR) ? key_display[i] : NOTE_DISAPPEAR;
				}
			}
			window.clear(sf::Color::Black);
			rectangle.setSize(sf::Vector2f(NOTES_WIDTH, NOTE_HEIGHT));

			for (unsigned char i = 0; i < NOTES_NB; i++) {
				rectangle.setPosition(i * (NOTES_WIDTH + NOTES_SEP), HEIGHT - NOTE_HEIGHT);
				rectangle.setFillColor(sf::Color(255, 0, 0, key_display[i]));
				window.draw(rectangle);
			}
			rectangle.setFillColor(sf::Color(255, 0, 0, 255));
			for (auto &note : notes) {
				rectangle.setPosition(note.getId() * (NOTES_WIDTH + NOTES_SEP), HEIGHT - NOTE_HEIGHT - note.getMove() - note.getSize());
				rectangle.setSize(sf::Vector2f(NOTES_WIDTH, note.getSize()));
				if (note.getMove())
					note.moveUp();
				window.draw(rectangle);
			}
      window.display();
  }
  delete midi;
  return 0;
}
