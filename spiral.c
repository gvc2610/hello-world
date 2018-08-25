#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cnt = 1;
int R = 10,C = 10;

void traverese_Left2Right(int A[R][C], int left,int right, int posRow) {
   for(int i = left; i <= right; i++) {
   	A[posRow][i] = cnt++; printf("%d \n",cnt-1 );
   }
}

void traverese_Top2Bottom(int A[R][C],int top,int bottom,int posCol) {
	for(int i = top; i<= bottom; i++) {
		A[i][posCol] = cnt++; printf("%d \n",cnt-1 );
	}
}

void traverese_Right2Left(int A[R][C],int right,int left,int negRow) {
	for (int i = right; i >= left; i--) {
	    A[negRow][i] = cnt++; printf("%d \n",cnt-1 );
	}
}

void traverese_Bottom2Top(int A[R][C], int bottom,int top,int negCol) {
	for (int i = bottom; i >= top; i--) {
		A[i][negCol] = cnt++; printf("%d \n",cnt-1 );
	}
}

void transpose(int A[R][C]) {

	int c1=0,temp;

	for(int i = 0; i < R; i++) {
		for(int j = c1; j < C; j++) {
           temp = A[i][j];
           A[i][j] = A[j][i];
           A[j][i] = temp;         
		} c1++;
	}
}

void reverse_row(int A[R][C]) {
int left = 0, right = C-1, temp;

	for(int i = 0; i< R; i++) {
		while(left <= right){
		   temp = A[i][left];
           A[i][left] = A[i][right];
           A[i][right] = temp;
           left++,right--;
		}left = 0, right = C-1;
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


int main() {

int A[R][C];

int left = 0, right = C-1, top = 0, bottom = R-1;
int posRow = 0, posCol = C-1, negRow = R-1, negCol = 0;


	//while (left <= (C+1)/2 && top <= (R+1)/2) {
	while(1){
    //if(left<=N-1 & ;e)
	if(cnt <= R*C){	
	printf("left:%d  right:%d posRow:%d\n",left,right,posRow );	
	traverese_Left2Right(A,left,right,posRow);
    top++;} else break;
    
    if(cnt <= R*C){	
    printf("top:%d  bottom:%d posCol:%d\n",top,bottom,posCol );	
	traverese_Top2Bottom(A,top,bottom,posCol);
    right--;} else break;

    if(cnt <= R*C){	
    printf("right:%d left:%d\n negRow:%d\n",right,left,negRow );	
	traverese_Right2Left(A,right,left,negRow);
    bottom--;} else break; 

    if(cnt <= R*C){	
    printf("bottom:%d  top:%d negCol:%d\n",bottom,top,negCol );	
	traverese_Bottom2Top(A,bottom,top,negCol);
    left++; } else break;

    posRow++; posCol--; negRow--; negCol++;

	}

print_matrix(A);
transpose(A);
printf("Transposed Matrix:\n\n\n");
print_matrix(A);

reverse_row(A);
printf("Row reversed Matrix:\n\n\n");
print_matrix(A);

}