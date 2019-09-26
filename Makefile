# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myuliia <myuliia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/02 15:43:48 by apavlov           #+#    #+#              #
#    Updated: 2019/09/26 16:12:13 by myuliia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

FLAGS =  -Wall -Wextra -g #-fsanitize=address #-Werror
NAME = RT

SRC =	main.c init_cl.c init_sdl.c output.c parser.c render.c user_event.c \
		math.c init.c mouse_events.c functions.c init_cl2.c init_cl3.c\
		xml/xml_read.c xml/xml_create_obj.c xml/xml_add_param.c \
		xml/xml_add_param_help.c xml/xml_add_light.c xml/xml_create_cam.c xml/xml_it_is.c \
		xml/xml_save.c xml/xml_write_obj.c xml/xml_write_other.c xml/xml_write_param.c \
		xml/xml_add_param2.c button/init_but.c button/action_but.c editor/check_edit.c \
		xml/xml_create_obj2.c filter.c

HEADERS = rt.h parse.h terminal_colors.h mymath.h editor.h functions.h

INC_DIR = ./includes/

INC = $(addprefix $(INC_DIR), $(HEADERS))

SRC_DIR = ./src/

OBJ_DIR = ./obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

FT = ./libft/

MXML = ./frameworks/mxml-3.0/lib #add folder path ++

FT_LIB	= $(addprefix $(FT),libft.a)

LINKS = -L$(FT) -l ft -lm #add link mxml ++
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LINKS += -lSDL2 -lSDL2_image -lOpenCL -lmxml -lpthread
else
	LINKS += -L$(MXML) -lmxml
endif
SDL_PATH = ./framework

INCLUDES = 		-I$(FT) -I$(INC_DIR) -I./frameworks/mxml-3.0/include

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

COLOR_BLUE =	"\x1b[36m"
COLOR_NONE =	"\x1b[0m"
COLOR_RED =		"\x1b[31m"	

all: obj_dir $(FT_LIB) $(NAME)
	@echo "\n 	"$(COLOR_BLUE) [ 😎 $(NAME) Compilated! ] "\x1b[0m\n"

obj_dir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/xml
	@mkdir -p $(OBJ_DIR)/button
	@mkdir -p $(OBJ_DIR)/editor

xml:
	/lib/mxml-3.0

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(FRAMEWORKS) $(OBJ) -o $(NAME) $(LINKS)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(INC)
	@$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<
	@echo $(DELETE_PREV) $(COLOR_BLUE) created: "\x1b[97;46m" $(@:%=%) $(COLOR_NONE)

$(FT_LIB):
	@make -C $(FT)
	@echo "\n 	"$(COLOR_BLUE) [ 😎 $(FT_LIB) ] "\n"

clean:
	@rm -rf $(OBJ_DIR)
	@echo $(COLOR_RED) [X] $(OBJ_DIR) $(COLOR_NONE)
	@make -C $(FT) clean
	@echo $(COLOR_RED) [X] $(FT) $(COLOR_NONE)

fclean: clean
	@rm -f $(NAME)
	@echo $(COLOR_RED) [X] $(NAME) $(COLOR_NONE) "\n"
	@make -C $(FT) fclean

re: fclean all
