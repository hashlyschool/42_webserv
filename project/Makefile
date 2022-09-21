NAME			=	webserv

RM				=	rm -rf
CC				=	g++
OPTFLAGS		=	#-g -fsanitize=address
FLAGS			=	-std=c++98 -Wall -Wextra -Werror -pedantic -MMD -MP

#Main
INCS_FILES		=	ALocation.hpp ConfigServer.hpp Location.hpp Parser.hpp \
					Responder.hpp Socket.hpp Cgi.hpp HttpRequest.hpp \
					Webserv.hpp HttpResponse.hpp HttpUtils.hpp \
					Utils.hpp DataFd.hpp Buffer.hpp

SOURCE_FILES	=	ALocation.cpp ConfigServer.cpp Location.cpp Parser.cpp \
					Responder.cpp Socket.cpp Webserv.cpp Cgi.cpp \
					HttpRequest.cpp  HttpResponse.cpp HttpUtils.cpp\
					main.cpp Utils.cpp DataFd.cpp Buffer.cpp

#Finish include
INC_PATH		=	./inc/
INC_NAME		=	$(INCS_FILES)
INC				=	$(addprefix $(INC_PATH), $(INC_NAME))

#Finish source
SRCS_PATH		=	./src/
SRCS_NAME		=	$(SOURCE_FILES)
SRCS			=	$(addprefix $(SRCS_PATH), $(SRCS_NAME))
#Finish objs
OBJS_PATH		=	./objs/
OBJS			=	$(addprefix $(OBJS_PATH), $(SRCS_NAME:.cpp=.o))
#Path to servers
WWW_PATH		= ./www/*
WWW_PATH_COPY	= /tmp/www/

all: ${NAME}

$(NAME): $(OBJS_PATH) $(WWW_PATH_COPY) $(OBJS)
	@$(CC) $(FLAGS) $(OPTFLAGS) -I$(INC_PATH) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "built $(NAME) finish"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp $(INC)
	@$(CC) $(FLAGS) $(OPTFLAGS) -I$(INC_PATH) -c $< -o $@
	@echo "Compiling: $<"

$(OBJS_PATH):
	@mkdir -p ${OBJS_PATH}

$(WWW_PATH_COPY):
	@mkdir -p ${WWW_PATH_COPY}
	@cp -r ${WWW_PATH} ${WWW_PATH_COPY}

clean:
	@echo "Cleaning: $(OBJS_PATH)"
	@$(RM) $(OBJS_PATH)

fclean:		clean
	@echo "Cleaning: $(NAME)"
	@$(RM) $(NAME)
	@$(RM) $(WWW_PATH_COPY)

re:			fclean all

.PHONY: all clean fclean re
