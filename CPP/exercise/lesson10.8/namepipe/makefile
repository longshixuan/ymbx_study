.PHONY:all
all:pprocess cprocess
pprocess:pprocess.cc
	g++ -o $@ $^ -std=c++11
cprocess:cprocess.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf pprocess cprocess namepipe
