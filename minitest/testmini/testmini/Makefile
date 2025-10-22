CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -f

SRCS =	ft_export.c \
	ft_pwd.c \
	ft_split.c \
	path.c \
	ft_cd.c \
	readline.c \
	redirections.c

OBJS = $(SRCS:.c=.o)
NAME = minishell
LIBS = -lreadline -lncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re