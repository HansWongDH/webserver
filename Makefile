NAME        = webserv

SRC_DIR     = srcs
OBJ_DIR     = obj
INC_DIR     = includes

SRC_FILES   = main.cpp \
			$(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

GCC         = c++
CFLAGS      = -Wall -Wextra -Werror
# C++FLAGS    = -std=c++98
SANITIZE    = -fsanitize=address -g3
RM          = rm -f

all:        $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(GCC) $(C++FLAGS) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

$(NAME):	$(OBJ_FILES)
	$(GCC) $(C++FLAGS) -o $(NAME) $(OBJ_FILES) $(SANITIZE)

test:	$(NAME)
	./$(NAME)

clean:
	$(RM) -r $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME)
	$(RM) output.log

re:	fclean all

.PHONY:	all clean fclean re test vector stack map set
