NAME 		= webserv

SRCS		= main.cpp\
			srcs/Sockets/Socket.cpp\
			srcs/Servers/Server.cpp\
			srcs/Cluster/Cluster.cpp\
			srcs/Config/Config.cpp\
			srcs/Config/Location.cpp\
			srcs/Config/parser.cpp\
			srcs/HTTP_requests/Response.cpp\
			srcs/HTTP_requests/http_header.cpp\
			srcs/CGI/CGI.cpp


OBJS		= $(SRCS:.cpp=.o)

HEADER		= includes/Sockets/Socket.hpp\
			includes/Servers/Server.hpp\
			includes/Cluster/Cluster.hpp\
			includes/Config/Config.hpp\
			includes/Config/Location.hpp\
			includes/Config/parser.hpp\
			includes/HTTP_requests/Response.hpp\
			includes/CGI/CGI.hpp


INC			= -Iincludes/Sockets\
			 -Iincludes/Servers\
			 -Iincludes/Config\
			 -Iincludes\
			 -Iincludes/HTTP_requests\
			 -Iincludes/Cluster\
			 -Iincludes/CGI
			 
CC			= clang++

FLAGS		= -std=c++98 -Wall -Wextra -Werror -pthread

all:		$(NAME)

$(NAME):	$(SRCS) $(HEADER)
			$(CC) $(FLAGS) $(INC) $(SRCS) -o $(NAME)

%.o:		%.cpp
			$(CC) -g $(FLAGS) $(INC) -c $< -o $@

debug: FLAGS += -fsanitize=address -g
debug: all

clean:
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re