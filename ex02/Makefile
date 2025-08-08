# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 15:46:40 by maustel           #+#    #+#              #
#    Updated: 2025/08/08 12:15:46 by maustel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++17

SRC = main.cpp PmergeMe.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "\033[32m $@ built successfully! 🎯\033[0m"

%.o: %.cpp
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	@echo "\033[34m cleaning up ... 🧹 🗑\033[0m"
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
