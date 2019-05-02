ifeq ($(OS), Windows_NT)
        LDFLAGS= -LC:\SFML-2.1\lib -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-audio-s-d -lsfml-network-s-d
        PCFLAGS= -g -DSFML_STATIC -IC:\SFML-2.1\include
else
        LDFLAGS= -I /usr/local/include/ -L /usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
        PCFLAGS= -I /usr/local/include -D__MACOSX_CORE__
endif

CC	= g++
RM	= rm
CFLAGS	= $(PCFLAGS)
SRCS	= main.cpp \
	  RtMidi.cpp
OBJS	= $(SRCS:%.cpp=%.o) 
NAME	= MidiExposer

all	:	$(NAME)

$(NAME)	:	$(OBJS) 
		$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) -f $(OBJS)

fclean	:	clean
		$(RM) -f $(NAME)

re	:	fclean all

.PHONY	:	all clean fclean re
