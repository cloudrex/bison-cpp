default:
	rm -rf bin
	mkdir -p bin
	bison -d grammar.y -o bin/grammar.cc
	flex -o bin/lex.cc --header-file="bin/lex.h" lex.l
	g++ -o bin/app bin/lex.cc bin/grammar.cc main.cpp `llvm-config --cxxflags --system-libs --ldflags --libs core`