CC = g++ 
CFLAGS = -c
DBGFLAGS = -g 
OPTFLAGS = -O2 

all : bin/Watermark
	@echo -n ""

# optimized version
bin/Watermark : Watermark_opt.o main_opt.o
			$(CC) $(OPTFLAGS) $^ -o $@
main_opt.o      : src/main.cpp src/Watermark.h
			$(CC) $(CFLAGS) $< -o $@
Watermark_opt.o : src/Watermark.cpp src/Watermark.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : bin/Watermark_dbg
	@echo -n ""

bin/Watermark_dbg : Watermark_dbg.o main_dbg.o
			$(CC) $(DBGFLAGS) $^ -o $@
main_dbg.o      : src/main.cpp
			$(CC) $(CFLAGS) $< -o $@
Watermark_dbg.o : src/Watermark.cpp src/Watermark.h
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

# clean all the .o and executable files
clean:
	rm -rf *.o bin/*
