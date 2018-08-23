#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
 
// uint32_t msb32(uint32_t n)
// {
// 	uint32_t b = 1;
// 	if (!n) return 0;
 
// #define step(x) if (n >= ((uint32_t)1) << x) b <<= x, n >>= x
// 	step(16); step(8); step(4); step(2); step(1);
// #undef step
// 	return b;
// }
 
// int msb32_idx(uint32_t n)
// {
// 	int b = 0;
// 	if (!n) return -1;
 
// #define step(x) if (n >= ((uint32_t)1) << x) b += x, n >>= x
// 	step(16); step(8); step(4); step(2); step(1);
// #undef step
// 	return b;
// }

uint8_t computeSetBits(uint16_t *num)
{
    uint16_t count = 0;
    uint16_t temp = *num;
    while (temp)
    {
      temp &= (temp-1) ;
      count++;
    }
    return count;
}

int msb16_idx(uint16_t n)
{
	int b = 0;
	if (!n) return -1;
 
#define step(x) if (n >= ((uint16_t)1) << x) b += x, n >>= x
	    step(8); step(4); step(2); step(1);
#undef step
	return b;
}

int findComplement1(int num) {
    
    if(num == 0) return 1;
    if(num == 1) return 0;
    
    int num_com = 0;
    int msb_loc = -1;
    int temp = num;
    while(temp) {
        temp = temp>>1;
        msb_loc++;
    }
    
    int mask = 1 << msb_loc;
    //printf("%d",msb_loc);
    
    while(mask) {
    num_com = (num & mask) ? (num_com):(num_com|0x1);
        //printf("num:%d num_com:%d mask:%d num & mask:%d\n",num,num_com,mask,((num & mask))?1:0);
        //msb_loc--;
        mask = mask>>1;

        if(mask) num_com = num_com <<1;

    }
    //printf("num_com:%d",num_com);
    return num_com;
}

int findComplement(int num) {
    long i;
    for(i=1;i<=num;i*=2)
        num^=i;
    return num;
}

bool isPowerOfFour1(int num) {

    if(num<0) return false;
    int loc =-1;
        
    if(((num)&(num-1))==0) {
       while(num) {
       num=num>>1;loc++;}    
      } else return false;
    
    printf("\nloc:%d\n",loc);
    return loc%2==0?true:false;   
}

bool isPowerOfFour(int num) {
    if( (num&(num-1))==0 && (num&0x55555555)!=0 ) return true;
    else
        return false;
}
 
//#define lsb32(n) ( (uint32_t)(n) & -(int32_t)(n) )

#define lsb16(n) ( (uint16_t)(n) & -(int16_t)(n) )
 
/* finding the *position* of the least significant bit
   rarely makes sense, so we don't put much effort in it*/
//int lsb32_idx(uint32_t n) { return msb32_idx(lsb32(n)); }
int lsb16_idx(uint16_t n) { return msb16_idx(lsb16(n)); }


void arrayPopulatorFromBitMap(uint8_t *ByteArray, uint16_t *bitArray) {
uint16_t temp_bitArray = *bitArray;
uint8_t temp_counter = 0;

uint8_t start_idx = lsb16_idx(temp_bitArray);
uint8_t end_idx = msb16_idx(temp_bitArray);

printf("start_idx:%d end_idx:%d\n",start_idx,end_idx );
while(start_idx <= end_idx) {
    ByteArray[temp_counter] = start_idx;
    temp_counter++; start_idx++;
    }
}

unsigned reversebits1(unsigned int num) {
 
  num = ((num & 0x55555555)<<1) | ((num & 0xaaaaaaaa) >> 1);  // 0101 0101 0101 0101 0101 0101 0101 0101  :: 1010 1010
  num = ((num & 0x33333333)<<2) | ((num & 0xcccccccc) >> 2);  // 0011 0011  :: 1100 1100
  num = ((num & 0x0f0f0f0f)<<4) | ((num & 0xf0f0f0f0) >> 4);  // 
  num = ((num & 0x00ff00ff)<<8) | ((num & 0xff00ff00) >> 8);
  num = ((num & 0x0000ffff)<<16) | ((num & 0xffff0000) >> 16);


  return num;

}
unsigned reversebits(unsigned int v) {
//unsigned int v; // 32-bit word to reverse bit order

// swap odd and even bits
v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
// swap consecutive pairs
v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
// swap nibbles ... 
v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
// swap bytes
v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
// swap 2-byte long pairs
v = ( v >> 16             ) | ( v               << 16);


return v;
}
/*

unsigned int s = sizeof(v) * CHAR_BIT; // bit size; must be power of 2 
unsigned int mask = ~0;         
while ((s >>= 1) > 0) 
{
  mask ^= (mask << s);
  v = ((v >> s) & mask) | ((v << s) & ~mask);
}

*/

/*
int isBitPalindrome(unsigned int num) {

  //int cnt = sizeof(num)*CHAR_BIT;
  int itr = 16; 
  unsigned int mask_left = 0x80000000;
  unsigned int mask_right = 0x00000001;
   int cnt = 0;
   while(itr) {
    printf("cnt:%d num:%x,num&mask_left:%x  num&mask_right:%x\n", cnt,num,(num&mask_left),(num&mask_right));
      if((num&mask_left) && (num&mask_right>0)) 
        cnt++;
      
      mask_left=mask_left >> 1;
      mask_right=mask_right << 1;
      itr--;
   }
   return cnt==16?1:0;
  
}
*/

int isBitPalindrome(unsigned int num) {

int left = sizeof(num)*8-1, right = 0, cnt = 0;
printf("size:%lu\n",sizeof(num) );

  while(left>right){
       if(((num & (0x1 << left)) >> left) == ((num & (0x1 << right)) >> right))  {
           cnt++;
          // printf("cnt:%d\n", cnt);
       } 
        left--;right++;
    }
    return cnt==(sizeof(num)*4)?1:0;
}

int * gray_codes(int num,int *num_codes) {

   int code_cnt =  pow(2,num);
   *num_codes = code_cnt;

   printf("num_codes:%d\n",*num_codes );

   int *gray_array = (int*)malloc(sizeof(int)*code_cnt);

   int curr_cnt;

   if(num == 0) {
    gray_array[0] = 0;
    return gray_array;
   }

   for(int i = 1; i <= num; i++) {
      
       if(i == 1) {
        gray_array[0] = 0;
        gray_array[1] = 1;
        continue;
       }

      curr_cnt = pow(2,i);

      for(int j = curr_cnt/2,k = ((curr_cnt/2)-1); j < curr_cnt && k >=0; j++,k--) {
          gray_array[j] = gray_array[k];
          gray_array[j] = gray_array[j] | 1 << (i-1);
      }
      
   }
return gray_array;

}

int main()
{
	int32_t n;
	int i;
 
	// for (i = 0, n = 1; ; i++, n *= 42) {
	// 	printf("42**%d = %10d(x%08x): M x%08x(%2d) L x%03x(%2d)\n",
	// 		i, n, n,
	// 		msb32(n), msb32_idx(n),
	// 		lsb32(n), lsb32_idx(n));
 
	// 	if (n >= INT32_MAX / 42) break;
	// }

	printf("0xffe0 M:%d  L:%d \n",msb16_idx(0xffe0), lsb16_idx(0xffe0));
   
  uint8_t BytesArray[16];
  uint16_t BitArray = {0xffe0}; 
    
  arrayPopulatorFromBitMap(BytesArray,&BitArray); 

    for(int i = 0; i < computeSetBits(&BitArray); i++)
    	printf("%d\n", BytesArray[i]);

    printf("palindrome:%d\n", isBitPalindrome(0x2f0ff0f4));
    printf("palindrome:%d\n", isBitPalindrome(0x0f0ff000));


    printf("rev:%x",reversebits(0xaaaaaaaa));


    uint8_t bit8 = 0xaa;
    uint8_t rot_bit8 = 0x55;
    uint16_t bit16 = 0xaaaa;
  
    printf("%8x\n",bit8|bit16);



     uint32_t int32 = 0xffffffff;

    printf("int32:%d\n",int32 & 0xf );

    printf("x1:%x\n",0x123 & ((0x1<<9)-1));
    printf("x2:%x\n",0x123 & ((0x1<<8)-1));

    printf("%x\n",findComplement(5) );
    printf("%d\n",isPowerOfFour(16) );
    int num = 0xA;
    printf("n1:%d  n2:%d\n",(num & (0x1 << 1)) >> 1,(num & (0x1<<3)) >> 3 );

     int num_codes = 0;
    int *gray = gray_codes(5,&num_codes);

    for(int i = 0; i< num_codes; i++) {
      printf("gray code: %d\n",gray[i] );
    }
    free(gray);

	return 0;
}