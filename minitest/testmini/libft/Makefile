LIBC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c \
      ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c \
      ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
      ft_tolower.c ft_memset.c ft_calloc.c ft_strdup.c ft_atoi.c ft_strcat.c ft_strcpy.c \
      ft_strndup.c

SUPL = ft_substr.c ft_strjoin.c ft_strmapi.c ft_itoa.c ft_strtrim.c ft_split.c \
       ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c 

BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c \
ft_lstmap_bonus.c
SRC = $(LIBC) $(SUPL)

SRCBON = $(BONUS)

OBJ = $(SRC:.c=.o)

OBJBON = $(SRCBON:.c=.o)

NAME = libft.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I.

all: ${NAME}

${NAME}: ${OBJ}
	ar rcs ${NAME} ${OBJ}

bonus: $(OBJBON)
	ar rcs ${NAME} ${OBJ} ${OBJBON}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OBJ} ${OBJBON}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: re clean fclean all bonus