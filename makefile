
FLAGS := -std=c++11 -Wall -I../include -I../include/WIZ/desktop

objects := $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))
headers := $(wildcard include/WIZ/*.h);

../libs/libWIZ.a: $(objects)
	ar crf $@ $(objects)
	@echo library created successful

obj/%.o: %.cpp $(headers)
	@echo compiling $@
	c++ -c $< -o $@ $(FLAGS)