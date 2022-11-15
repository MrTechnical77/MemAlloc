input = memAlloc
output = memAlloc

all:
	g++ -o $(output) $(input).cpp

clean:
	rm $(output)
	rm output.txt
	rm *.o
	

test:
	g++ -o $(output) $(input).cpp