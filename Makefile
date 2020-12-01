#BUILD ?= build
# in-tree build
BUILD ?= .

CPPFLAGS += -MMD -MP
CXXFLAGS += -std=gnu++17 -O3 -fdiagnostics-color=always
CXXFLAGS += -Werror -Wall -Wextra -pedantic -Wshadow -Wmissing-declarations \
	-Wredundant-decls -Wconversion
LDLIBS := -lncurses
ifeq ($(DEBUG),y)
	CPPFLAGS += -DDEBUG
	CXXFLAGS += -O0 -g3
endif

.PHONY: all
all: exec

SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.hpp)
OBJECTS := $(SOURCES:%.cpp=%.o)
OBJECTS := $(addprefix $(BUILD)/, $(OBJECTS))
DEPS := $(SOURCES:%.cpp=%.d)
DEPS := $(addprefix $(BUILD)/, $(DEPS))
EXEC := $(BUILD)/chess
$(EXEC) $(OBJECTS): | $(BUILD)

.SUFFIXES:

-include $(DEPS)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD):
	@mkdir -p $(BUILD)

tags:
	ctags --exclude Makefile .

compile_commands.json: $(SOURCES) $(HEADERS) Makefile
	make clean
	$(RM) $@
	bear -- make all -j5 DEBUG=y

.PHONY: exec clean

exec: $(EXEC)

clean:
	@-$(RM) $(OBJECTS) $(DEPS) $(EXEC) vgcore.* perf.* *.plist
	@rmdir $(BUILD) 1>/dev/null 2>&1 || true
