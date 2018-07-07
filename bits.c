#include <stdio.h>
#include <stdint.h>
 
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

unsigned reversebits(unsigned int num) {
 
  num = ((num & 0x55555555)<<1) | ((num & 0xaaaaaaaa) >> 1);  // 0101 0101 0101 0101 0101 0101 0101 0101  :: 1010 1010
  num = ((num & 0x33333333)<<2) | ((num & 0xcccccccc) >> 2);  // 0011 0011  :: 1100 1100
  num = ((num & 0x0f0f0f0f)<<4) | ((num & 0xf0f0f0f0) >> 4);  // 
  num = ((num & 0x00ff00ff)<<8) | ((num & 0xff00ff00) >> 8);
  num = ((num & 0x0000ffff)<<16) | ((num & 0xffff0000) >> 16);

return num;

}
/*
int isBitPalindrome(unsigned int num) {

  int cnt = 32; 
   while(cnt > 0) {


   }

}
*/



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

    //isBitPalindrome(0x16);


    printf("rev:%x",reversebits(0xaaaaaaaa));


    uint8_t bit8 = 0xaa;
    uint8_t rot_bit8 = 0x55;
    uint16_t bit16 = 0xaaaa;
  
    printf("%8x\n",bit8|bit16);

	return 0;
}