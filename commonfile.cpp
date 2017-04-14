#include"commonfile.h"

struct color
{
	int topleft;
	int topright;
	int bottomleft;
	int bottomright;
}P[3500];
int ColorNum = 0;

int judgement(int x1, int x2, int y1, int y2, IplImage * img, double Threshold) //返回1可以结束，返回0表示需要继续扫描
{
	double i1=0,i2=0;
	int x,y;
	CvScalar  g1s,g2s,g3s,g4s,gs;
	double g1,g2,g3,g4,g5,g6,g_est,g,g_tem;
	g1s = cvGet2D(img,y1,x1);
	g2s = cvGet2D(img,y1,x2);
	g3s = cvGet2D(img,y2,x1);
	g4s = cvGet2D(img,y2,x2);
	g1 = g1s.val[0];
	g2 = g2s.val[0];
	g3 = g3s.val[0];
	g4 = g4s.val[0];

	if(x2-x1!=0 && y2-y1!=0)//正常矩形
	{
		for(y=0;y<=(y2-y1);y++)
			for(x=0;x<=(x2-x1);x++)
			{
				i1=(double)y/(y2-y1);
				i2=(double)x/(x2-x1);
				g5=g1+(g2-g1)*i2;
				g6=g3+(g4-g3)*i2;
				g_est=g5+(g6-g5)*i1;
				gs = cvGet2D(img,(y1+y),(x1+x));
				g = gs.val[0];
				g_tem = abs(g_est-g);
				if(g_tem>Threshold)
				{
					/*P[Num].topleft = g1;
					P[Num].topright = g2;
					P[Num].bottomleft = g3;
					P[Num].bottomright = g4;*/
					return 1;
				}
			}
		return 0;
	}
	else if(x2-x1!=0 && y2-y1==0)//宽为1的矩形
	{
		for(x=0;x<=(x2-x1);x++)
		{
			i2=(double)x/(x2-x1);
			g_est=g1+(g4-g1)*i2;
			gs = cvGet2D(img,y1,(x1+x));
			g = gs.val[0];
			g_tem = abs(g_est-g);
			if(g_tem>Threshold)
			{
				return 1;
			}
		}
		return 0;
	}
	else if(x2-x1==0 && y2-y1!=0)//长为1的矩形
	{
		for(y=0;y<=(y2-y1);y++)
		{
			i1=(double)y/(y2-y1);
			g_est=g1+(g4-g1)*i1;
			gs = cvGet2D(img,(y1+y),x1);
			g = gs.val[0];
			g_tem = abs(g_est-g);
			if(g_tem>Threshold)
			{
				return 1;
			}
		}
		return 0;
	}
	else if(x2-x1==0 && y2-y1==0)//孤立点矩形
	{
		return 0;
	}
	else
	{
		return false;
	}
}

void GreyValueRecord(int x1, int x2, int y1, int y2, IplImage * img)
{
	CvScalar  g1,g2,g3,g4;

	g1 = cvGet2D(img,y1,x1);
	g2 = cvGet2D(img,y1,x2);
	g3 = cvGet2D(img,y2,x1);
	g4 = cvGet2D(img,y2,x2);

	P[ColorNum].topleft = (int)g1.val[0];
	P[ColorNum].topright = (int)g2.val[0];
	P[ColorNum].bottomleft = (int)g3.val[0];
	P[ColorNum].bottomright = (int)g4.val[0];

	ColorNum++;
}

void matrixprocess(int x1, int x2, int y1, int y2, int flag[M][N]) //将已处理过的点标志为3
{
	int i,j;
	for(j=y1;j<=y2;j++)
		for(i=x1;i<=x2;i++)
		{
			if(flag[j][i]==0)
			{
				flag[j][i]=3;
			}
		}
}

void segmentdisplay(int x1, int x2, int y1, int y2, IplImage * img)
{
	if((x1>0) && (y1>0))
	{
		cvRectangle(img,cvPoint((x1-1),(y1-1)),cvPoint(x2,y2),cvScalar(0,0,0));
	}
	else if((x1==0) && (y1>0))
	{
		cvRectangle(img,cvPoint(x1,(y1-1)),cvPoint(x2,y2),cvScalar(0,0,0));
	}
	else if((x1>0) && (y1==0))
	{
		cvRectangle(img,cvPoint((x1-1),y1),cvPoint(x2,y2),cvScalar(0,0,0));
	}
	else
	{
		cvRectangle(img,cvPoint(x1,y1),cvPoint(x2,y2),cvScalar(0,0,0));
	}
}

void ClearTextfile(char filename[])
{
	FILE *fp;
	fp = fopen(filename,"w");
	fclose(fp);
}

void OutputEncodedFlagToTextfile(char Q[])
{
	ClearTextfile("EncodedFlag.txt");

	FILE *fp; 
	fp = fopen("EncodedFlag.txt","a");
	for(int i=0 ; i<60000 ; i++)
	{
		fprintf(fp,"%c",Q[i]);
	}
	fclose(fp); 
}

void OutputMatrixToTextfile(int flag[M][N])
{
	ClearTextfile("FlagMatrix.txt");

	FILE *fp;
	fp = fopen("FlagMatrix.txt","a");
	for(int h1=0;h1<M;h1++)
	{
		for(int h2=0;h2<N;h2++)
		{
			fprintf(fp,"%d",flag[h1][h2]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void OutputGreyValueRecordToTextfile()
{
	ClearTextfile("GreyValueRecord.txt");

	FILE *fp;
	fp = fopen("GreyValueRecord.txt","a");
	for(int h=0;h<ColorNum;h++)
	{
		fprintf(fp,"%d,",P[h].topleft);
		fprintf(fp,"%d,",P[h].topright);
		fprintf(fp,"%d,",P[h].bottomleft);
		fprintf(fp,"%d,",P[h].bottomright);
		fprintf(fp,"\n");
	}
	fclose(fp);
}

int NumberOfBits(int number)
{
	if( number>=128 && number<256 )
	{
		return 8;
	}
	else if( number>=64 && number<128 )
	{
		return 7;
	}
	else if( number>=32 && number<64 )
	{
		return 6;
	}
	else if( number>=16 && number<32 )
	{
		return 5;
	}
	else if( number>=8 && number<16 )
	{
		return 4;
	}
	else if( number>=4 && number<8 )
	{
		return 3;
	}
	else if( number>=2 && number<4 )
	{
		return 2;
	}
	else if( number>=1 && number<2 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DecimalToTenBinary(int decimal, char binary[10])
{
	int k =0;
	int BinaryNum = 10;
	for(int i=0;i<BinaryNum;i++)
	{
		binary[i]='0';
	}
	while(decimal!=0)
	{
		if(decimal%2 == 1)
		{
			binary[BinaryNum-k-1] = '1';
		}
		//else
		//{
		//	binary[BinaryNum-k-1] = '0';
		//}
		decimal = decimal/2;
		k++;
	}
}

int BinaryToDecimal(char binary[], int BinaryNum)
{
	int decimal = 0;
	for(int a=0 ; a<BinaryNum ; a++)
	{
		if(binary[a]=='1')
		{
			decimal = decimal + pow( 2.0, (BinaryNum-1-a));
		}
	}
	return decimal;
}

uchar GetGrayValue( int x, int y, IplImage *img)
{
	CvScalar g;
	g = cvGet2D(img, y, x);
	return (uchar)g.val[0];
}

void BlockRegeneration( int x1, int x2, int y1, int y2, uchar topleft, uchar topright, uchar bottomleft, uchar bottomright ,IplImage * img)
{
	double i1 = 0, i2 = 0;
	double g1,g2,g3;
	if((x1!=x2)&&(y1!=y2))
	{
		for(int y=y1;y<=y2;y++)
			for(int x=x1;x<=x2;x++)
			{
				i1=(double)(y-y1)/(y2-y1);
				i2=(double)(x-x1)/(x2-x1);
				g1=topleft+(topright-topleft)*i2;
				g2=bottomleft+(bottomright-bottomleft)*i2;
				g3=g1+(g2-g1)*i1;
				cvSet2D( img, y, x, cvScalar(g3));// g3会四舍五入
				//uchar* ptr = (uchar*) (img->imageData+y*img->widthStep);
				//ptr[x]=g3;//小数部分直接截取，而非四舍五入
			}
	}
	else if((x1==x2)&&(y1==y2))
	{
		cvSet2D( img, y1, x1, cvScalar(topleft));// g3会四舍五入
        //g3 = topleft;
		//uchar* ptr = (uchar*) (img->imageData+y1*img->widthStep);
		//ptr[x1]=g3;//小数部分直接截取，而非四舍五入
	}
	else if((x1==x2)&&(y1!=y2))
	{
		for(int y=y1;y<=y2;y++)
		{
			i1=(double)(y-y1)/(y2-y1);
			g3=topleft+(bottomright-topleft)*i1;
			cvSet2D( img, y, x1, cvScalar(g3));// g3会四舍五入
			//uchar* ptr = (uchar*) (img->imageData+y*img->widthStep);
			//ptr[x1]=g3;//小数部分直接截取，而非四舍五入
		}
	}
	else if((x1!=x2)&&(y1==y2))
	{
		for(int x=x1;x<=x2;x++)
		{
			i2=(double)(x-x1)/(x2-x1);
			g3=topleft+(bottomright-topleft)*i2;
			cvSet2D( img, y1, x, cvScalar(g3));// g3会四舍五入
            //uchar* ptr = (uchar*) (img->imageData+y1*img->widthStep);
			//ptr[x]=g3;//小数部分直接截取，而非四舍五入
		}
	}
	else
	{
	}
}
