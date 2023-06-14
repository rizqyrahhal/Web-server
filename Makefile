# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/17 16:07:04 by rarahhal          #+#    #+#              #
#    Updated: 2023/06/14 21:47:20 by rarahhal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++

SRCS =	main.cpp \
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