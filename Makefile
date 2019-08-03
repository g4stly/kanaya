CC 	= gcc
CFLAGS 	= -Wall -c -g
AR	= ar
ARFLAGS	= rcs

LIBS 	= -lclass
LIBDIR	= lib
INCLUDE	= include

PROJECT	= kanaya
TARGET 	= lib/lib$(PROJECT).a
SOURCES = src/list.c src/stack.c src/util.c
OBJECTS	= $(SOURCES:.c=.o)

TEST	= bin/$(PROJECT)
TEST_SRC= src/main.c
TEST_OBJ= $(TEST_SRC:.c=.o)

all: $(TARGET) $(TEST)

$(TEST): $(TARGET) $(TEST_OBJ)
	@mkdir -p bin
	$(CC) -o $@ $(TEST_OBJ) -L$(LIBDIR) $(LIBS) -l$(PROJECT)
$(TARGET): $(OBJECTS)
	@mkdir -p bin lib
	$(AR) $(ARFLAGS) $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $<

.PHONY: clean
clean: 
	rm -rf $(OBJECTS) $(TEST_OBJ) $(TARGET)
test: all
	./$(TEST)
	make -s clean
