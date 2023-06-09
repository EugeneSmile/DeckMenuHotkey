# This is the default target, which will be built when 
# you invoke make
.PHONY: all
all: deckmenuhotkey

# This rule tells make how to build hello from hello.cpp
deckmenuhotkey:
	mkdir -p ./out
	g++ -o ./out/deckmenuhotkey ./src/main.cc ./src/Controller.h ./src/Controller.cc ./src/Controller_XBox.h ./src/Controller_XBox.cc

# This rule tells make to delete hello and hello.o
.PHONY: clean 
clean:
	rm -f deckmenuhotkey