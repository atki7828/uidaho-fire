make: Crypto.o
	g++ -g -shared -o libCrypto.so Crypto.o

Crypto.o : Crypto.cpp Crypto.h
	g++ -g -c -fPIC Crypto.cpp -o Crypto.o

Crypto.so : Crypto.o
	g++ -g -shared -o libCrypto.so Crypto.o

clean:
	rm *.o
	rm *.so
