NAME = webserv
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address
CC = c++


SRCS = main.cpp \
	networking/parcing.cpp \
	networking/web_serv.cpp \
	networking/networking.cpp \
	response/sources/responsePart.cpp \
	response/sources/HttpResponse.cpp \
	response/sources/error_page.cpp \
	response/sources/beforeMethods.cpp \
	response/sources/GetMethod.cpp \
	response/sources/responseutils.cpp \
	response/sources/utils.cpp \
	response/sources/mimeTypes.cpp \



OBJS = $(SRCS:.cpp=.o)

$(NAME): $(OBJS)
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean
	@make all

.PHONY: all clean fclean re



# SRC = main.cpp \
# 	networking/parcing.cpp \
# 	networking/web_serv.cpp \
# 	networking/networking.cpp \
# 	response/sources/responsePart.cpp \
# 	response/sources/HttpResponse.cpp \
# 	response/sources/error_page.cpp \
# 	response/sources/beforeMethods.cpp \
# 	response/sources/GetMethod.cpp \
# 	response/sources/responseutils.cpp \
# 	response/sources/mimeTypes.cpp \


# CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
# NAME = web_serv
# CC = c++
# OBJ = ${SRC:.cpp=.o}

# all : $(NAME)
# $(NAME) : $(OBJ)
# 	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

# clean :
# 	rm -f $(OBJ)
# fclean : clean
# 	rm -f $(NAME)

# re : fclean all

# .PHONY:	all clean fclean re