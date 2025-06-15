NAME		:= philo
CC			:= cc
CFLAGS		:= -pthread -Wall -Wextra -Werror -g3
SRCS		:= main.c utils.c
OBJS		:= $(SRCS:.c=.o)
MAKEFLAGS	+= --no-print-directory

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all