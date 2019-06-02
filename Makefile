fastSudoku : fastSudoku.c
	gcc -pthread -o fastSudoku fastSudoku.c -O2
clean :
	rm fastSudoku
