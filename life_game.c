/****************************************************************
    2次元、30×30のライフゲーム

    フィールドの上下左右はつながっている(ドラクエふう)
*****************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define P 54	/*初期配置での生きたセルの確率*/	

int field[32][32];        /*0-死 1-生*/
int next_field[32][32];   
int old_field[32][32];
 
int i,ii,a,t=0;
int change = 0;
int cnt = 0;
char command,line[10];

int GetRandom(int min,int max) /*minからmaxまでの乱数を返す関数*/
{
	static int flag;
	if(flag == 0)
	{
		srand( (unsigned int) time (NULL) );
		flag = 1;	
	}
	
	return min + (int)( rand()*(max - min + 1.0)/(1.0 + RAND_MAX) );
}

int GetSum(int x,int y)/*周囲8セルの合計を返す関数*/
{
	int sum;
	sum = field[x-1][y-1]+field[x-1][y]+field[x-1][y+1]+field[x][y-1]+field[x][y+1]+field[x+1][y-1]+field[x+1][y]+field[x+1][y+1];

	return sum;
}

int main(void)
{
	for(i=1;i<31;++i)
	{
		for(ii=1;ii<31;++ii)
		{
			a=GetRandom(1,100);
			field[i][ii] = (a <= P) ? 1:0;			
		}
	}

	/*以下を繰り返す*/
	while(1)
	{
		/*現在のfieldを画面に出力*/
		system("sleep 0.2");
		system("clear");
		for(i=1;i<31;++i)
		{
			for(ii=1;ii<31;++ii)
			{
				if(field[ii][i] == 1) printf("O ");

				else printf("  ");
			}
			printf("\n");
		} 
		printf("%d 世代 \n",t);

		/*fieldの上下左右を接続*/
		for(i=1;i<31;++i)
		{
			field[0][i]=field[30][i];
			field[31][i]=field[1][i];

			field[i][0]=field[i][30];
			field[i][31]=field[i][1];
		}
		field[0][0]=field[30][30];
		field[0][31]=field[30][1];
		field[31][0]=field[1][30];
		field[31][31]=field[1][1];
		
		/*fieldからold_fieldを決定*/
		if(t%2 == 0)
		{
			for(i=0;i<32;++i)
			{
				for(ii=0;ii<32;++ii)
				{
					old_field[i][ii] = field[i][ii];
				}
			}
		}

		/*fieldからnext_fieldを決定*/
		for(i=1;i<31;++i)
		{
			for(ii=1;ii<31;++ii)
			{
				switch(field[i][ii])
				{
					case 0:/*セルが死んでいる場合*/
					if( GetSum(i,ii) == 3)
					{
    						next_field[i][ii] = 1;
						++change;
					}
					else    	       
					{
						next_field[i][ii] = 0;
					}	
					break;
					
					case 1:/*セルが生きている場合*/
					if( GetSum(i,ii) == 2 || GetSum(i,ii) == 3)
					{
						next_field[i][ii] = 1;
					}
					else
					{
						next_field[i][ii] = 0;
						++change;	
					} 
					break;
				}				
			}			
		}

		/*fieldをnext_fieldで更新*/
		for(i=1;i<31;++i)
		{
			for(ii=1;ii<31;++ii)
			{
				field[i][ii] = next_field[i][ii];
			}
		}

		/*変化がない場合*/
		++t;	
		if (change == 0)
		{
			printf("stop\n");
			break;
		}
		change = 0;
		
		/*ループに入った場合*/
		for(i=1;i<31;++i)
		{
			for(ii=1;ii<31;++ii)
			{
				if(old_field[i][ii] != next_field[i][ii])
				{
					++change;
				}
			}
		}

		if (change == 0) 
		{
			++cnt;
		}
		if (cnt >= 5)
		{
		       	printf("loop\n");
			break;
		}
		change = 0;

	}

	return 0;

}
