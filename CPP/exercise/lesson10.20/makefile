.PHONY:all
all:mykill demo

demo:demo.cc
	g++ -o $@ $^ -std=c++11
mykill:mykill.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf mykill demo