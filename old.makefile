default:
	rm -rf build
	mkdir -p build
	bison -d grammar.y -o build/grammar.cc
	flex -o build/lex.cc --header-file="build/lex.h" lex.l
	g++ -o build/app build/lex.cc build/grammar.cc main.cpp