TARGET := fs

LIB_STATS := $(SDK_LIB_DIR)/stats

INCLUDE := -Wall -Wextra -I$(LIB_STATS)
LINK    := -L$(LIB_STATS) -lsdk_stats -lpthread -lm

all: $(TARGET)

$(TARGET): fs.c
	gcc -o $@ fs.c $(INCLUDE) $(LINK)

clean:
