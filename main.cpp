#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <time.h>
#include "commonfile.h"
#include "STCMethod.h"
#include "MergingMethod.h"

//int flag[M][N]={0};

int main(int argc,char** argv)//�ֱ�����1.ԭ�Ҷ�ͼ 2.ת�Ҷ�ͼ�ļ��� 3.�������ļ��� 4.EPSILON 5.��y�����x�����и�?

{
	IplImage* img;
	if( argc==6 && (img=cvLoadImage( argv[1], 0))!=0 )   //��Դ��ɫͼ��imgת����Ŀ���ɫͼ�����
	{
		cout << "ͼƬ�ѱ���ȷ��ȡ����������Ҳ��ȷ��" << endl;

		int k=0;//�ָ�������
		unsigned int time1,time2;
		const char* original_img = argv[1];
		//////////////////////////////////////////��ʾԭʼͼ��
		IplImage* colorimg = cvLoadImage(original_img);
		cvNamedWindow("ԭͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("ԭͼ",colorimg);
		//////////////////////////////////////////��ʾ�Ҷ�ͼ�񲢱���
		IplImage* grayimg = cvCreateImage(cvGetSize(colorimg), IPL_DEPTH_8U, 1);
		cvCvtColor(colorimg,grayimg,CV_BGR2GRAY);
		cvNamedWindow("�Ҷ�ͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("�Ҷ�ͼ",grayimg);
		const char* gray_img = argv[2];
		cvSaveImage(gray_img,grayimg);

		IplImage* segmented_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		cvSet(segmented_img,cvScalar(255));
		IplImage* decoded_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		double Threshold = atof(argv[4]);
		printf("�ָ���ֵ��%f\n",Threshold);
		int Initial_Direction = atoi(argv[5]);
		time1 = clock();
		k = STCMethod_2( grayimg, segmented_img, Initial_Direction, Threshold);//����STCͼ��ָ��
		//Decoding(Linear_tree_table, Color_table, Initial_Direction, decoded_img, decoded_img->width, decoded_img->height);
		MergingMethod( grayimg, Linear_tree_table, Color_table, Initial_Direction);
		time2 = clock();

		const char* d_img = argv[3];
		cvSaveImage(d_img,decoded_img);
		printf("����ʱ��: %u ms\n",time2-time1);//��ʾ�ָ���̻��ѵ�ʱ��
		printf("�ָ����%d\n",k);				//��ʾ�ָ����
		double bpp;
		bpp = BPP( Linear_tree_table, Color_table, decoded_img->width, decoded_img->height);
		printf("�������BPP��%f λ\n",bpp);
		double psnr;
		psnr = PSNR( grayimg ,decoded_img);
		printf("��ֵ�����PSNR��%f db\n",psnr);
		double cr;
		cr = CR( grayimg, Linear_tree_table, Color_table);
		printf("ͼ��ѹ����CR��%f\n",cr);
		cvNamedWindow("ͼ�и���",CV_WINDOW_AUTOSIZE);
		cvShowImage("ͼ�и���",segmented_img);	
		cvNamedWindow("����ָ����ͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("����ָ����ͼ",decoded_img);	

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
		cout << "ͼƬ�����ڻ��������ȷ��"<< endl;
		return -1;
	}
}
