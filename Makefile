SOURCES = main.c parser.c

target: assembler

assembler: $(SOURCES)
	gcc -o assembler $(SOURCES)

clean:
	rm -f assembler *.o *.hack
