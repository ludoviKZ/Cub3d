FILES = draw2D.c main.c player.c parsing.c utils.c checks.c raycast.c raycast_extra.c texture.c draw_slice.c movement.c
OBJ = $(FILES:.c=.o)
NAME = Cub3D
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
	clear

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
