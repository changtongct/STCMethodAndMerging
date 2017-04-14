#ifndef _COMMOMFILE_H_
#define _COMMOMFILE_H_

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <math.h>

#ifndef M
#define M 200
#define N 200
#endif

int judgement(int x1, int x2, int y1, int y2, IplImage * img, double Threshold);
void GreyValueRecord(int x1, int x2, int y1, int y2, IplImage * img);
void matrixprocess(int x1, int x2, int y1, int y2, int flag[M][N]);
void segmentdisplay(int x1, int x2, int y1, int y2, IplImage * img);
void ClearTextfile(char filename[]);
void OutputEncodedFlagToTextfile(char Q[]);
void OutputMatrixToTextfile(int flag[M][N]);
void OutputGreyValueRecordToTextfile();
int NumberOfBits(int number);
void DecimalToTenBinary(int decimal, char binary[]);
int BinaryToDecimal(char binary[], int BinaryNum);
uchar GetGrayValue( int x, int y, IplImage *img);
void BlockRegeneration( int x1, int x2, int y1, int y2, uchar topleft, uchar topright, uchar bottomleft, uchar bottomright ,IplImage * img);

#endif