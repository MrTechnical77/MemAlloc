input = memAlloc
output = memAlloc

all:
	g++ -o $(output) $(input).cpp

clean:
	rm $(output)
	rm *.o

test:
	g++ -std=c99 -o $(output) $(input).cpp