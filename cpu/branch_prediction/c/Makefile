LIB_STATS := $(SDK_LIB_DIR)/stats
SRCS    := bp.c
TARGET  := bp
CCFLAGS := -Wall -Wextra -I$(LIB_STATS)
LDFLAGS := -L$(LIB_STATS) -lsdk_stats -lm


all: $(TARGET)

$(TARGET): $(SRCS)
	gcc -o $(TARGET) $(SRCS) $(CCFLAGS) $(LDFLAGS)

clean:
	$(RM) bp bp.exe
