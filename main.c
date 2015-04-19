#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define program_x (80)
#define program_y (25)

typedef int stack_data_type;

typedef struct{
	int x;
	int y;
} CURSOR;

char program[program_x][program_y];

const CURSOR direction[4] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1}
};

CURSOR cursor = {0, 0};
int cursor_direction = 0;

stack_data_type *stack;
int stack_size = 0;

void init_interpreter(void){
	for(int y = 0; y < program_y; y++) for(int x = 0; x < program_x; x++) program[x][y] = '\0';
}

void exit_interpreter(void){
	free(stack);
}

void get_program(void){
	for(int y = 0; y < program_y; y++){
		char tmp_line[program_x];
		if(fgets(tmp_line, program_x * sizeof (char), stdin) == NULL)return;
		for(int x = 0; x < program_x; x++){
			if(tmp_line[x] == '\0')return;
			if((tmp_line[x] == '\n') || (tmp_line[x] == '\r'))break;
			program[x][y] = tmp_line[x];
		}
	}
}

void print_program(int cursor_x, int cursor_y){
	for(int y = 0; y < program_y; y++){
		for(int x = 0; x < program_x; x++){
			if((cursor_x == x) && (cursor_y == y))putchar('&');
			else if(program[x][y] == '\0')putchar(' ');
			else putchar(program[x][y]);
		}
		puts("|");
	}
}

void print_stack(void){
	for(int i = stack_size; i != 0; i--){
		if(('!' <= stack[i - 1])&& (stack[i - 1] <= '~'))
			printf("%6d\t%c\r\n", stack[i - 1], stack[i - 1]);
		else printf("%6d\r\n", stack[i - 1]);
	}
}

void move_cursor(void){
	cursor.x += direction[cursor_direction].x;
	cursor.y += direction[cursor_direction].y;
	if(cursor.x == program_x)cursor.x = 0;
	else if(cursor.x == -1)cursor.x = program_x - 1;
	if(cursor.y == program_y)cursor.y = 0;
	else if(cursor.y == -1)cursor.y = program_y - 1;
}

void push(stack_data_type value){
	stack = realloc(stack, ++stack_size * sizeof (stack_data_type));
	stack[stack_size - 1] = value;
}

stack_data_type pop(void){
	int tmp = stack[stack_size - 1];
	stack_size--;
	stack = realloc(stack, stack_size * sizeof (stack_data_type));
	return tmp;
}

void interpreter(void){

	srand((unsigned)time(NULL));

	while(1){
		int x, y, v;
		switch(program[cursor.x][cursor.y]){
			case '<': //実行の向きを左にする
				cursor_direction = 2;
				break;
			case '>': //実行の向きを右にする
				cursor_direction = 0;
				break;
			case '^': //実行の向きを上にする
				cursor_direction = 3;
				break;
			case 'v': //実行の向きを下にする
				cursor_direction = 1;
				break;
			case '?': //実行の向きを上下左右のいずれかにランダムで変更する。
				cursor_direction = rand() % 4;
				break;

			case '_': //スタックをポップして、その値が0ならば実行の向きを右に、そうでなければ左にする
				if(pop() == 0)cursor_direction = 0;
				else cursor_direction = 2;
				break;
			case '|': //スタックをポップして、その値が0ならば実行の向きを下に、そうでなければ上にする
				if(pop() == 0)cursor_direction = 1;
				else cursor_direction = 3;
				break;

				//数値をスタックにプッシュする
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				push((int)(program[cursor.x][cursor.y] - '0'));
				break;
			case '"': //次に"が出現するまで、文字の命令を実行する代わりに文字のASCIIコードをスタックにプッシュする
				while(1){
					move_cursor();
					if(program[cursor.x][cursor.y] != '"')push((stack_data_type)program[cursor.x][cursor.y]);
					else break;
				}
				break;

			case '&': //ユーザに数値を入力させ、その値をスタックにプッシュする
				break; //未実装というかあえて実装しない
			case '~': //ユーザに1文字入力させ、その文字のASCIIコードをスタックにプッシュする
				break; //未実装というかあえて実装しない
			case '.': //スタックをポップして、その値を十進表示し、続けて半角スペースを出力する
				printf("%d ", pop());
				break;
			case ',': //スタックをポップして、その値をASCIIコードに持つ文字を表示する
				putchar((char)pop());
				break;

			case '+': //スタックから y, x をポップして、x + y の値をプッシュする
				y = pop();
				x = pop();
				push(x + y);
				break;
			case '-': //スタックから y, x をポップして、x - y の値をプッシュする
				y = pop();
				x = pop();
				push(x - y);
				break;
			case '*': //スタックから y, x をポップして、x × y の値をプッシュする
				y = pop();
				x = pop();
				push(x * y);
				break;
			case '/': //スタックから y, x をポップして、x / y の値をプッシュする
				y = pop();
				x = pop();
				push(x / y);
				break;
			case '%': //スタックから y, x をポップして、x を y で割ったときの余りをプッシュする
				y = pop();
				x = pop();
				push(x % y);
				break;
			case '`': //スタックから y, x をポップして、x > y ならば 1 を、そうでなければ 0 をプッシュする
				y = pop();
				x = pop();
				push(x > y);
				break;
			case '!': //スタックをポップして、その値が 0 ならば 1 を、そうでなければ 0 をプッシュする
				push(pop() == 0);
				break;

			case ':': //スタックをポップして、その値を2回プッシュする
				x = pop();
				push(x);
				push(x);
				break;
			case '\\': //スタックから y, x をポップして y をプッシュし、その後 x をプッシュする
				y = pop();
				x = pop();
				push(y);
				push(x);
				break;
			case '$': //スタックをポップして、その値を使用しない
				pop();
				break;

			case 'g': //スタックから y, x をポップして、プログラムの y 行 x 桁目の文字のASCIIコードをスタックにプッシュする
				y = pop();
				x = pop();
				push((stack_data_type)(program[x][y]));
				break;
			case 'p': //スタックから y, x, v をポップして、プログラムの y 行 x 桁目を、v をASCIIコードに持つ文字に書き換える
				y = pop();
				x = pop();
				v = pop();
				program[x][y] = (char)v;
				break;

			case '#': //次の文字が表す命令を実行しない
				move_cursor();
				break;

			case '@': //プログラムの実行を停止する
				return;

			case ' ': //何もしない
			default:
				break;

		}

		move_cursor();

	}

}

int main(int argc, char *argv[]){

	init_interpreter();

	get_program();
	print_program(-1, -1);

	interpreter();

	exit_interpreter();

	return (EXIT_SUCCESS);

}
