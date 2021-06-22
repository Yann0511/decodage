NAME 	=	decode 

CFLAGS	=	-g2

HEADERS_DIR 	=	 $(shell find . -name "*.h")

SRC 	=	 $(shell find  -name "*.c")

OBJ  	=	$(SRC:.c=.o)

all: 	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(SRC) $(CFLAGS) $(HEADERS_DIRS)

temp:
	find . -name "*[#,~,.o]" -type f -delete

clean:   temp
	rm -f $(NAME)
	rm -f $(OBJ)

re: 	 fclean  all


