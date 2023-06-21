SRC = main.cpp \
	parcing.cpp \
	web_serv.cpp \
	networking.cpp \
	./sources/responsePart.cpp \
	./sources/HttpResponse.cpp \
	./sources/error_page.cpp \

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = web_serv
CC = c++
OBJ = ${SRC:.cpp=.o}

all : $(NAME)
$(NAME) : $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
		
clean :
	rm -f $(OBJ)
fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY:	all clean fclean re