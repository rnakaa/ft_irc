NAME = ircserv
PORT = 6667
PASS = password
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)	
INC = include
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
FORMAT = clang-format -i

CNAME = client
CSRC = client.cpp

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	@mkdir -p $(@D)
	$(FORMAT) $<
	@if [ -f $(INC)/$*.hpp ]; then \
		$(FORMAT) $(INC)/$*.hpp; \
		echo "$(FORMAT) $(INC)/$*.hpp"; \
	fi
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INC)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJ_DIR)

fclean: clean cclean
	$(RM) $(NAME)

re: fclean all

run: all client
	./$(NAME) $(PORT) $(PASS)

client:
	$(CXX) $(CXXFLAGS) $(CSRC) -o $(CNAME)

cclean:
	$(RM) $(CNAME)

format:
	$(FORMAT) $(SRCS) $(wildcard $(INC)/*.hpp)

docker:
	sudo docker compose up --build -d

.PHONY: all fclean clean re run client cclean format
