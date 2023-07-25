NAME = webserv
CPPFLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++


SRCS = main.cpp \
	networking/parcing.cpp \
	networking/web_serv.cpp \
	networking/networking.cpp \
	networking/client.cpp \
	response/sources/responsePart.cpp \
	response/sources/HttpResponse.cpp \
	response/sources/error_page.cpp \
	response/sources/beforeMethods.cpp \
	response/sources/GetMethod.cpp \
	response/sources/DeleteMethod.cpp \
	response/sources/PostMethod.cpp \
	response/sources/responseutils.cpp \
	response/sources/utils.cpp \
	response/sources/mimeTypes.cpp \
	response/sources/cgi.cpp \
	response/sources/Response.cpp \




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
