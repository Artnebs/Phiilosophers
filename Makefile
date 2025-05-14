# **************************************************************************** #
#                                   VARIABLES                                  #
# **************************************************************************** #

NAME        = philo

SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = Libft_GNL_Printf
OBJ_DIR     = obj

LIBFT       = $(LIBFT_DIR)/libft.a
LIBFT_REPO  = https://github.com/Artnebs/Libft_GNL_Printf.git

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR)/includes
LDFLAGS     = -L$(LIBFT_DIR) -lft


SRCS        = $(SRC_DIR)/main.c \
              $(SRC_DIR)/actions.c \
              $(SRC_DIR)/init.c \
              $(SRC_DIR)/monitor.c \
              $(SRC_DIR)/routine.c \
              $(SRC_DIR)/utils.c 

OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                   TARGETS                                    #
# **************************************************************************** #

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "Build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "Cloning Libft repository..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@echo "Building Libft library..."
	@$(MAKE) -C $(LIBFT_DIR)

# ---------------------------------------------------------------------------- #
# Debug and Valgrind Targets
# ---------------------------------------------------------------------------- #

# Valgrind: build the program then run it with full leak checking.
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(NAME)

# Debug: add debugging flags (-g -DDEBUG) and rebuild everything.
debug: CFLAGS += -g -DDEBUG
debug: re

# ---------------------------------------------------------------------------- #
# Cleaning Rules
# ---------------------------------------------------------------------------- #

# clean: Remove object files.
clean:
	@echo "Cleaning object files in $(OBJ_DIR)/..."
	rm -rf $(OBJ_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) clean; \
	fi

# fclean: Remove object files and the final binary.
fclean: clean
	@echo "Removing $(NAME) binary..."
	rm -f $(NAME)
	@echo "Removing Libft directory (optional)..."
	rm -rf $(LIBFT_DIR)

# re: Rebuild everything.
re: fclean all

# **************************************************************************** #
#                                   PHONY                                      #
# **************************************************************************** #
.PHONY: all clean fclean re valgrind debug
