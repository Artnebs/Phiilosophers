# **************************************************************************** #
#                                   VARIABLES                                  #
# **************************************************************************** #

NAME        = philo

SRC_DIR     = srcs
INC_DIR     = includes
OBJ_DIR     = obj

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDES    = -I$(INC_DIR)
LDFLAGS     =

SRCS        = $(SRC_DIR)/main.c \
              $(SRC_DIR)/actions.c \
              $(SRC_DIR)/init.c \
              $(SRC_DIR)/monitor.c \
              $(SRC_DIR)/routine.c \
              $(SRC_DIR)/utils.c \
              $(SRC_DIR)/utils2.c 

OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                   TARGETS                                    #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "Build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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

# fclean: Remove object files and the final binary.
fclean: clean
	@echo "Removing $(NAME) binary..."
	rm -f $(NAME)

# re: Rebuild everything.
re: fclean all

# **************************************************************************** #
#                                   PHONY                                      #
# **************************************************************************** #
.PHONY: all clean fclean re valgrind debug
