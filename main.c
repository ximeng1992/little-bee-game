#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "config.h"

#define ROW 20
#define COL 60
#define STEP 1

#define msleep(x) usleep(x*1000)
#define random(x) (rand()%x)

pthread_t ntid;

typedef struct
{
    void (*entry)(int, int, int, int); /* 函数入口*/
    int arg[4]; /* 参数*/
}FUNC;

void *start(void *arg)
{
 
    FUNC *func = (FUNC *)arg;
 
    (*(func->entry))(func->arg[0], func->arg[1], func->arg[2], func->arg[3]);

    return NULL;
}

struct block 
{
	int W ;   //方块的宽度
	int H ;   //方块的高度
	int row ; //方块的起始坐标row和col
 	int col ; 
   	int drow ; //方块的移动方向drow和dcol
    int dcol ; 
   	void (*show)(struct block block) ;  //方块显示
    void (*clear)(struct block block); //方块清除
    void (*move)(struct block *block, int direction) ;  //方块移动
};

struct block tmp;

void show_background(void);
void show_block(struct block block);
void clear_block(struct block block);
void move_block(struct block *block, int direction);
void move_ball(struct block *block, int direction);
void * block_move(int b_W, int b_H, int b_row, int b_col);

int main(void)
{
	int err;
	char c;
	int i;

	struct block b1 = {5, 1, ROW*4/5, COL/2, 1, 1, 
					show_block, clear_block, move_block};
	system("clear");
	show_background();
	printf("\033[?25l");
	b1.show(b1);

	/* 构造函数集合的结构体 */
    FUNC function = {0};
 
    function.entry = block_move;
 
	function.arg[0] = i;

	tmp = b1;	

	err = pthread_create(&ntid, NULL, start,(void *)&function);
	if (err != 0)
		printf("can't create thread\n");
	
	while (1) {
		c = getch();

		switch(c) {
		case 'q':
			system("clear");
			return 0;
		case 'd':
			b1.clear(b1);
			b1.move(&b1, 1);
			b1.show(b1);
			tmp = b1;
			break;
		case 'a':
			b1.clear(b1);
			b1.move(&b1, 2);
			b1.show(b1);
			tmp = b1;
			break;
		case 's':
			b1.clear(b1);
			b1.move(&b1, 3);
			b1.show(b1);
			tmp = b1;
			break;
		case 'w':
			b1.clear(b1);
			b1.move(&b1, 4);
			b1.show(b1);
			tmp = b1;
			break;
		default:
			break;
		}
	}

	return 0;
}

void * 
block_move(int b_W, int b_H, int b_row, int b_col)
{
	int i;

	struct block b_ball = {2, 1,  ROW*4/5-1, COL/2+2, 1, 1, 
					show_block, clear_block, move_ball};

	b_ball.show(b_ball);

	while (1) {
		b_ball.show(b_ball);
		msleep(200);
		b_ball.clear(b_ball);
		b_ball.move(&b_ball, 0);		
	}
}

//显示方块
void show_block(struct block block)
{
	int i,j;
	for (i = 0; i < block.H; i++) {
		for (j = 0; j < block.W; j ++)
			printf("\033[%d;%dH\033[43m \033[0m", 
				block.row+i, block.col+j);
	}
	fflush(stdout);
}

//清除方块
void clear_block(struct block block)
{
	int i,j;
	for (i = 0; i < block.H; i++) {
		for (j = 0; j < block.W; j ++)
			printf("\033[%d;%dH \033[0m", 
				block.row+i, block.col+j);
	}
	fflush(stdout);
}
//移动方球
void move_ball(struct block *block, int direction)
{
//	printf("======>block.row * block.col = %d x %d", block->row, block->col);
//	printf("======>tmp.row * tmp.col = %d x %d", tmp.row, tmp.col);
	
	if (block->row < 3 || block->row > ROW-block->H-1)
		block->drow = -block->drow ; 
	if (block->col < 3 || block->col > COL-block->W-1)
        block->dcol = -block->dcol ; 

	/* 碰到挡板 */
	if (block->row >= tmp.row-tmp.H && block->col < tmp.col+tmp.W && block->col > tmp.col) {
		block->drow = -block->drow ;
	}

	block->row += block->drow ; 
	block->col += block->dcol ; 
}
//移动挡板
void move_block(struct block *block, int direction)
{
	int step_row, step_col;

	switch (direction) {
	case 1:				/* East */
		step_row = 0;
		step_col = STEP;
		break;
	case 2:				/* west */
		step_row = 0;
		step_col = -STEP;
		break;
	case 3:				/* south */		
		step_row = STEP;
		step_col = 0;
		break;
	case 4:				/* north */
		step_row = -STEP;
		step_col = 0;
		break;
	case 5:				/* East-North */
		step_row = STEP;
		step_col = STEP;
		break;
	case 6:				/* West-South*/
		step_row = -STEP;
		step_col = -STEP;
		break;
	case 7:				/* East-South */		
		step_row = STEP;
		step_col = -STEP;
		break;
	case 8:				/* West-North */
		step_row = -STEP;
		step_col = STEP;
		break;
	default:
		step_row = 0;
		step_col = 0;
		break;
	}
	
	if ((block->row+step_row) < 2 || (block->row+step_row) > ROW-block->H) {
		step_row = 0;
		step_col = 0;
	}
	if ((block->col+step_col) < 2 || (block->col+step_col) > COL-block->W) {
		step_row = 0;
		step_col = 0;
	}
//	block->row += block->drow;
//	block->col += block->dcol;

	block->row += step_row;
	block->col += step_col;
}

//显示背景
void show_background(void)
{
	int i,j;
	
	for (i = 1; i < ROW; i++) {
		printf("\033[%d;1H\033[43m \033[0m", i);
		printf("\033[%d;%dH\033[43m \033[0m", i, COL);
	}
	
	for (i = 1; i <= COL; i++) {
		printf("\033[1;%dH\033[43m \033[0m", i);
		printf("\033[%d;%dH\033[43m \033[0m", ROW, i);
	}
}


