flags := -g -MMD -MP
includes := -Ithirdparty -Iinclude -Ibuild/include
libraries := -Lthirdparty/enet/lib -lenet_32 -lws2_32 -lwinmm
binary := main.exe

all: $(binary)

sources := main.c $(shell find include -type f -name '*.c' 2>/dev/null)
objects := $(sources:%.c=build/%.o)

$(binary): $(objects)
	gcc $(objects) -o $@ $(libraries)

build/%.o: %.c $(pch_gch)
	@mkdir -p $(dir $@)
	gcc $(flags) $(includes) -c $< -o $@

-include $(objects:.o=.d)

.PHONY : all clean
clean :
	-rm -rf build $(binary)
