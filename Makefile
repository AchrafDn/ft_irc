CC = c++
Cflags = -Wall -Wextra -Werror -std=c++98
Cfiles = main.cpp Server.cpp Channel.cpp Join.cpp Mode.cpp
# Cfiles = Test.cpp


Name = irc
RM = rm -rf

OBJ = ${Cfiles:.cpp=.o}


all: $(Name)

$(Name) : $(OBJ)
	$(CC) $(Cflags) $(OBJ) -o $(Name)

.cpp.o:
	$(CC) $(Cflags) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(Name)

re : fclean all