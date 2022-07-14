NAME				=	webserv

RM					=	rm -rf
OS					=	$(shell uname)
ifeq ($(OS), Linux)
	INCLUDE_KQUEUE	+=	-I/usr/include/kqueue
	LDFLAGS			+=	-lkqueue
	CC				=	g++
else
	CC				=	clang++
endif
CC					+=	-std=c++98
OPTFLAGS			=	-g #-fsanitize=address
FLAGS				=	-Wall -Wextra -Werror -pedantic

#Socket include
INC_SOCKET_PATH		=	Socket/
INC_SOCKET_NAME		=	BindingSocket.hpp ConnectionSocket.hpp ListeningSocket.hpp SimpleSocket.hpp
INC_SOCKET			=	$(addprefix $(INC_SOCKET_PATH), $(INC_SOCKET_NAME))
#Socket source
SRCS_SOCKET_PATH	=	Socket/
SRCS_SOCKET_NAME	=	BindingSocket.cpp ConnectionSocket.cpp ListeningSocket.cpp SimpleSocket.cpp
SRCS_SOCKET			=	$(addprefix $(SRCS_SOCKET_PATH), $(SRCS_SOCKET_NAME))

#Server include
INC_SERVER_PATH		=	Server/
INC_SERVER_NAME		=	SimpleServer.hpp HTTPServer.hpp
INC_SERVER			=	$(addprefix $(INC_SERVER_PATH), $(INC_SERVER_NAME))
#Server source
SRCS_SERVER_PATH	=	Server/
SRCS_SERVER_NAME	=	SimpleServer.cpp HTTPServer.cpp
SRCS_SERVER			=	$(addprefix $(SRCS_SERVER_PATH), $(SRCS_SERVER_NAME))

#Finish include
INC_PATH			=	./inc/
INC_NAME			=	$(INC_SOCKET) $(INC_SERVER)
INC					=	$(addprefix $(INC_PATH), $(INC_NAME))
#Finish source
SRCS_PATH			=	./src/
SRCS_NAME			=	$(SRCS_SOCKET) $(SRCS_SERVER) main.cpp
SRCS				=	$(addprefix $(SRCS_PATH), $(SRCS_NAME))
#Finish objs
OBJS_PATH			=	./objs/
FULL_OBJS_PATH		=	./objs/ ./objs/Server ./objs/Socket
OBJS				=	$(addprefix $(OBJS_PATH), $(SRCS_NAME:.cpp=.o))

all: ${NAME}

$(NAME): $(OBJS_PATH) $(OBJS)
	@$(CC) $(FLAGS) $(OPTFLAGS) $(INCLUDE_KQUEUE) -I$(INC_PATH) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "built $(NAME) finish"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp $(INC)
	@echo "Compiling: $<"
	@$(CC) $(FLAGS) $(OPTFLAGS) $(INCLUDE_KQUEUE) -I$(INC_PATH)  $(LDFLAGS) -c $< -o $@

$(OBJS_PATH):
	@mkdir -p ${FULL_OBJS_PATH}

clean:
	@echo "Cleaning: $(FULL_OBJS_PATH)"
	@$(RM) $(OBJS_PATH)

fclean:		clean
	@echo "Cleaning: $(NAME)"
	@$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re
