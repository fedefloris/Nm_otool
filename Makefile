# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffloris <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/21 14:59:06 by ffloris           #+#    #+#              #
#    Updated: 2019/03/21 14:59:16 by ffloris          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM = ft_nm
OTOOL = ft_otool
NAME = $(NM) $(OTOOL)

OS_TYPE = $(shell uname)

LIBFT_DIR = libft

NM_DIR = nm
OTOOL_DIR = otool

all: $(NAME)

test: $(NAME)
	# ------- NM -------
ifeq ($(OS_TYPE), Darwin)
	@FUNCTION=nm OPTIONS= VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /bin
	@FUNCTION=nm OPTIONS= VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /usr/bin
	@FUNCTION=nm OPTIONS= VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /usr/lib
	@FUNCTION=nm OPTIONS= VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh ./examples/mach_o
else
	@FUNCTION=nm OPTIONS=-p VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /bin
	@FUNCTION=nm OPTIONS=-p VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /usr/bin
	@FUNCTION=nm OPTIONS=-p VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh ./examples/elf/good_files
endif
ifeq ($(OS_TYPE), Darwin)
	# ------- OTOOL -------
	@FUNCTION=otool OPTIONS=-t VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /bin
	@FUNCTION=otool OPTIONS=-t VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /usr/bin
	@FUNCTION=otool OPTIONS=-t VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh /usr/lib
	@FUNCTION=otool OPTIONS=-t VERBOSE=1 USE_VALGRIND=1 ./scripts/test.sh ./examples/mach_o
endif

comp_libft:
	@make -C $(LIBFT_DIR)/

comp_nm: comp_libft
	@make -C $(NM_DIR)/

comp_otool: comp_libft
	@make -C $(OTOOL_DIR)/

$(NM): comp_nm $(NM_DIR)/$(NM)
	@cp $(NM_DIR)/$(NM) $(NM)

$(OTOOL): comp_otool $(OTOOL_DIR)/$(OTOOL)
	@cp $(OTOOL_DIR)/$(OTOOL) $(OTOOL)

clean:
	@make -C $(LIBFT_DIR)/ clean
	@make -C $(NM_DIR)/ clean
	@make -C $(OTOOL_DIR)/ clean

fclean:
	@make -C $(LIBFT_DIR)/ fclean
	@make -C $(NM_DIR)/ fclean
	@make -C $(OTOOL_DIR)/ fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all test comp_libft comp_nm comp_otool clean fclean re
