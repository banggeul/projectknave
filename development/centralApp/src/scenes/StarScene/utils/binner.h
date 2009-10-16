#include <stdio.h>

#ifndef 	BINNER_H
#define 	BINNER_H

//------------------------------------------------------
static int 				nBinDivisionsW; // must be an odd number
static int 				nBinDivisionsH; // must be an odd number
static float 			binDivisionWidth;
static float			binDivisionWidthInv;
static float 			binDivisionHeight;
static float 			binDivisionHeightInv;

inline void 			setupBins(int w, int h);	// do this once!
inline void 			setupBins(int w, int h){

	nBinDivisionsW 				= 14;
	nBinDivisionsH 				= 14;
	binDivisionWidth  			= w / (float)nBinDivisionsW;
	binDivisionHeight 			= h / (float)nBinDivisionsH;
	binDivisionWidthInv  		= 1.0f/binDivisionWidth;
	binDivisionHeightInv 		= 1.0f/binDivisionHeight;
    printf("setup %f %f \n", binDivisionWidth, binDivisionHeight);
}


inline void 	computeBinPosition(int xpos, int ypos, unsigned int * bitFlagW, unsigned int * bitFlagH);
inline void 	computeBinPosition(int xpos, int ypos, unsigned int * bitFlagW, unsigned int * bitFlagH){

	int nBinsOut;
	int i;
	int posBin1;int posBin2;*bitFlagW = 0x00000000;*bitFlagH = 0x00000000;
	nBinsOut = 1;

	//---------------------- x -----------------------
	posBin1 = (int)((float)xpos * binDivisionWidthInv);

	*bitFlagW |= (0x00000001 << posBin1);
	for (i = 0; i < nBinsOut; i++){
		if (posBin1 - i - 1 >= 0){
			*bitFlagW |= (0x00000001 << (posBin1-i-1));

		}

		if (posBin1 + i + 1 < nBinDivisionsW){
			*bitFlagW |= (0x00000001 << (posBin1 + i + 1));

		}
	}
    //printf("%i \n", *bitFlagW);

	//---------------------- y -----------------------
	posBin1 = (int)((float)ypos * binDivisionHeightInv);
	*bitFlagH |= (0x00000001 << posBin1);
	for (i = 0; i < nBinsOut; i++){
		if (posBin1 - i - 1 >= 0){
			*bitFlagH |= (0x00000001 << (posBin1-i-1));
		}
		if (posBin1 + i + 1 < nBinDivisionsH){
			*bitFlagH |= (0x00000001 << (posBin1 + i + 1));
		}
	}


}
//------------------------------------------------------



inline void 	computeBinPositionWithBinExt(int xpos, int ypos, int nBinsw, int nBinsh, long long *bitFlag, 		long long *bitFlag2);
inline void 	computeBinPositionWithBinExt(int xpos, int ypos, int nBinsw, int nBinsh,  long long *bitFlagW, long long *bitFlagH){


	int i;
	int posBin1;int posBin2;*bitFlagW = 0x00000000;*bitFlagH = 0x00000000;

	//---------------------- x -----------------------
	posBin1 = (int)((float)xpos * binDivisionWidthInv);
	*bitFlagW |= (0x00000001 << posBin1);
	for (i = 0; i < nBinsw; i++){
		if (posBin1 - i - 1 >= 0){
			*bitFlagW |= (0x00000001 << (posBin1-i-1));
		}
		if (posBin1 + i + 1 < nBinDivisionsW){
			*bitFlagW |= (0x00000001 << (posBin1 + i + 1));
		}
	}


	//---------------------- y -----------------------
	posBin1 = (int)((float)ypos * binDivisionHeightInv);
	*bitFlagH |= (0x00000001 << posBin1);
	for (i = 0; i < nBinsh; i++){
		if (posBin1 - i - 1 >= 0){
			*bitFlagH |= (0x00000001 << (posBin1-i-1));
		}
		if (posBin1 + i + 1 < nBinDivisionsH){
			*bitFlagH |= (0x00000001 << (posBin1 + i + 1));
		}
	}
}



#endif
