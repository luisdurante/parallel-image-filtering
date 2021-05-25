OBJS	= main.o Image.o
SOURCE	= main.cpp Image.cpp
HEADER	= Image.h stb_image.h stb_image_write.h
OUT	= output
CC	 = g++
FLAGS	 = -g -c -Wall -fopenmp
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g -fopenmp $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

Image.o: Image.cpp
	$(CC) $(FLAGS) Image.cpp 


clean:
	rm -f $(OBJS) $(OUT)