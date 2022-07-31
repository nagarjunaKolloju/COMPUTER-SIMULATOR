
mycompiler: y.tab.c lex.yy.c 
	gcc  y.tab.c -ll -w -o mycompiler
lex.yy.c: parse.l
	lex parse.l
y.tab.c: parse.y
	yacc  parse.y
clean:
	rm -f mycompiler myparser  y.tab.c lex.yy.c y.tab.h y.output y.dot a.out
