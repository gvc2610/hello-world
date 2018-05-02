#include <stdio.h>
#include <stdlib.h>

int cnt = 1;
int R = 4,C = 4;

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


	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			printf("%3d ", A[i][j]);
		} printf("\n");
	}

}