# **************************************************************************** #
#                                   VARIABLES                                  #
# **************************************************************************** #

# Program Name
NAME        = philo

# Directories
SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = Libft_GNL_Printf
OBJ_DIR     = obj

# Libft Setup
LIBFT       = $(LIBFT_DIR)/libft.a
LIBFT_REPO  = https://github.com/Artnebs/Libft_GNL_Printf.git

# Compiler and Flags
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR)/includes
LDFLAGS     = -L$(LIBFT_DIR) -lft

# Sources (all your .c files for the philo project)
SRCS        = $(SRC_DIR)/main.c \
              $(SRC_DIR)/philosopher.c \
              $(SRC_DIR)/utils.c

# Object files are placed in obj/ preserving subdirectory structure.
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                   TARGETS                                    #
# **************************************************************************** #

# Default rule: Build the program.
all: $(LIBFT) $(NAME)

# Linking the final binary from object files and Libft library.
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "Build complete!"

# Compile .c files into .o files in the obj/ directory (creates subdirectories as needed).
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build Libft: if the Libft directory does not exist, clone it.
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
