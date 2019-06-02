#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t readflag;
sem_t printflag;

int read_cycle = 0;
int cal_cycle = 0;
int print_cycle = 0;
char input[3], input1[3],input2[3],input3[3];
int MAX_PROB_NUM = 100000;
int puzzle[100000][9][9];
int i;

int isAvailable(int puzzle[][9], int row, int col, int num){
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int x;

    for(x=0; x<9; ++x){
        if (puzzle[row][x] == num) return 0;
        if (puzzle[x][col] == num) return 0;
        if (puzzle[rowStart + (x%3)][colStart + (x/3)] == num) return 0;
    }
    return 1;
}

int solveSudoku(int puzzle[][9], int row, int col){
    int x;
    if(row<9 && col<9){
        if(puzzle[row][col]){
            if((col+1)<9) return solveSudoku(puzzle, row, col+1);
            else if((row+1)<9) return solveSudoku(puzzle, row+1, 0);
            else return 1;
        }
        else{
            for(x=0; x<9; ++x){
                if(isAvailable(puzzle, row, col, x+1)){
                    puzzle[row][col] = x+1;
             
                    if(solveSudoku(puzzle, row, col)) return 1;
                    else puzzle[row][col] = 0;
                }
            }
        }
        return 0;
    }
    else return 1;
}

void init()
{
    sem_init(&readflag, 0, 0); 
	sem_init(&printflag, 0, 0);
}

void *producer(void *arg){
	int n = *((int *) arg);
	while(read_cycle < n){
		
		for(i=0; i<9; i++){
	    	scanf("%s %s %s", &input1, &input2, &input3);
	    	puzzle[read_cycle][i][0] = input1[0] - '0';
	    	puzzle[read_cycle][i][1] = input1[1] - '0';
	    	puzzle[read_cycle][i][2] = input1[2] - '0';
	    	puzzle[read_cycle][i][3] = input2[0] - '0';
	    	puzzle[read_cycle][i][4] = input2[1] - '0';
	    	puzzle[read_cycle][i][5] = input2[2] - '0';
	    	puzzle[read_cycle][i][6] = input3[0] - '0';
	    	puzzle[read_cycle][i][7] = input3[1] - '0';
	    	puzzle[read_cycle][i][8] = input3[2] - '0';
		}
		sem_post(&readflag);
		read_cycle++;
	}
}

void *consumer(void *arg){
	int n = *((int *) arg);
	while(cal_cycle < n){
		sem_wait(&readflag);
		solveSudoku(puzzle[cal_cycle], 0, 0);
		sem_post(&printflag);
		cal_cycle++;
	}
}

void *printer(void *arg){
	int n = *((int *) arg);
	while(print_cycle < n){
		sem_wait(&printflag);
		printf("%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n%d%d%d %d%d%d %d%d%d\n\n", puzzle[print_cycle][0][0],puzzle[print_cycle][0][1],puzzle[print_cycle][0][2],puzzle[print_cycle][0][3],puzzle[print_cycle][0][4],puzzle[print_cycle][0][5],puzzle[print_cycle][0][6],puzzle[print_cycle][0][7],puzzle[print_cycle][0][8],puzzle[print_cycle][1][0],puzzle[print_cycle][1][1],puzzle[print_cycle][1][2],puzzle[print_cycle][1][3],puzzle[print_cycle][1][4],puzzle[print_cycle][1][5],puzzle[print_cycle][1][6],puzzle[print_cycle][1][7],puzzle[print_cycle][1][8],puzzle[print_cycle][2][0],puzzle[print_cycle][2][1],puzzle[print_cycle][2][2],puzzle[print_cycle][2][3],puzzle[print_cycle][2][4],puzzle[print_cycle][2][5],puzzle[print_cycle][2][6],puzzle[print_cycle][2][7],puzzle[print_cycle][2][8],puzzle[print_cycle][3][0],puzzle[print_cycle][3][1],puzzle[print_cycle][3][2],puzzle[print_cycle][3][3],puzzle[print_cycle][3][4],puzzle[print_cycle][3][5],puzzle[print_cycle][3][6],puzzle[print_cycle][3][7],puzzle[print_cycle][3][8],puzzle[print_cycle][4][0],puzzle[print_cycle][4][1],puzzle[print_cycle][4][2],puzzle[print_cycle][4][3],puzzle[print_cycle][4][4],puzzle[print_cycle][4][5],puzzle[print_cycle][4][6],puzzle[print_cycle][4][7],puzzle[print_cycle][4][8],puzzle[print_cycle][5][0],puzzle[print_cycle][5][1],puzzle[print_cycle][5][2],puzzle[print_cycle][5][3],puzzle[print_cycle][5][4],puzzle[print_cycle][5][5],puzzle[print_cycle][5][6],puzzle[print_cycle][5][7],puzzle[print_cycle][5][8],puzzle[print_cycle][6][0],puzzle[print_cycle][6][1],puzzle[print_cycle][6][2],puzzle[print_cycle][6][3],puzzle[print_cycle][6][4],puzzle[print_cycle][6][5],puzzle[print_cycle][6][6],puzzle[print_cycle][6][7],puzzle[print_cycle][6][8],puzzle[print_cycle][7][0],puzzle[print_cycle][7][1],puzzle[print_cycle][7][2],puzzle[print_cycle][7][3],puzzle[print_cycle][7][4],puzzle[print_cycle][7][5],puzzle[print_cycle][7][6],puzzle[print_cycle][7][7],puzzle[print_cycle][7][8],puzzle[print_cycle][8][0],puzzle[print_cycle][8][1],puzzle[print_cycle][8][2],puzzle[print_cycle][8][3],puzzle[print_cycle][8][4],puzzle[print_cycle][8][5],puzzle[print_cycle][8][6],puzzle[print_cycle][8][7],puzzle[print_cycle][8][8]);
		print_cycle++;
	}
}

int main()
{
	pthread_t pthread, cthread, fthread;

	scanf("%s", &input);
	int prob_num = atoi(input);
    init();

	while(prob_num > MAX_PROB_NUM){
		pthread_create(&pthread, NULL, producer, &MAX_PROB_NUM);
		pthread_create(&cthread, NULL, consumer, &MAX_PROB_NUM);
		pthread_create(&fthread, NULL, printer, &MAX_PROB_NUM);
		pthread_join(pthread, NULL);
		pthread_join(cthread, NULL);
		pthread_join(fthread, NULL);
		prob_num -= MAX_PROB_NUM;
		read_cycle = 0;
		cal_cycle = 0;
		print_cycle = 0;
	}
	pthread_create(&pthread, NULL, producer, &prob_num);
	pthread_create(&cthread, NULL, consumer, &prob_num);
	pthread_create(&fthread, NULL, printer, &prob_num);
	pthread_join(pthread, NULL);
	pthread_join(cthread, NULL);
	pthread_join(fthread, NULL);

    return 0;
}
