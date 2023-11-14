# Project/executable name
PROJECT_NAME := GNS

# Libraries to include with `pkg-config`
LIBRARIES := raylib

# Directories to use
BUILD_DIR := ./build
SRC_DIRS := ./src/src
INC_DIR := ./src/include

# Customizable CFLAGS and LDFLAGS
DEFINITIONS :=
CFLAGS :=
CXXFLAGS :=
LDFLAGS :=

RELEASE_DEFINITIONS := NDEBUG
RELEASE_CFLAGS := -O3 -g
RELEASE_CXXFLAGS := -O3 -g
RELEASE_LDFLAGS :=

DEBUG_DEFINITIONS := DEBUG
DEBUG_CFLAGS := -Wall -Wextra -g
DEBUG_CXXFLAGS := -Wall -Wextra -g
DEBUG_LDFLAGS :=


# Make debug the default target
.PHONY: default
default: debug

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
RELEASE_OBJS := $(SRCS:%=$(BUILD_DIR)/release/%.o)
DEBUG_OBJS   := $(SRCS:%=$(BUILD_DIR)/debug/%.o)
DEPS := $(RELEASE_OBJS:.o=.d) $(DEBUG_OBJS:.o=.d)

INC_DIRS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

DEFINITIONS := $(addprefix -D,$(DEFINITIONS))
RELEASE_DEFINITIONS := $(addprefix -D,$(RELEASE_DEFINITIONS))
DEBUG_DEFINITIONS := $(addprefix -D,$(DEBUG_DEFINITIONS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP

CFLAGS := $(CFLAGS) $(shell pkg-config --cflags $(LIBRARIES)) $(DEFINITIONS)
CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags $(LIBRARIES)) $(DEFINITIONS)
LDFLAGS := $(LDFLAGS) $(shell pkg-config --libs $(LIBRARIES)) -lm


#  RELEASE BUILD
# ===============

$(BUILD_DIR)/release/$(PROJECT_NAME): $(RELEASE_OBJS)
	$(CXX) $(RELEASE_OBJS) -o $@ $(LDFLAGS) $(RELEASE_LDFLAGS)

$(BUILD_DIR)/release/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(RELEASE_CFLAGS) $(RELEASE_DEFINITIONS) -c $< -o $@

$(BUILD_DIR)/release/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(RELEASE_CXXFLAGS) $(RELEASE_DEFINITIONS) -c $< -o $@


#   DEBUG BUILD
# ===============

$(BUILD_DIR)/debug/$(PROJECT_NAME): $(DEBUG_OBJS)
	$(CXX) $(DEBUG_OBJS) -o $@ $(LDFLAGS) $(DEBUG_LDFLAGS)

$(BUILD_DIR)/debug/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEBUG_CFLAGS) $(DEBUG_DEFINITIONS) -c $< -o $@

$(BUILD_DIR)/debug/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEBUG_CXXFLAGS) $(DEBUG_DEFINITIONS) -c $< -o $@


.PHONY: release
release: $(BUILD_DIR)/release/$(PROJECT_NAME)

.PHONY: debug
debug: $(BUILD_DIR)/debug/$(PROJECT_NAME)

.PHONY: all
all: release debug

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)