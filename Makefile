SOURCE=src/**.c
TARGET=x86_64-w64-windows-gnu
FLAGS=-g -Wall
OUTPUT=output.exe
LIBS=-lpthread

build:
	clang $(FLAGS) --target=$(TARGET) $(SOURCE) -o $(OUTPUT) $(LIBS)

debug:
	lldb $(OUTPUT)

run:
	./$(OUTPUT)
