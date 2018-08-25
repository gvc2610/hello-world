#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int R = 10, C = 10, cnt = 1;

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

void print_matrix(int A[R][C]) {

		for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%3d ", A[i][j]);
		} printf("\n");
	}
}

bool searchMatrix(int matrix[R][C], int matrixRowSize, int matrixColSize, int target) {

	if(matrix == NULL || matrixRowSize == 0 || matrixColSize == 0)
		return false;
    
    int idx_row = 0, idx_col = matrixColSize - 1;

    while(idx_row >= 0 && idx_row < matrixRowSize && idx_col < matrixColSize && idx_col >= 0) {

    	if(target == matrix[idx_row][idx_col]) {
    		printf("row:%d col:%d\n",idx_row, idx_col );
    		return true;
    	}
    	else if(target > matrix[idx_row][idx_col]) {
    		   idx_row++;
    	}
    	else {
    		  idx_col--;
    	}

    } return false;

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

	printf("Printing Diagnol\n");
    print_diagonal(A);

    print_matrix(A);

printf("search:%d\n",searchMatrix(A,R,C,91));
}