NAME			=	webserv
OS = $(shell uname)
ifeq ($(OS), Linux)
	INCLUDE_KQUEUE += -I/usr/include/kqueue
	LDFLAGS += -lkqueue
	CC = g++
else
	CC = clang++
endif
CC += -std=c++98
RM				=	rm -rf
FLAGS			=	-Wall -Wextra -Werror
INC_PATH		=	./inc/
INC_NAME		=	Server.hpp Logger.hpp
INC				=	$(addprefix $(INC_PATH), $(INC_NAME))
SRCS_PATH		=	./src/
OBJS_PATH		=	./objs/
SRCS_NAME		=	main.cpp Server.cpp Logger.cpp
SRCS			=	$(addprefix $(SRCS_PATH), $(SRCS_NAME))
OBJS			=	$(addprefix $(OBJS_PATH), $(SRCS_NAME:.cpp=.o))

OPTFLAGS		=	-g #-fsanitize=address

all: ${NAME}

$(NAME): $(OBJS_PATH) $(OBJS)
	@$(CC) $(FLAGS) $(OPTFLAGS) $(INCLUDE_KQUEUE) -I $(INC_PATH) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "built $(NAME) finish"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp $(INC)
	@echo "Compiling: $<"
	@$(CC) $(FLAGS) $(OPTFLAGS) $(INCLUDE_KQUEUE) -I $(INC_PATH)  $(LDFLAGS) -c $< -o $@

$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)

clean:
	@echo "Cleaning: $(OBJS_PATH)"
	@$(RM) $(OBJS_PATH)

fclean:		clean
	@echo "Cleaning: $(NAME)"
	@$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re
