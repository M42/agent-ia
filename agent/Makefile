all: agent memoria.pdf

agent: main.cpp agent.cpp environment.cpp agent.cpp evaluator.cpp random_num_gen.cpp agent.h
	g++ main.cpp environment.cpp agent.cpp evaluator.cpp random_num_gen.cpp -o agent -fpermissive

memoria.md: memoria.ipynb
	ipython nbconvert --to markdown $<

memoria.pdf: memoria.md
	pandoc memoria.md -o memoria.pdf

clean:
	rm -f agent memoria.pdf
