TARGET = ARGSS

BIN_DIR = ../../bin

PLATFORM = SDL
# PLATFORM = Qt
# PLATFORM = WIN32

SRC_DIR = ../../src
SRC_CXX = $(wildcard $(SRC_DIR)/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/argss/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/$(PLATFORM)/*.cxx)
OBJS_DIR = ./objs
OBJS_CXX = $(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_CXX:.cxx=.o))
OBJS = $(OBJS_CXX) $(wildcard src/SOIL/projects/makefile/obj/*.o)

LINUX_SRC_DIR = src
LINUX_SRC_C = $(wildcard $(LINUX_SRC_DIR)/*.c)
OBJS += $(subst $(LINUX_SRC_DIR), $(OBJS_DIR), $(LINUX_SRC_C:.c=.o))

CFLAGS = \
	-Wall -Os -pg -g3 \
	-I$(SRC_DIR) \
	$(shell sdl-config --cflags) \
	$(shell pkg-config gl --cflags) \
	$(shell freetype-config --cflags) \
	-I../../dependencies \
	-Isrc/SOIL/src \
	-Isrc \
	-I$(SRC_DIR) \
	-I/usr/include/wine/windows \
	-D_FORTIFY_SOURCE=1 -fstack-protector-all \
	-DARGSS_$(PLATFORM) \

CXXFLAGS = $(CFLAGS)
LDFLAGS = --enable-gold

LD = $(CXX)

LIBS = \
	$(shell sdl-config --libs) \
	$(shell pkg-config gl --libs) \
	$(shell freetype-config --libs) \
	-lSDL_mixer \

# ruby version things

# RUBY_VERSION = 1.9.2
RUBY_VERSION = 1.9.1
# RUBY_VERSION = 1.9.0
# RUBY_VERSION = 1.8

RUBY_BUILD = i486-linux

ifeq ($(RUBY_VERSION),1.8)
CFLAGS += -I/usr/lib/ruby/$(RUBY_VERSION)/$(RUBY_BUILD) -DRUBY_VERSION_1_8
LIBS += -lruby$(RUBY_VERSION)
else ifeq ($(RUBY_VERSION),1.9.0)
CFLAGS += \
	-I/usr/include/ruby-$(RUBY_VERSION) \
	-I/usr/include/ruby-$(RUBY_VERSION)/$(RUBY_BUILD)
LIBS += -lruby1.9
else
CFLAGS += \
	-I/usr/include/ruby-$(RUBY_VERSION) \
	-I/usr/include/ruby-$(RUBY_VERSION)/$(RUBY_BUILD)
LIBS += -lruby-$(RUBY_VERSION)
endif

ifeq ($(PLATFORM),Qt)
LIBS += -lQtGui -lQtCore -lQtOpenGL
CXXFLAGS += -I/usr/include/qt4
endif

CFLAGS += -include /usr/include/time.h


all : $(TARGET)

clean :
	$(RM) -f $(TARGET) $(TARGET).* $(OBJS) $(BIN_DIR)/$(TARGET)
	make -C src/SOIL/projects/makefile/ clean

rebuild : clean all

debug : $(TARGET)
	cd $(BIN_DIR); gdb ./$(TARGET)

run : $(TARGET)
	cd $(BIN_DIR); ./$(TARGET)

profile : run
	gprof $(BIN_DIR)/$(TARGET) gmon.out > $(TARGET).profile

$(OBJS_DIR)/%.o : $(SRC_DIR)/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o : $(LINUX_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET) : make_objs_dir build_soil $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS)
	cp $(TARGET) $(BIN_DIR)/$(TARGET)

build_soil :
	make -C src/SOIL/projects/makefile/

make_objs_dir :
	mkdir -p objs
	mkdir -p objs/argss
	mkdir -p objs/$(PLATFORM)
