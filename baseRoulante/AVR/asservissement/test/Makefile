all:
	g++ -lcunit -o suite test/main.c
	
test: suite
	./suite
	rm temp.txt
	
clean:
	rm *.o temp.txt suite