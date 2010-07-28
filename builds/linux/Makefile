BIN_DIR = ../../bin

PLATFORM := SDL
# PLATFORM := Qt
# PLATFORM := WIN32

RGSS_TYPE := XP
# RGSS_TYPE := VX

TARGET = ARGSS_$(RGSS_TYPE)

SOIL_OJBS = $(addprefix src/SOIL/projects/makefile/obj/, SOIL.o image_DXT.o image_helper.o stb_image_aug.o)

SRC_DIR = ../../src
SRC_CXX = $(wildcard $(SRC_DIR)/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/argss/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/argss/$(RGSS_TYPE)/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/$(PLATFORM)/*.cxx)
SRC_CXX += $(wildcard $(SRC_DIR)/$(RGSS_TYPE)/*.cxx)
OBJS_DIR = ./objs
OBJS_CXX = $(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_CXX:.cxx=.o))
OBJS = $(OBJS_CXX) $(SOIL_OJBS) $(OBJS_DIR)/malloc_wrapper.o $(OBJS_DIR)/rubybind.o

OBJS_DIR_ALL = $(OBJS_DIR) $(OBJS_DIR)/$(RGSS_TYPE) $(OBJS_DIR)/argss $(OBJS_DIR)/argss/$(RGSS_TYPE) $(OBJS_DIR)/$(PLATFORM)

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
	-I../../dependencies/rubybind \
	-Isrc/SOIL/src \
	-Isrc \
	-I$(SRC_DIR) \
	-I/usr/include/wine/windows \
	-D_FORTIFY_SOURCE=1 -fstack-protector-all \
	-DARGSS_$(PLATFORM) \

CXXFLAGS = $(CFLAGS)
LDFLAGS = --enable-gold

ifeq ($(RGSS_TYPE),XP)
CFLAGS += -DRPGMAKER=1
else ifeq ($(RGSS_TYPE),VX)
CFLAGS += -DRPGMAKER=2
endif

LD = $(CXX)

LIBS += \
	-lSDL_mixer \
	$(shell sdl-config --libs) \
	$(shell pkg-config gl --libs) \
	$(shell freetype-config --libs) \
	-lboost_iostreams \

# ruby version things

RUBY_VERSION = 1.9.2
# RUBY_VERSION = 1.9.1
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
else ifeq ($(RUBY_VERSION),1.9.2)
RUBY_VERSION = 1.9.1
RUBY_BUILD = i686-linux
CFLAGS += \
	-I/usr/local/include/ruby-$(RUBY_VERSION) \
	-I/usr/local/include/ruby-$(RUBY_VERSION)/$(RUBY_BUILD)
LIBS += -L/usr/local/lib -lruby -lcrypt
else
CFLAGS += \
	-I/usr/include/ruby-$(RUBY_VERSION) \
	-I/usr/include/ruby-$(RUBY_VERSION)/$(RUBY_BUILD)
LIBS += -lruby-$(RUBY_VERSION)
endif

ifeq ($(PLATFORM),Qt)
LIBS += -lQtGui -lQtCore -lQtOpenGL
CXXFLAGS += -I/usr/include/qt4
else ifeq ($(PLATFORM),WIN32)
CFLAGS += -DNOMINMAX
endif

CFLAGS += -include /usr/include/time.h


all : $(TARGET)

clean :
	$(RM) -f $(TARGET) $(OBJS) $(BIN_DIR)/$(TARGET)
	make -C src/SOIL/projects/makefile/ clean

rebuild : clean all

debug : $(TARGET)
	cd $(BIN_DIR); gdb ./$(TARGET)

run : $(TARGET)
	cd $(BIN_DIR); ./$(TARGET)

profile : run
	gprof $(BIN_DIR)/$(TARGET) gmon.out > $(TARGET).profile

# it's not $(SRC_DIR)
$(OBJS_DIR)/rgssad.o : src/rgssad.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# nedmalloc wrapper
$(OBJS_DIR)/malloc_wrapper.o : ../../dependencies/malloc/wrapper.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/rubybind.o : ../../dependencies/rubybind/src/rubybind.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o : $(SRC_DIR)/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET) : $(OBJS_DIR_ALL) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS)
	cp $(TARGET) $(BIN_DIR)/$(TARGET)

$(SOIL_OJBS) :
	make -C src/SOIL/projects/makefile/

$(OBJS_DIR_ALL) :
	mkdir -p $(OBJS_DIR_ALL)
