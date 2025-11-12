CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -f

SRCS =	expand.c \
	expand2.c \
	export_utils1.c \
	export_utils2.c \
	free.c \
	ft_cd.c \
	ft_echo.c \
	ft_pwd.c \
	ft_split.c \
	ft_unset.c \
	ft_unset2.c \
	handle_command.c \
	fill_main.c \
	main.c \
	parse_pipe.c \
	path.c \
	path2.c \
	pipe.c \
	pipe2.c \
	quotes.c \
	quote_utils.c \
	readline.c \
	redir_in_out.c \
	redirec.c \
	redirections.c \
	heredoc_setup.c \
	heredoc_exec.c \
	signals.c \
	utils.c \
	utils2.c

OBJS = $(SRCS:.c=.o)

# libft
LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

NAME = minishell
LIBS = -L $(LIBFT_DIR) -lft -lreadline -ltinfo

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I $(LIBFT_DIR) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
