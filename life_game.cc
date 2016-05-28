/****************************************************************
    2次元、30×30のライフゲーム

    フィールドの上下左右はつながっている(ドラクエふう)
*****************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int field[32][32];        /*0-死 1-生*/
int next_field[32][32];   
//int old_field[32][32]; いらない疑惑
//char command,line[10];

int get_random(int min,int max); /*minからmaxまでの乱数を返す関数*/
int get_sum(int x,int y); /*周囲8セルの合計を返す関数*/
void init_field(int init_living_pct); /*フィールドの初期化*/
void print_field(void); /*フィールドの初期化*/
void copy_border_cells(void); /*左右と上下のデータをコピー*/
int count_changes(void); /*変わったセルの数*/
int sweep(void); /*生き死にを変える*/
void update(void); /*バッファをコピー*/

int get_random(int min,int max) /*minからmaxまでの乱数を返す関数*/
{
	static bool flag;
	if(flag){
		srand( (unsigned int) time (NULL) );
		flag = false;
	}
	
	return min + (int)( rand()*(max - min + 1.0)/(1.0 + RAND_MAX) );
}

int get_sum(int x,int y)/*周囲8セルの合計を返す関数*/
{
	return field[x-1][y-1] + field[x-1][y] + field[x-1][y+1]
		+field[x][y-1] + field[x][y+1] 
		+ field[x+1][y-1] + field[x+1][y] + field[x+1][y+1];
}

void init_field(int init_living_pct)
{
	for(int i=1;i<31;++i){
		for(int ii=1;ii<31;++ii){
			if(get_random(1,100) <= init_living_pct)
				field[i][ii] = 1;
			else
				field[i][ii] = 0;
		}
	}
	copy_border_cells();
}

void print_field(int generation)
{
	system("clear");
	for(int i=1;i<31;++i){
		for(int ii=1;ii<31;++ii){
			if(field[ii][i] == 1)
				cout << " O";
			else
				cout << "  ";
		}
		cout << endl;
	} 
	cout << generation << "世代" << endl;
}

void copy_border_cells(void)
{
	for(int i=1;i<31;++i){
		field[0][i]=field[30][i];
		field[31][i]=field[1][i];

		field[i][0]=field[i][30];
		field[i][31]=field[i][1];
	}
	field[0][0]=field[30][30];
	field[0][31]=field[30][1];
	field[31][0]=field[1][30];
	field[31][31]=field[1][1];
}

int count_changes(void)
{
	int c = 0;
	for(int i=1;i<31;++i)
		for(int ii=1;ii<31;++ii)
			//if(old_field[i][ii] != next_field[i][ii])
			if(field[i][ii] != next_field[i][ii])
				++c;

	return c;
}

int sweep(void)
{
	for(int i=1;i<31;++i){
		for(int ii=1;ii<31;++ii){
			if(field[i][ii] == 0){/*セルが死んでいる場合*/
				if( get_sum(i,ii) == 3)
    					next_field[i][ii] = 1;
				else    	       
					next_field[i][ii] = 0;
			}else{ /*セルが生きている場合*/
				if( get_sum(i,ii) == 2 || get_sum(i,ii) == 3)
					next_field[i][ii] = 1;
				else
					next_field[i][ii] = 0;
			}				
		}			
	}

	return count_changes();
}

void update(void)
{
	/*fieldをnext_fieldで更新*/
	for(int i=1;i<31;++i)
		for(int ii=1;ii<31;++ii)
			field[i][ii] = next_field[i][ii];

	copy_border_cells();
}

int main(int argc, char const* argv[])
{
	int p; /*初期配置における生きたセルのパーセンテージ*/
	cout << "生きたセルは何パーセント? (整数で):" << endl;
	cin >> p;
	init_field(p);

	int generation = 0;
	print_field(generation);

	while(sweep()){
		++generation;	

		update();
		print_field(generation);
		usleep(200*1000);

/* ここら辺、来ないような気がするのでコメントアウト（by 上田）
		change = count_changes();

		if (change == 0){
			++cnt;
			cout << "loop" << endl;
		}
		if (cnt >= 10)
			break;

		change = 0;
*/
	}
	exit(0);
}
