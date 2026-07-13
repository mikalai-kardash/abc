export ROOT_DIR := $(shell pwd)
export SDK_LIB_DIR := $(ROOT_DIR)/sdk/c


LIB_STATS    := sdk/c/stats
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
	@for dir in $(LIB_STATS) $(APP_MEMORY_FALSE_SHARING_DIR); do \
		$(MAKE) -C $$dir clean; \
	done
