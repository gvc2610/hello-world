#include <stdio.h>
#include <stdlib.h>

int R = 20, C = 20, cnt = 1;

void print_row(int A[R][C], int row_id, int sum) {
    
    int col_id = sum-row_id;
     
   // printf("row_id:%d  col_id:%d  sum:%d\n",row_id,col_id,sum );

	while(row_id >=0 && row_id <= R-1 && col_id >=0 && col_id <= C-1) {
		printf("%3d ",A[row_id][col_id]);
		row_id--;
		col_id = sum-row_id;
	}

}

int row_id = 0,col_id = 0;
void print_diagonal(int A[R][C]) {

for(int i =0; i< (R+C)-1; i++) {

	print_row(A,row_id,i);
	printf("\n");

	if(row_id < R-1) row_id++;
  }
}

int main() {
int A[R][C];
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			A[i][j] = cnt;cnt++;
		} 
	}

		for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%3d ", A[i][j]);
		} printf("\n");
	}

	printf("Printing Diagnoal\n");
    print_diagonal(A);
}