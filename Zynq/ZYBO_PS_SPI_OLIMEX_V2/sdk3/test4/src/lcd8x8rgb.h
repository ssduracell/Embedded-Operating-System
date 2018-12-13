/*
 * color - drawing color
 * drawPixel(x,y) draws drawPixel at X,Y coordinates (1,1) is upper left corner
 * drawLine(x1,y1,x2,y2) draws line
 * drawRectangle(x1,y1,x2,y2) draws rectangle
 * drawSolidRectangle(x1,y1,x2,y2) draws solid rectangle
 * drawElipse(x,y,rx,ry) draws elipse
 * drawCircle(x,y,r) draws circle
 * drawTriangle(x1,y1,x2,y2,x3,y3) draws triangle
 * drawChar(c)  - draws char at cX, cY and updates cX, cY
 * drawString(s) - draws string at cX, cY and updates cX, cY
 * lScroll() - scrolls all screen left 1 column
 * rScroll() - scrolls all screen right 1 column
 * scrollCharLeft(c) - scrolls one character from the bottom right matrix to the left
 * scrollCharRight(c) - scrolls one character from the upper left matrix to the right
 *scrollString( c, dir) - scrolls string left or right
 *
*/

#include "Matrix.h"


#define bigX 1                  // Number of MOD-LED8x8RGB in columns
#define bigY 1                  // Number of MOD-LED8x8RGB in rows
#define NumberX bigX*bigY       // Total number of MOD-LED8x8RGBs connected together
#define MATRIX_SPI_SELECT_1	0x01
#define MATRIX_SPI_SELECT_0	0x00


#define Matrix_DELAY 100000

#define SPI_DEVICE_ID		XPAR_XSPIPS_0_DEVICE_ID

u8 color = 1;        // Starting color of LEDs
u8 sdelay = 100;
//const char chipSelectPin = 13;

u8 videobuf[NumberX*24];   //video buffer

u8 cX = 1;
u8 cY = 1;



#ifndef TESTAPP_GEN
static XSpiPs SpiInstance;
#endif

u16 SpiDeviceId= SPI_DEVICE_ID;
XSpiPs *SpiInstancePtr;
XSpiPs_Config *SpiConfig;

void delay(int i)
{
	volatile int Delay;
	for (Delay = 0; Delay < i* Matrix_DELAY; Delay++);

}

void InitMatrix()
{
			int Status;
			SpiInstancePtr = &SpiInstance;
	   /*
	    	 * Initialize the SPI driver so that it's ready to use
	    	 */
	    	SpiConfig = XSpiPs_LookupConfig(SpiDeviceId);
	    	if (NULL == SpiConfig) {
	    		return XST_FAILURE;
	    	}

	    	Status = XSpiPs_CfgInitialize(SpiInstancePtr, SpiConfig,
	    					SpiConfig->BaseAddress);
	    	if (Status != XST_SUCCESS) {
	    		return XST_FAILURE;
	    	}

	    	/*
	    	 * Perform a self-test to check hardware build
	    	 */
	    	Status = XSpiPs_SelfTest(SpiInstancePtr);
	    	if (Status != XST_SUCCESS) {
	    		return XST_FAILURE;
	    	}
	    	/*
	    	 * Set the SPI device as a master with manual start and manual
	    	 * chip select mode options
	    	 */

	    	/* XSpiPs_SetOptions(SpiInstancePtr, XSPIPS_MANUAL_START_OPTION | \
	    				XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION); */

	    	 XSpiPs_SetOptions(SpiInstancePtr, XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION |\
	    			XSPIPS_FORCE_SSELECT_OPTION);

	    		/*
	    		 * Set the SPI device pre-scalar to divide by 8
	    		 */
	    	XSpiPs_SetClkPrescaler(SpiInstancePtr, XSPIPS_CLK_PRESCALE_256);
	    		/*
	    			 * Set the matrix chip select
	    			 */
	    	//u8 WriteData = 0x88;
	 /*   	u8 WriteData[24];
	    	WriteData[0]	=0b11111111;
	    	WriteData[1]	=0b11111111;
	    	WriteData[2]	=0b11111111;
	    	WriteData[4]	=0b10111101;
	    	WriteData[5]	=0b10111101;
	    	WriteData[6]	=0b10111101;
	    	WriteData[7]	=0b11011011;
	    	WriteData[8]	=0b11011011;
	    	WriteData[9]	=0b11011011;
	    	WriteData[10]	=0b11100111;
	    	WriteData[11]	=0b11100111;
	    	WriteData[12]	=0b11100111;
	    	WriteData[13]	=0b11111011;
	    	WriteData[14]	=0b11111011;
	    	WriteData[15]	=0b11111011;
	    	WriteData[16]	=0b11111101;
	    	WriteData[17]	=0b11111101;
	    	WriteData[18]	=0b11111101;
	    	WriteData[19]	=0b11111110;
	    	WriteData[20]	=0b11111110;
	    	WriteData[21]	=0b11111110;
	    	WriteData[22]	=0b11111111;
	    	WriteData[23]	=0b11111111; */
	 //   	u32 ChipSelect = MATRIX_SPI_SELECT_0;
	 //   	XSpiPs_SetSlaveSelect(SpiInstancePtr, ChipSelect);
	 //   	XSpiPs_PolledTransfer(SpiInstancePtr, WriteData, NULL, sizeof(WriteData));
}

//----------------------------------------------------------------------------------------------
void vClear() {          			//clear the video buffer
   for(int i=0; i<NumberX*24; i++)
      videobuf[i]=0;
}

//----------------------------------------------------------------------------------------------
void Transfer () {      			//transfer the video buffer to MOD-LED8x8RGB
 	//u32 ChipSelect = MATRIX_SPI_SELECT_0;
    //	XSpiPs_SetSlaveSelect(SpiInstancePtr, ChipSelect);
    //	XSpiPs_PolledTransfer(SpiInstancePtr, WriteData, NULL, sizeof(WriteData));




	u32 ChipSelect = MATRIX_SPI_SELECT_0;
	XSpiPs_SetSlaveSelect(SpiInstancePtr, ChipSelect);

	//digitalWrite(chipSelectPin, LOW);
	XSpiPs_PolledTransfer(SpiInstancePtr, videobuf, NULL, sizeof(videobuf));
	ChipSelect = MATRIX_SPI_SELECT_1;
	XSpiPs_SetSlaveSelect(SpiInstancePtr, ChipSelect);
	/* for(int i = 0; i < NumberX*24; i++){
      SPI.transfer(videobuf[i]);
      delayMicroseconds(10);
   } */
   //digitalWrite(chipSelectPin, HIGH);
}

//----------------------------------------------------------------------------------------------
void drawPixel (unsigned int X, unsigned int Y) {    //draw drawPixel at x,y coordinates to MOD-LED8x8RGB 1,1 is upper left corner
   if (Y<=bigY*8 && X<=bigX*8 && X>0 && Y>0) {
      if (Y>8) X=(X+bigX*8)*((Y-1)/8);
      Y=Y%8;
      if (Y==0) Y=8;
      
      int p;
      p=NumberX-((X-1)/8)-1;

      videobuf[3*(Y-1)+24*p]&=~(1<<((X-1)%8));                  //turn off chosen drawPixel
      videobuf[3*(Y-1)+1+24*p]&=~(1<<((X-1)%8));
      videobuf[3*(Y-1)+2+24*p]&=~(1<<((X-1)%8));

      if (color&1) videobuf[3*(Y-1)+24*p]|=(1<<((X-1)%8));      //set color to the drawPixel
      if (color&2) videobuf[3*(Y-1)+1+24*p]|=(1<<((X-1)%8));
      if (color&4) videobuf[3*(Y-1)+2+24*p]|=(1<<((X-1)%8));
   }
}

//----------------------------------------------------------------------------------------------
void drawLine (int x1, int y1, int x2, int y2) {      //draw a line from x1,y1 to x2,y2
   int dx, dy, sx, sy, err, e2;

   dx = abs (x2-x1);
   dy = abs (y2-y1);
   if (x1<x2) sx = 1;
      else sx = -1;
   if (y1<y2) sy = 1;
      else sy = -1;
   err = dx-dy;
   do {
      drawPixel (x1, y1);
      if ((x1 == x2) && (y1 == y2))
         break;
      e2 = 2*err;
      if (e2 > -dy) {
         err = err - dy;
		 x1 = x1+sx;
      }
      if (e2 < dx) {
         err = err + dx;
		 y1 = y1 + sy;
      }
   } while (1);
	return;
}

//----------------------------------------------------------------------------------------------
void drawRectangle (int x1, int y1, int x2, int y2) {  //draw a rectangle from x1,y1 to x2,y2
   drawLine (x1, y1, x1, y2);
   drawLine (x1, y1, x2, y1);
   drawLine (x2, y1, x2, y2);
   drawLine (x1, y2, x2, y2);
   return;
}

//----------------------------------------------------------------------------------------------
void drawSolidRectangle (int x1, int y1, int x2, int y2) {  //draw a solid rectangle
   if (x2>x1)
      for (int i=x1; i<=x2;i++)
         drawLine (i, y1, i, y2);
   else for (int i=x2; i<=x1;i++)
         drawLine (i, y1, i, y2);
   return;
}

//----------------------------------------------------------------------------------------------
void Draw_4_Ellipse_Points (int CX, int CY, int X, int Y) {  //function needed for drawing an ellipse
   drawPixel (CX+X, CY+Y);
   drawPixel (CX-X, CY+Y);
   drawPixel (CX-X, CY-Y);
   drawPixel (CX+X, CY-Y);
   return;
}

//----------------------------------------------------------------------------------------------
void drawEllipse (int CX, int CY, int XRadius, int YRadius) {   //draw an ellipse & fix radius if negative
   int X, Y, XChange, YChange, EllipseError, TwoASquare, TwoBSquare, StoppingX, StoppingY;
   if (XRadius<0) XRadius=-XRadius;
   if (YRadius<0) YRadius=-YRadius;

   TwoASquare = 2 * XRadius*XRadius;
   TwoBSquare = 2 * YRadius*YRadius;
   X = XRadius;
   Y = 0;
   XChange = YRadius*YRadius * (1-2*XRadius);
   YChange = XRadius*XRadius;
   EllipseError = 0;
   StoppingX = TwoBSquare*XRadius;
   StoppingY = 0;

   while (StoppingX >= StoppingY) {	        // 1st set of points, y'> -1
      Draw_4_Ellipse_Points (CX, CY, X, Y);
      Y++;
      StoppingY = StoppingY + TwoASquare;
      EllipseError = EllipseError + YChange;
      YChange = YChange + TwoASquare;
      if ((2*EllipseError + XChange) > 0) {
		 X--;
		 StoppingX = StoppingX - TwoBSquare;
		 EllipseError = EllipseError + XChange;
		 XChange = XChange + TwoBSquare;
   }}

   X = 0;
   Y = YRadius;
   XChange = YRadius*YRadius;
   YChange = XRadius*XRadius * (1-2*YRadius);
   EllipseError = 0;
   StoppingX = 0;
   StoppingY = TwoASquare * YRadius;

   while (StoppingX <= StoppingY) {        // 2nd set of points, y'< -1
      Draw_4_Ellipse_Points (CX, CY, X, Y);
      X++;
      StoppingX = StoppingX + TwoBSquare;
      EllipseError = EllipseError + XChange;
      XChange = XChange + TwoBSquare;
      if ((2*EllipseError + YChange) > 0) {
         Y--;
		 StoppingY = StoppingY - TwoASquare;
		 EllipseError = EllipseError + YChange;
		 YChange = YChange + TwoASquare;
   }}
   return;
}

//----------------------------------------------------------------------------------------------
void drawCircle (int x, int y, int r) {   //draw a circle
   drawEllipse (x, y, r, r);
   return;
}

//----------------------------------------------------------------------------------------------
void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3) {   //draw a triangle
   drawLine (x1, y1, x2, y2);
   drawLine (x2, y2, x3, y3);
   drawLine (x3, y3, x1, y1);
   return;
}

//----------------------------------------------------------------------------------------------
void drawChar(unsigned char c) {  //draw static character  if within the Font limit
   unsigned char b,i;
   signed char k;
   if (c<32 || c>125) c=32;

   for(k=0;k<5;k++) {
      b = FontLookup[c-32][k];
      for(i=0;i<8;i++)
         if (b & (1<<(7-i))) drawPixel(k+cX,i+cY);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
   }
}

//----------------------------------------------------------------------------------------------
void drawString( unsigned char c[]) {  //draw static string
      for(int i=0; c[i];i++) {
         drawChar(c[i]);
         cX +=6;
      }
}

//----------------------------------------------------------------------------------------------
void lScroll() {   //scroll everything left
   for (int i=NumberX*24-1;i>=0;i--) {
      videobuf[i]=videobuf[i]>>1;
      if (i>=24) videobuf[i]|=((videobuf[i-24]&1)<<7);
   }
}

//----------------------------------------------------------------------------------------------
void rScroll() {   //scroll everything right
   for (int i=0;i<NumberX*24;i++) {
      videobuf[i]=videobuf[i]<<1;
      if (i<(NumberX-1)*24) videobuf[i]|=(videobuf[i+24]>>7);
   }
}

//----------------------------------------------------------------------------------------------
void scrollCharLeft(u8 c) {   //scroll one character left if within the Font limit
   unsigned char b,i,k;
   if (c<32 || c>125) c=32;

   for(k=0;k<5;k++) {
      b = FontLookup[c-32][k];
      for(i=0;i<8;i++)
         if (b & (1<<(7-i))) drawPixel(bigX*8,i+1+(bigY-1)*8);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
      Transfer();
      lScroll();
      delay(sdelay);
   }
   Transfer();
   lScroll();
   delay(sdelay);
}

//----------------------------------------------------------------------------------------------
void scrollCharRight(u8 c) {  //scroll one character right if within the Font limit
   u8 b,i;
   Xint8 k;	// SPP - k must be able to take negative value so it should be signed char
   if (c<32 || c>125) c=32;

   rScroll();
   for(k=4;k>=0;k--) {
      b = FontLookup[c-32][k];
      for(i=0;i<8;i++)
         if (b & (1<<(7-i))) drawPixel(1,i+1);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
      Transfer();
      rScroll();
      delay(sdelay);
   }
   Transfer();
   delay(sdelay);
}

//----------------------------------------------------------------------------------------------
void theEnder(bool directions) {  //move everything left/right until it leaves the screen
   for (int i=0;i<NumberX*8-1;i++) {
      if (directions) rScroll();
         else lScroll();
      Transfer();
      delay(sdelay);
   }
}

//----------------------------------------------------------------------------------------------
void scrollString(u8 c[], bool directions) { //draw a scrolling string

      int len;
      for(len=0;c[len];len++);

      if (directions) {
         for(int i=len-1; i>=0;i--) {
            scrollCharRight(c[i]);
            color++; if (color>7) color = 1;
         }
      } else {
         for(int i=0;c[i];i++) {
            scrollCharLeft(c[i]);
            color++; if (color>7) color = 1;
         }
      }
      theEnder(directions);
}
