 jd#include <stdio.h>
#include <stdlib.h>

//Note: Don't include mid in the regular search cases

int binsrch(int *A, int l,int h, int val) {

	int low = l, high = h, mid = 0;

    if(val > A[high] || val < A[low])
    	return -1;

    while (low <= high ) {
    	mid = (low+high)/2;

    if (val == A[mid]) return mid;    

	else if(val < A[mid]) 
       high = mid-1;
	
    else
       low = mid+1; 
   }
   return -1;
}


int first_binsrch(int *A, int n, int val) {

    int low = 0, high = n-1, mid = 0, first = 0;

    if(val > A[high] || val < A[low])
    	return -1;

    while (low <= high) {

    	mid = (high+low)/2; first = mid;
    	if(val >= A[low] && val < A[mid])
    		high = mid-1;
    	else if(val > A[mid] && val <= A[high])
    		low = mid +1;
    	else{
    	  if (A[mid] == A[mid-1])
    		{first = mid-1, high = mid-1;}
          else return first;
        }
    }
}

int last_binsrch(int *A, int n, int val) {

    int low = 0, high = n-1, mid = 0, last = 0; 

    if(val > A[high] || val < A[low])
    	return -1;

   while (low <= high) {
            mid = (low+high)/2; last = mid;
        if(val >= A[low] && val < A[mid])
        	high = mid-1;
        else if (val > A[mid] && val <= A[high])
        	low = mid + 1;
        else {
        	   if(A[mid] == A[mid+1])
        	   	{last = mid +1; low = mid+1;}
        	   	else return last; 

        }

    }
 }


int circular_binsrch(int *A, int n, int val) {
	int low = 0, high = n-1, mid =0;

	while(low <= high) {
          mid = (low + high)/2; 
 	
		if (A[low] < A[high]) {
			return binsrch(A,low,high,val);

		}
		else if(A[low] < A[mid]) {
			if(val >= A[low] && val < A[mid])
				high = mid - 1; 
			else low = mid + 1;
		}
		else if (A[high] > A[mid]) {
			if (val > A[mid+1] && val <= A[high]) 
				low = mid+1;
			else high = mid - 1; 
		}
		else  return mid;
	}
printf("check6\n");	
return -1;

}

int main() {

	int arr[] =  {0,1,2,6,7,8};
	register int cnt asm("eax");
	printf("%d\n", cnt);
    int arr1[] = {0,1,2,2,2,5,6,7,8};
    int arr2[] = {2,2,2,2,2,2,2,2,2};
    int arr3[] = {4,5,6,7,8,0,1,2,3};
 	printf("find 10:%d\n",binsrch(arr,0,5,8));
// // 	printf("find 0:%d\n",binsrch(arr,9,0));
   //  	printf("find %d\n",binsrch(arr,0,8,5));
// // 	printf("find 4:%d\n",binsrch(arr,9,4));
// // 	printf("find 6:%d\n",binsrch(arr,9,6));
// // 	printf("find 5:%d\n",binsrch(arr1,9,5));
//  	printf("find left 2:%d\n",first_binsrch(arr2,9,2));
//  	printf("find right 2:%d\n",last_binsrch(arr1,9,2));
//  	printf("find right 2:%d\n",last_binsrch(arr2,9,2));
 	 printf("find circular 2:%d\n",circular_binsrch(arr3,9,2));
 	 printf("find circular 10:%d\n",circular_binsrch(arr3,9,10));
 	 printf("find circular 4:%d\n",circular_binsrch(arr3,9,4));
 	 printf("find circular 3:%d\n",circular_binsrch(arr3,9,3));
 }