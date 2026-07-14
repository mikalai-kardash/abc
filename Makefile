ifeq ($(OS),Windows_NT)
	ROOT_DIR := $(subst \\,/,$(CURDIR))
	SDK_LIB_DIR := $(subst \\,/,$(ROOT_DIR)/sdk/c)
	RM := del /Q /F
else
	ROOT_DIR := $(shell pwd)
	SDK_LIB_DIR := $(ROOT_DIR)/sdk/c
	RM := rm -f
endif

export ROOT_DIR
export SDK_LIB_DIR
export RM

LIB_STATS   := $(ROOT_DIR)/sdk/c/stats
LIB_STATS_A := $(LIB_STATS)/libsdk_stats.a

APP_MEMORY_FALSE_SHARING_DIR := memory/false_sharing/c
APP_MEMORY_FALSE_SHARING_APP := $(APP_MEMORY_FALSE_SHARING_DIR)/fs

APP_CPU_BP_DIR := cpu/branch_prediction/c
APP_CPU_BP_APP := $(APP_CPU_BP_DIR)/bp


all: $(LIB_STATS_A) $(APP_MEMORY_FALSE_SHARING_APP) $(APP_CPU_BP_APP)

$(LIB_STATS_A):
	$(MAKE) -C $(LIB_STATS)

$(APP_MEMORY_FALSE_SHARING_APP): $(LIB_STATS_A)
	$(MAKE) -C $(APP_MEMORY_FALSE_SHARING_DIR)

$(APP_CPU_BP_APP): $(LIB_STATS_A)
	$(MAKE) -C $(APP_CPU_BP_DIR)

clean:
	$(MAKE) -C $(LIB_STATS) clean
	$(MAKE) -C $(APP_MEMORY_FALSE_SHARING_DIR) clean
	$(MAKE) -C $(APP_CPU_BP_DIR) clean

debug:
	@echo "pwd: $(PWD)"
	@echo "curdir: $(CURDIR)"
	@echo "root dir: $(ROOT_DIR)"
	@echo "sdk lib dir: $(SDK_LIB_DIR)"
	@echo "lib stats dir: $(LIB_STATS)"
	@echo "os: $(OS)"
