default:
	mkdir -p build
	bison -d grammar.y -o build/grammar.cc
	flex -o build/lex.cc lex.l
	g++ -g build/lex.cc build/grammar.cc