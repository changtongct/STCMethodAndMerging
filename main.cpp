#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <time.h>
#include "commonfile.h"
#include "STCMethod.h"
#include "MergingMethod.h"

//int flag[M][N]={0};

int main(int argc,char** argv)//分别输入1.原灰度图 2.转灰度图文件名 3.解码后的文件名 4.EPSILON 5.先y方向后x方向切割?

{
	IplImage* img;
	if( argc==6 && (img=cvLoadImage( argv[1], 0))!=0 )   //将源彩色图像img转化成目标灰色图像读入
	{
		cout << "图片已被正确读取，参数数量也正确！" << endl;

		int k=0;//分割块的数量
		unsigned int time1,time2;
		const char* original_img = argv[1];
		//////////////////////////////////////////显示原始图像
		IplImage* colorimg = cvLoadImage(original_img);
		cvNamedWindow("原图",CV_WINDOW_AUTOSIZE);
		cvShowImage("原图",colorimg);
		//////////////////////////////////////////显示灰度图像并保存
		IplImage* grayimg = cvCreateImage(cvGetSize(colorimg), IPL_DEPTH_8U, 1);
		cvCvtColor(colorimg,grayimg,CV_BGR2GRAY);
		cvNamedWindow("灰度图",CV_WINDOW_AUTOSIZE);
		cvShowImage("灰度图",grayimg);
		const char* gray_img = argv[2];
		cvSaveImage(gray_img,grayimg);

		IplImage* segmented_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		cvSet(segmented_img,cvScalar(255));
		IplImage* decoded_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		double Threshold = atof(argv[4]);
		printf("分割阈值：%f\n",Threshold);
		int Initial_Direction = atoi(argv[5]);
		time1 = clock();
		k = STCMethod_2( grayimg, segmented_img, Initial_Direction, Threshold);//调用STC图像分割函数
		//Decoding(Linear_tree_table, Color_table, Initial_Direction, decoded_img, decoded_img->width, decoded_img->height);
		MergingMethod( grayimg, Linear_tree_table, Color_table, Initial_Direction);
		time2 = clock();

		const char* d_img = argv[3];
		cvSaveImage(d_img,decoded_img);
		printf("花费时间: %u ms\n",time2-time1);//显示分割过程花费的时间
		printf("分割块数%d\n",k);				//显示分割块数
		double bpp;
		bpp = BPP( Linear_tree_table, Color_table, decoded_img->width, decoded_img->height);
		printf("像素深度BPP：%f 位\n",bpp);
		double psnr;
		psnr = PSNR( grayimg ,decoded_img);
		printf("峰值信噪比PSNR：%f db\n",psnr);
		double cr;
		cr = CR( grayimg, Linear_tree_table, Color_table);
		printf("图像压缩比CR：%f\n",cr);
		cvNamedWindow("图切割结果",CV_WINDOW_AUTOSIZE);
		cvShowImage("图切割结果",segmented_img);	
		cvNamedWindow("解码恢复后的图",CV_WINDOW_AUTOSIZE);
		cvShowImage("解码恢复后的图",decoded_img);	

		//IplImage* decoded_imgXXX = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		//DecodingXXX(Linear_tree_table, Color_table, Initial_Direction, decoded_imgXXX, decoded_img->width, decoded_img->height);
		//int h=0;
		//for(int y=0;y<512;y++)
		//	for(int x=0;x<512;x++)
		//	{
		//		CvScalar a,b;
		//		a=cvGet2D(decoded_img,y,x);
		//		b=cvGet2D(decoded_imgXXX,y,x);
		//		if(a.val[0]==b.val[0])
		//		{
		//			printf("%d\n",h++);
		//			//printf("%f , %f\n",a.val[0],b.val[0]);
		//		}
		//	}

		cvWaitKey(0);
		cvReleaseImage(&colorimg);
		cvReleaseImage(&grayimg);
		cvReleaseImage(&segmented_img);
		cvReleaseImage(&decoded_img);
		cvDestroyAllWindows();
		
		return 0;
	}
	else
	{
		cout << "图片不存在或参数不正确！"<< endl;
		return -1;
	}
}
