#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apavlov <apavlov@student.unit.ua>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/02 15:43:48 by apavlov           #+#    #+#              #
#    Updated: 2019/08/02 15:43:53 by apavlov          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC = gcc

FLAGS =  -Wall -Wextra #-g -fsanitize=address #-Werror

NAME = RT

SRC =	main.c init_cl.c init_sdl.c output.c parser.c render.c user_event.c math.c

HEADERS = rt.h parse.h terminal_colors.h mymath.h

INC_DIR = ./includes/

INC = $(addprefix $(INC_DIR), $(HEADERS))

SRC_DIR = ./src/

OBJ_DIR = ./obj/



OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

FT = ./libft/

FT_LIB	= $(addprefix $(FT),libft.a)

LINKS = -L$(FT) -l ft -lm
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LINKS += -lSDL2 -lSDL2_image -lOpenCL
endif
SDL_PATH = ./framework

INCLUDES = 		-I$(FT) -I$(INC_DIR) 

ifeq ($(UNAME_S),Darwin)
INCLUDES += -I./frameworks/SDL2.framework/Headers \
			-I./frameworks/SDL2_image.framework/Headers \
			-F./frameworks
endif

				

FRAMEWORKS :=
ifeq ($(UNAME_S),Darwin)
FRAMEWORKS += 	-F./frameworks \
				-rpath ./frameworks \
				-framework AppKit -framework OpenGL \
				-framework SDL2 -framework SDL2_image \
				-framework OpenCL
endif

all: obj_dir $(FT_LIB) $(NAME)
	echo 'Compilated!'

obj_dir:
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(FRAMEWORKS) $(OBJ) -o $(NAME) $(LINKS)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(INC)
	$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<

$(FT_LIB):
	make -C $(FT)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(FT) clean

fclean: clean
	rm -f $(NAME)
	make -C $(FT) fclean

re: fclean all
