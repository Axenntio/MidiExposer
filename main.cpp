#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "defines.hpp"
#include "Keyboard.hpp"
#include "Note.hpp"
#include "RtMidi.h"

std::vector<Note> notes;
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

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
	if (message->at(0) == 144) {
		key[message->at(1) - START_NOTE] = true;
		notes.push_back(Note(message->at(1) - START_NOTE, float(message->at(2)) / 100 * 255, sf::Color(255 - float(message->at(1)) / NOTES_NB * 255, 0, float(message->at(1)) / NOTES_NB * 255, (STRENGHT_SENSIBILITY) ? float(message->at(2)) / 100 * 255 : 255)));
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
	Keyboard keyboard(NOTES_NB, 0);
	sf::RenderWindow window(sf::VideoMode(keyboard.GetWindowWidth(), HEIGHT), "Midi Exposer");
	sf::Event event;
	sf::RectangleShape rectangle;
  RtMidiIn *midi = new RtMidiIn();
  unsigned int nPorts = midi->getPortCount();
	unsigned char key_display[NOTES_NB];

  if (nPorts != 0) {
		std::cout << "There is " << nPorts << " port(s) available!" << std::endl;
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
									notes.push_back(Note(i, 255, sf::Color(255 - float(i) / NOTES_NB * 255, 0, float(i) / NOTES_NB * 255)));
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

				rectangle.setFillColor(note.GetColor());
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
