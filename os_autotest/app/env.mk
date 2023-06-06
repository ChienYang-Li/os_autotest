NAME     ?= $(shell basename "$$PWD")
BIN.TYPE ?= bin

WORKDIR := $(DIR_INTERMEDIATE)/$(NAME)

CC    := $(TOOLCHAIN_PREFIX)-gcc
CXX   := $(TOOLCHAIN_PREFIX)-g++
LD    := $(TOOLCHAIN_PREFIX)-g++
AR    := $(TOOLCHAIN_PREFIX)-ar
STRIP := $(TOOLCHAIN_PREFIX)-strip

SRC.C     ?= $(shell find . -name '*.c')
SRC.CXX   ?= $(shell find . -name '*.cxx')
SRC.CPP   ?= $(shell find . -name '*.cpp')

SRC.C.O   ?= $(patsubst %,$(WORKDIR)/%.o, $(SRC.C))
SRC.CXX.O ?= $(patsubst %,$(WORKDIR)/%.o, $(SRC.CXX))
SRC.CPP.O ?= $(patsubst %,$(WORKDIR)/%.o, $(SRC.CPP))

CFLAGS += -Os
CFLAGS += -Wall
CFLAGS += -I$(WORKDIR)
CFLAGS += -I..

CXXFLAGS += $(CFLAGS)
CXXFLAGS += -std=c++1z

LDFLAGS += -ldl -pthread -lrt

ARFLAGS := rcs

all: clean gch $(SRC.C.O) $(SRC.CXX.O) $(SRC.CPP.O) link/$(BIN.TYPE) install/$(BIN.TYPE)

clean: clean/$(BIN.TYPE)
	rm -rf $(WORKDIR)
clean/bin:
	rm -f $(DIR_INSTALL)/app/$(NAME)
clean/static-lib:
	rm -f $(DIR_INSTALL)/app/$(NAME).a

gch:
	mkdir -p $(WORKDIR)
	$(CXX) $(CXXFLAGS) -x c++-header $(NAME).h -o $(WORKDIR)/$(NAME).h.gch

$(WORKDIR)/%.c.o: %.c
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c -o $@ $<
$(WORKDIR)/%.cxx.o: %.cxx
	mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(WORKDIR)/%.cpp.o: %.cpp
	mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -c -o $@ $<

link/bin:
	$(LD) $(LDFLAGS) $(SRC.C.O) $(SRC.CXX.O) $(SRC.CPP.O) -Wl,--start-group $(LIBS) -Wl,--end-group -o $(WORKDIR)/$(NAME)
link/static-lib:
	$(AR) $(ARFLAGS) $(WORKDIR)/$(NAME).a $(SRC.C.O) $(SRC.CXX.O) $(SRC.CPP.O)

install/bin:
	mkdir -p $(DIR_INSTALL)/app
	cp $(WORKDIR)/$(NAME) $(DIR_INSTALL)/app

	cp $(WORKDIR)/$(NAME) $(DIR_NFS)
install/static-lib:
	mkdir -p $(DIR_INSTALL)/app
	cp $(WORKDIR)/$(NAME).a $(DIR_INSTALL)/app
