CC= gcc
CFLAGS= -Wall -Waddress
TARGET= chip8

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)

#The Dependency Check: $(TARGET): main.c tells make to look at the timestamp of main.c. If main.c hasn't been modified since the last time you compiled, make is smart enough to skip recompiling, saving you time.
#target: dependencies
#	command_to_run   <-- MUST start with a physical Tab character
# if .PHONY is used then the compiler will nnot be confused even if a file with the same name like "clean" added
#.PHONY: target_name