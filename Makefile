# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/14 15:37:50 by lde-sous          #+#    #+#              #
#    Updated: 2024/05/14 15:50:36 by andvieir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

#----------COMANDS----------#

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
RM = rm -rf

#----------DIRS----------#

NAME = webserv

SRC =	main.cpp \
		srcs/server/Server.cpp \
		srcs/server/ServerCluster.cpp \
		srcs/config/Config.cpp \
		srcs/requests/Request.cpp \
		srcs/Utils.cpp \
		srcs/HTML.cpp \
		srcs/responses/Response.cpp \
		srcs/responses/ResponseCode.cpp \
		srcs/server/Connection.cpp \

OBJ_D = bin
LOGS_D = logs

VALGRIND_LOG_DIR = logs/valgrind
VALGRIND_LOG_FILE = $(VALGRIND_LOG_DIR)/valgrind.log
DUMMY_FILE = .dummy_file

#----------OBJECTS----------#

OBJ = $(addprefix $(OBJ_D)/, $(SRC:.cpp=.o))

#----------COLORS----------#

BLACK	=	\033[0;90m
GREENER	=	\033[1;32m
GREEN	=	\033[0;92m
YELLOW	=	\033[0;93m
CYAN	=	\033[1;36m
RED		=	\033[1;31m
RESET	=	$(shell tput sgr0)

#----------DOCKER OPERATIONS----------#

# Define the Docker image name (adjust as needed)
DOCKER_IMAGE = lubuper/42_webserv_html:latest

# Define the path inside the container where files are located
CONTAINER_VAR_PATH = /usr/webserv/var

# Define the local target directory
LOCAL_VAR_PATH = .

# Verify if docker is installed
DOCKER_EXISTS := $(shell command -v docker > /dev/null 2>&1; echo $$?)

#----------RULES----------#

all: pull-and-copy-files $(NAME)


$(NAME): $(OBJ)
		@echo "$(YELLOW)Installing...$(RESET)"
		$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
		@echo "$(GREENER)Done!$(RESET)"

$(OBJ_D)/%.o: %.cpp
		mkdir -p $(@D)
		mkdir -p $(LOGS_D)
		$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to pull the Docker image and copy files to the local directory
pull-and-copy-files:
	@if [ "$(DOCKER_EXISTS)" -eq 0 ]; then \
		echo "$(CYAN)Pulling Docker image $(DOCKER_IMAGE)...$(BLACK)"; \
		docker pull $(DOCKER_IMAGE); \
		echo "$(GREEN)Creating temporary container to copy files...$(RESET)"; \
		docker create --name temp_webserv $(DOCKER_IMAGE); \
		echo "$(GREEN)Copying files from container to local directory $(LOCAL_VAR_PATH)...$(RESET)"; \
		mkdir -p $(LOCAL_VAR_PATH); \
		docker cp temp_webserv:$(CONTAINER_VAR_PATH) $(LOCAL_VAR_PATH); \
		echo "$(GREEN)Removing temporary container...$(RESET)"; \
		docker rm temp_webserv; \
		echo "$(GREEN)Files copied to $(LOCAL_VAR_PATH) successfully $(RESET)"; \
	else \
		echo "$(RED)Docker is not installed. Please install Docker first.$(RESET)"; \
		exit 1; \
	fi

clean:
		@echo "$(RED)Cleaning logs...$(RESET)"
		@if [ -f .dummy_file ]; then \
			echo "$(RED)Removing .dummy_file...$(BLACK)"; \
			$(RM) .dummy_file; \
		fi
		$(RM) $(LOGS_D)
		@echo "$(GREENER)Done!$(RESET)"

fclean:
	@echo "$(RED)Uninstalling...$(RESET)"
	@IMAGE_ID=$$(docker images -q lubuper/42_webserv_html:latest); \
	if [ -n "$$IMAGE_ID" ]; then \
		echo "$(RED)Image found, removing...$(BLACK)"; \
		docker image rm lubuper/42_webserv_html:latest; \
	else \
		echo "$(YELLOW)No image found, skipping...$(RESET)"; \
	fi
	@if [ -f .dummy_file ]; then \
		echo "$(RED)Removing .dummy_file...$(BLACK)"; \
		$(RM) .dummy_file; \
	fi
	@$(RM) $(OBJ) $(NAME) $(OBJ_D) var
	@echo "$(GREENER)Done!$(RESET)"

re: fclean all


# This rule defines a target called "run" which is used to execute the program.
# It first checks if the user running the make command has root privileges by comparing the user ID to 0.
# If the user ID is not 0 (indicating a non-root user), it displays a warning message about the program requiring sudo privileges for ports under 1024.
# It then prompts the user to enter the path to the configuration file using the "read" command.
# After reading the input, it executes the program "./webserv" with the provided configuration file path as an argument.

run:
	if [ "$$(id -u)" -ne 0 ]; then \
		echo "$(RED)WARNING: This program requires sudo privileges to run if you are setting ports under 1024.$(RESET)"; \
	fi; \
	read -p "Enter the path to the config file: " CONFIG_FILE_PATH; \
	./webserv $$CONFIG_FILE_PATH; \



#----------VALGRIND----------#
# This section defines rules related to running the Valgrind tool.
# If the CONFIG_FILE variable is defined, the valgrind target depends on the check_config_file and run_valgrind rules.
# If CONFIG_FILE is not defined, attempting to run the valgrind target will display an error message and exit with a non-zero status code.
# The check_config_file rule checks if the configuration file specified by CONFIG_FILE exists.
#		If it does not exist, it displays an error message and exits with a non-zero status code.
# The run_valgrind rule runs the Valgrind tool with specified options and the configuration file.
# The $(VALGRIND_LOG_DIR) rule ensures the directory for Valgrind logs exists.
# The $(DUMMY_FILE) rule ensures the existence of a dummy file used for control flow.

ifdef CONFIG_FILE
valgrind: check_config_file run_valgrind
else
valgrind:
	@echo "$(RED)➜$(RESET) $(CYAN)make:$(RESET) $(RED)Error:$(RESET) No configuration file provided. $(GREEN)Usage:$(RESET) make valgrind $(GREEN)CONFIG_FILE=$(RESET)<config_file>"
	-@exit 1
endif

check_config_file:
	@if [ ! -f "$(CONFIG_FILE)" ]; then \
		echo "$(RED)➜$(RESET) $(CYAN)make:$(RESET) $(RED)Error:$(RESET) Configuration file '$(CONFIG_FILE)' not found."; \
		exit 1; \
	fi

run_valgrind: $(DUMMY_FILE) | $(VALGRIND_LOG_DIR)
	valgrind --show-leak-kinds=all --leak-check=full --track-origins=yes --log-file=$(VALGRIND_LOG_FILE) ./webserv $(CONFIG_FILE)
	@rm -f $(DUMMY_FILE)

$(VALGRIND_LOG_DIR):
	mkdir -p $(VALGRIND_LOG_DIR)

$(DUMMY_FILE):
ifndef CONFIG_FILE
	@echo "$(RED)➜$(RESET) $(CYAN)make:$(RESET) $(RED)Error:$(RESET) No configuration file provided. $(GREEN)Usage:$(RESET) make valgrind $(GREEN)CONFIG_FILE=$(RESET)<config_file>"
	-@exit 1
endif
	@touch $(DUMMY_FILE)

.PHONY: all clean fclean re check_config_file pull-and-copy-files

.SILENT:
