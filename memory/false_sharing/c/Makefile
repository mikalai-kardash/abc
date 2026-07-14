TARGET := fs

LIB_STATS := $(SDK_LIB_DIR)/stats

CCFLAGS := -Wall -Wextra -I"$(LIB_STATS)"
LDFLAGS := -lsdk_stats -lpthread -lm
LLFLAGS := -L"$(LIB_STATS)"

all: $(TARGET)

$(TARGET): fs.c
	gcc $(CCFLAGS) $(LLFLAGS) fs.c $(LDFLAGS) -o $@

clean:
	$(RM) fs fs.exe
