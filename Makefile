CXX=g++
#CPPFLAGS=-Wall -g -O2 
CPPFLAGS=-Wall -g -D_DEBUG -I.
LDFLAGS= -LSimpleNet -Llib-linux
# source code root dir
SOURCE_DIR=src/
FILES=$(shell find $(SOURCE_DIR) -name "*.cpp")
HEADERS=$(shell find $(SOURCE_DIR) -name "*.h")
LIBS= -lsimple_net -llinux_tool
DEPEND=SimpleNet/libsimple_net.a lib-linux/liblinux_tool.a
OBJECTS=$(FILES:.cpp=.o)
TARGET=portscan

all:_SimpleNet _lib-linux $(TARGET) 

# make the SimpleNet lib
_SimpleNet:
	make -C SimpleNet

_lib-linux:
	make -C lib-linux

# include all source file depends.
-include Makefile.deps

$(TARGET): Makefile.deps $(OBJECTS) $(DEPEND)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

Makefile.deps: $(FILES) $(HEADERS)
	makedepend -f "$(FILES)" -c "$(CXX)" -p "$(CPPFLAGS)" > Makefile.deps

clean:
	-rm -f $(OBJECTS) $(TARGET) Makefile.deps
