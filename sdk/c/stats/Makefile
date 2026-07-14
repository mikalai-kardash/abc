TARGET  := libsdk_stats.a
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o) 

CFLAGS  := -Wall -Wextra


all: $(TARGET)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	$(RM) *.o $(TARGET)
