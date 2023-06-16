# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/17 16:07:04 by rarahhal          #+#    #+#              #
#    Updated: 2023/06/16 22:39:59 by rarahhal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address
CC = c++

SRCS =	main.cpp \
		server-side.cpp \
		response.cpp \

OBJS = $(SRCS:.cpp=.o)

$(NAME): $(OBJS)
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf *.o

fclean: clean
	@rm -f $(NAME)

re: fclean
	@make all

.PHONY: all clean fclean re