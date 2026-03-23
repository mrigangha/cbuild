SOURCE=src/**.c
TARGET=x86_64-w64-windows-gnu
FLAGS=-g
OUTPUT=a.exe


build:
	clang $(FLAGS) --target=$(TARGET) $(SOURCE) -o $(OUTPUT)

debug:
	lldb $(OUTPUT)

run:
	./$(OUTPUT)
