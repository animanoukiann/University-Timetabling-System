NAME    = UniversityTimetable

SRCS 	=	$(wildcard *.cpp)

HEADER	=	$(wildcard *.hpp)

OBJS 	=	$(patsubst %.cpp, %.o, $(SRCS))

CC 			=	c++

RM      = rm -fr

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADER)
	@$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re