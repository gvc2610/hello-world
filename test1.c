#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#define A_UINT8 uint8_t
#define TPC_NUM_BAND 2
#define MAX_NUM_CHAINS 2
#define WHAL_NUM_POINTS_TO_MEAS 32
#define WHAL_NUM_POINTS_TO_MEAS_GRP 16
#define TPC_5G 0

int main() {

A_UINT8 txCalGainIdx5G_dflt[] = {3,5,7,9,11,13,15,17,19,21,23,25,27};  
A_UINT8 txCalGainIdxGrp2_5G_dflt[] = {1,2,255,255,255,255,255,255,255,255,255,255,255,255,255,255};  
A_UINT8 txCalGainIdxGrp3_5G_dflt[] = {100,101,102,103,104,255,255,255,255,255,255,255,255,255,255,255,255};
A_UINT8 txCalGainIdxGrp4_5G_dflt[] = {200,201,202,203,255,255,255,255,255,255,255,255,255,255,255,255}; 


A_UINT8 Max_TX_CAL_Gain_ID_Grp2_5G = 2;
A_UINT8 Max_TX_CAL_Gain_ID_Grp3_5G = 5;
A_UINT8 Max_TX_CAL_Gain_ID_Grp4_5G = 4;
A_UINT8 Max_TX_CAL_Gain_ID_5G = (sizeof(txCalGainIdx5G_dflt) / sizeof(A_UINT8));

typedef struct {
A_UINT8  numGain;
A_UINT8  numGainEndGrp2;
A_UINT8  numGainEndGrp3;
A_UINT8  numGainEndGrp4;
A_UINT8  gainIdx[MAX_NUM_CHAINS][WHAL_NUM_POINTS_TO_MEAS];
A_UINT8  gainIdxGrp2[MAX_NUM_CHAINS][WHAL_NUM_POINTS_TO_MEAS_GRP];
A_UINT8  gainIdxGrp3[MAX_NUM_CHAINS][WHAL_NUM_POINTS_TO_MEAS_GRP];
A_UINT8  gainIdxGrp4[MAX_NUM_CHAINS][WHAL_NUM_POINTS_TO_MEAS_GRP];
A_UINT8  gainIdxMaster[MAX_NUM_CHAINS][WHAL_NUM_POINTS_TO_MEAS];
} TPC_CAL_CFG;

TPC_CAL_CFG                    tpcCfg[TPC_NUM_BAND];
tpcCfg[0].numGain = Max_TX_CAL_Gain_ID_5G;

    tpcCfg[0].numGainEndGrp2 = Max_TX_CAL_Gain_ID_Grp2_5G + tpcCfg[0].numGain;


    tpcCfg[0].numGainEndGrp3 = Max_TX_CAL_Gain_ID_Grp3_5G + tpcCfg[0].numGainEndGrp2;


    tpcCfg[0].numGainEndGrp4 = Max_TX_CAL_Gain_ID_Grp4_5G + tpcCfg[0].numGainEndGrp3;




memcpy((void*)&(tpcCfg[0].gainIdxMaster[0][0]), (void*)txCalGainIdx5G_dflt, Max_TX_CAL_Gain_ID_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[0][tpcCfg[0].numGain]), (void*)txCalGainIdxGrp2_5G_dflt, Max_TX_CAL_Gain_ID_Grp2_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[0][tpcCfg[0].numGainEndGrp2]), (void*)txCalGainIdxGrp3_5G_dflt, Max_TX_CAL_Gain_ID_Grp3_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[0][tpcCfg[0].numGainEndGrp3]), (void*)txCalGainIdxGrp4_5G_dflt, Max_TX_CAL_Gain_ID_Grp4_5G);


memcpy((void*)&(tpcCfg[0].gainIdxMaster[1][0]), (void*)txCalGainIdx5G_dflt, Max_TX_CAL_Gain_ID_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[1][tpcCfg[0].numGain]), (void*)txCalGainIdxGrp2_5G_dflt, Max_TX_CAL_Gain_ID_Grp2_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[1][tpcCfg[0].numGainEndGrp2]), (void*)txCalGainIdxGrp3_5G_dflt, Max_TX_CAL_Gain_ID_Grp3_5G);
memcpy((void*)&(tpcCfg[0].gainIdxMaster[1][tpcCfg[0].numGainEndGrp3]), (void*)txCalGainIdxGrp4_5G_dflt, Max_TX_CAL_Gain_ID_Grp4_5G);

    memcpy((void*)&(tpcCfg[TPC_5G].gainIdx[0][0]),(void*)&(tpcCfg[TPC_5G].gainIdxMaster[0][0]),tpcCfg[TPC_5G].numGainEndGrp4);
    memcpy((void*)&(tpcCfg[TPC_5G].gainIdx[1][0]),(void*)&(tpcCfg[TPC_5G].gainIdxMaster[1][0]),tpcCfg[TPC_5G].numGainEndGrp4);

for (int i = 0 ; i < tpcCfg[0].numGainEndGrp4 ; i++)
{
	//printf("%d\n", tpcCfg[0].gainIdxMaster[1][i] );
}

for (int i = 0 ; i < tpcCfg[0].numGainEndGrp4 ; i++)
{
	printf("%d\n", tpcCfg[0].gainIdx[1][i] );
}


return 0;	
}