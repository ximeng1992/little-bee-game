#include <stdio.h>
#include <unistd.h>

#define ROW 20
#define COL 60

#define msleep(x) usleep(x*1000)

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
    void (*move)(struct block *block) ;  //方块移动
};

void show_background(void);
void show_block(struct block block);
void clear_block(struct block block);
void move_block(struct block *block);

int main(void)
{
	char c;
	struct block b1 = {4, 2, 5, 5, 1, 1, 
					show_block, clear_block, move_block};
	system("clear");
	show_background();
	printf("\033[?25l");
	b1.show(b1);
	
	while (1) {
		c = getchar();

		switch(c) {
		case 'q':
			return 0;
		}
//		b1.show(b1);
//		msleep(100);
//		b1.clear(b1);
//		b1.move(&b1);	
	}

	return 0;
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

//移动方块
void move_block(struct block *block)
{
	if (block->row < 3 || block->row > ROW-block->H-1)
		block->drow = -block->drow;
	if (block->col < 3 || block->col > COL-block->H-1)
		block->dcol = -block->dcol;

	block->row += block->drow;
	block->col += block->dcol;
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


