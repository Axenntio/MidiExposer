#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

#define HEIGHT 800

#define START_NOTE 21
#define NOTES_NB 88

#define NOTE_DISAPPEAR 30

#define NOTES_WIDTH 16
#define NOTE_HEIGHT 80
#define NOTES_SEP 2

bool key[NOTES_NB];

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
  //unsigned int nBytes = message->size();
	
	if (message->at(0) == 144)
		key[message->at(1) - START_NOTE] = true;
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
	for (unsigned short i = 0; i < NOTES_NB; i++) {
		key[i] = false;
		key_display[i] = 0;
	}
	rectangle.setSize(sf::Vector2f(NOTES_WIDTH, NOTE_HEIGHT));
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	while (window.isOpen()) {
      while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed)
              window.close();
      }
			for (unsigned short i = 0; i < NOTES_NB; i++) {
				if (key[i])
					key_display[i] = 255;
				else if (key_display[i])
					key_display[i] -= (key_display[i] < NOTE_DISAPPEAR) ? key_display[i] : NOTE_DISAPPEAR;
			}
			window.clear(sf::Color::Black);
			for (unsigned short i = 0; i < NOTES_NB; i++) {
				rectangle.setPosition(i * (NOTES_WIDTH + NOTES_SEP), HEIGHT - NOTE_HEIGHT);
				rectangle.setFillColor(sf::Color(255, 0, 0, key_display[i]));
				window.draw(rectangle);
			}
      window.display();
  }
  delete midi;
  return 0;
}