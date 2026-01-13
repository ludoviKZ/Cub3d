FILES = main.c player.c parsing.c parsing_utils.c utils.c checks.c checks_utils.c raycast.c raycast_extra.c texture.c texture_utils.c draw_slice.c movement.c key_hook.c
OBJ = $(FILES:.c=.o)
NAME = cub3D
MLX_PATH = minilibx/
MLX_LIB = $(MLX_PATH)libmlx.a
LIBFT_PATH = libft/
LIBFT_LIB = $(LIBFT_PATH)libft.a
CFLAGS = -Wall -Wextra -Werror -g
MLX_FLAGS = -lX11 -lXext -Lminilibx -lmlx 
MATH_LIBS = -lm
CC = cc
RM = rm -f

%.o : %.c
	@$(CC) $(CFLAGS) -Imlx -c -o $@ $<

all: adittionallibs $(NAME)

adittionallibs:
	@make -C $(MLX_PATH) all
	@make -C $(LIBFT_PATH) all

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(MLX_FLAGS) $(OBJ) $(MLX_LIB) $(LIBFT_LIB) $(MATH_LIBS) -o $(NAME)
	@echo "Cub3D compiled successfully!"
	
clean:
	make -C $(MLX_PATH) clean
	make -C $(LIBFT_PATH) clean
	${RM} $(OBJ)

fclean: clean
	make -C $(MLX_PATH) clean
	make -C $(LIBFT_PATH) fclean
	${RM} $(NAME)

re: fclean all

.PHONY: all clean fclean re
