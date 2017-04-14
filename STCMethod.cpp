#include "STCMethod.h"

vector<uchar> Linear_tree_table;
vector<color> Color_table;

int STCMethod_2(IplImage* img, IplImage* segmented_img, int Initial_Direction, double Threshold)
{
	node *STC_Node = new node;
	int k = 0;
	(*STC_Node).x1 = 0;
	(*STC_Node).y1 = 0;
	(*STC_Node).x2 = img->width-1;
	(*STC_Node).y2 = img->height-1;
	(*STC_Node).Direction = Initial_Direction;
	(*STC_Node).leftchild = NULL;
	(*STC_Node).rightchild = NULL;
	B_Tree_Generation( STC_Node, img, segmented_img, Threshold);
	B_Tree_Traversing( STC_Node, &k);
	Linear_tree_table_Generation( STC_Node, img);
	Table_Size();
	return k;
}

void B_Tree_Generation( node *STC_Node, IplImage* img, IplImage* segmented_img, double Threshold)
{
	if(judgement( (*STC_Node).x1, (*STC_Node).x2, (*STC_Node).y1, (*STC_Node).y2, img, Threshold))
	{
		node *L_Node = new node;
		node *R_Node = new node;
		(*STC_Node).leftchild = L_Node;
		(*STC_Node).rightchild = R_Node;
		if((*STC_Node).Direction == 0)  //沿水平分割
		{
			(*STC_Node).leftchild->x1			= (*STC_Node).x1;
			(*STC_Node).leftchild->y1			= (*STC_Node).y1;
			(*STC_Node).leftchild->x2			= (*STC_Node).x2;
			(*STC_Node).leftchild->y2			= ((*STC_Node).y1+(*STC_Node).y2)/2;
			(*STC_Node).leftchild->Direction	= 1;
			(*STC_Node).leftchild->leftchild	= NULL;
			(*STC_Node).leftchild->rightchild	= NULL;
			B_Tree_Generation( (*STC_Node).leftchild, img, segmented_img, Threshold);

			(*STC_Node).rightchild->x1			= (*STC_Node).x1;
			(*STC_Node).rightchild->y1			= ((*STC_Node).y1+(*STC_Node).y2)/2+1;
			(*STC_Node).rightchild->x2			= (*STC_Node).x2;
			(*STC_Node).rightchild->y2			= (*STC_Node).y2;
			(*STC_Node).rightchild->Direction	= 1;
			(*STC_Node).rightchild->leftchild	= NULL;
			(*STC_Node).rightchild->rightchild = NULL;
			B_Tree_Generation( (*STC_Node).rightchild, img, segmented_img, Threshold);
		}
		else if((*STC_Node).Direction == 1)  //沿竖直分割
		{
			(*STC_Node).leftchild->x1			= (*STC_Node).x1;
			(*STC_Node).leftchild->y1			= (*STC_Node).y1;
			(*STC_Node).leftchild->x2			= ((*STC_Node).x1+(*STC_Node).x2)/2;
			(*STC_Node).leftchild->y2			= (*STC_Node).y2;
			(*STC_Node).leftchild->Direction	= 0;
			(*STC_Node).leftchild->leftchild	= NULL;
			(*STC_Node).leftchild->rightchild	= NULL;
			B_Tree_Generation( (*STC_Node).leftchild, img, segmented_img, Threshold);

			(*STC_Node).rightchild->x1			= ((*STC_Node).x1+(*STC_Node).x2)/2+1;
			(*STC_Node).rightchild->y1			= (*STC_Node).y1;
			(*STC_Node).rightchild->x2			= (*STC_Node).x2;
			(*STC_Node).rightchild->y2			= (*STC_Node).y2;
			(*STC_Node).rightchild->Direction	= 0;
			(*STC_Node).rightchild->leftchild	= NULL;
			(*STC_Node).rightchild->rightchild = NULL;
			B_Tree_Generation( (*STC_Node).rightchild, img, segmented_img, Threshold);
		}
		else
		{
		}
	}
	else
	{
		segmentdisplay((*STC_Node).x1, (*STC_Node).x2, (*STC_Node).y1, (*STC_Node).y2, segmented_img);
		//printf("(%d,%d),(%d,%d)\n",(*STC_Node).x1, (*STC_Node).y1, (*STC_Node).x2, (*STC_Node).y2);
	}
}

//二叉树中序遍历，输出叶节点数
void B_Tree_Traversing( node *STC_Node, int *k)
{
	if( (*STC_Node).leftchild == NULL )
	{
		(*k)++;
	}
	else
	{
		B_Tree_Traversing( (*STC_Node).leftchild, k);
		B_Tree_Traversing( (*STC_Node).rightchild, k);
	}
}

void Linear_tree_table_Generation( node *STC_Node, IplImage *img)
{
	if((*STC_Node).leftchild==NULL)
	{
		Linear_tree_table.push_back(1);
		struct color thiscolor;
		thiscolor.topleft		= GetGrayValue( (*STC_Node).x1, (*STC_Node).y1, img);
		thiscolor.topright		= GetGrayValue( (*STC_Node).x2, (*STC_Node).y1, img);
		thiscolor.bottomleft	= GetGrayValue( (*STC_Node).x1, (*STC_Node).y2, img);
		thiscolor.bottomright	= GetGrayValue( (*STC_Node).x2, (*STC_Node).y2, img);
		Color_table.push_back(thiscolor);
	}
	else
	{
		Linear_tree_table.push_back(0);
		Linear_tree_table_Generation( (*STC_Node).leftchild, img);
		Linear_tree_table_Generation( (*STC_Node).rightchild, img);
	}
}

void Table_Size()
{
	cout<<"位置编码节点个数："<<Linear_tree_table.size()<<endl;
	cout<<"颜色编码节点个数："<<Color_table.size()<<endl;
}

node* Decoding(vector<uchar> &Linear_tree_table, vector<color> &Color_table, int Initial_Direction, IplImage *decoded_img, int width, int height)
{
	node *Decoded_Node = new node;
	int l = 0;
	int m = 0;
	(*Decoded_Node).x1 = 0;
	(*Decoded_Node).y1 = 0;
	(*Decoded_Node).x2 = width-1;
	(*Decoded_Node).y2 = height-1;
	(*Decoded_Node).Direction = Initial_Direction;
	(*Decoded_Node).leftchild = NULL;
	(*Decoded_Node).rightchild = NULL;
	
	B_Tree_ReGeneration( Decoded_Node, Linear_tree_table, l);
	return Decoded_Node;
	/*for(unsigned int i=0;i<Linear_tree_table.size();i++)
		cout<<Linear_tree_table.at(i);*/
	//cout<<"生成的树的节点数"<<*l<<endl;
	//Q_TreeTraversing( node *STC_Node, int *k);
	//Image_Regeneration( Decoded_Node, Color_table, m, decoded_img);//需生成恢复图像时使用
}

void B_Tree_ReGeneration( node *Decoded_Node, vector<uchar> &Linear_tree_table, int &l)
{
	if(l<Linear_tree_table.size())
	{
		if(Linear_tree_table.at(l)==0)
		{
			node *New_L_Node = new node;
			node *New_R_Node = new node;

			(*Decoded_Node).leftchild = New_L_Node;
			(*Decoded_Node).rightchild = New_R_Node;

			if((*Decoded_Node).Direction==0)
			{
				(*Decoded_Node).leftchild->x1			= (*Decoded_Node).x1;
				(*Decoded_Node).leftchild->y1			= (*Decoded_Node).y1;
				(*Decoded_Node).leftchild->x2			= (*Decoded_Node).x2;
				(*Decoded_Node).leftchild->y2			= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2;
				(*Decoded_Node).leftchild->Direction	= 1;
				(*Decoded_Node).leftchild->leftchild	= NULL;
				(*Decoded_Node).leftchild->rightchild	= NULL;
				l++;
				B_Tree_ReGeneration( (*Decoded_Node).leftchild, Linear_tree_table, l);

				(*Decoded_Node).rightchild->x1			= (*Decoded_Node).x1;
				(*Decoded_Node).rightchild->y1			= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2+1;
				(*Decoded_Node).rightchild->x2			= (*Decoded_Node).x2;
				(*Decoded_Node).rightchild->y2			= (*Decoded_Node).y2;
				(*Decoded_Node).rightchild->Direction	= 1;
				(*Decoded_Node).rightchild->leftchild	= NULL;
				(*Decoded_Node).rightchild->rightchild	= NULL;
				l++;
				B_Tree_ReGeneration( (*Decoded_Node).rightchild, Linear_tree_table, l);
			}
			else if((*Decoded_Node).Direction==1)
			{
				(*Decoded_Node).leftchild->x1			= (*Decoded_Node).x1;
				(*Decoded_Node).leftchild->y1			= (*Decoded_Node).y1;
				(*Decoded_Node).leftchild->x2			= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2;
				(*Decoded_Node).leftchild->y2			= (*Decoded_Node).y2;
				(*Decoded_Node).leftchild->Direction	= 0;
				(*Decoded_Node).leftchild->leftchild	= NULL;
				(*Decoded_Node).leftchild->rightchild	= NULL;
				l++;
				B_Tree_ReGeneration( (*Decoded_Node).leftchild, Linear_tree_table, l);

				(*Decoded_Node).rightchild->x1			= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2+1;
				(*Decoded_Node).rightchild->y1			= (*Decoded_Node).y1;
				(*Decoded_Node).rightchild->x2			= (*Decoded_Node).x2;
				(*Decoded_Node).rightchild->y2			= (*Decoded_Node).y2;
				(*Decoded_Node).rightchild->Direction	= 0;
				(*Decoded_Node).rightchild->leftchild	= NULL;
				(*Decoded_Node).rightchild->rightchild	= NULL;
				l++;
				B_Tree_ReGeneration( (*Decoded_Node).rightchild, Linear_tree_table, l);
			}
			else
			{
			}
		}
		else if(Linear_tree_table.at(l)==1)
		{
			//在恢复STC切割而非合并后图时，可以将下一个函数整合在这里
		}
		else
		{
		}
	}
}

void Image_Regeneration( node *Decoded_Node, vector<color> &Color_table, int &m, IplImage *decoded_img)
{
	if( (*Decoded_Node).leftchild == NULL )
	{
		int x1, y1, x2, y2;
		uchar c_topleft, c_topright, c_bottomleft, c_bottomright;
		x1 = (*Decoded_Node).x1;
		y1 = (*Decoded_Node).y1;
		x2 = (*Decoded_Node).x2;
		y2 = (*Decoded_Node).y2;
		c_topleft = Color_table.at(m).topleft;
		c_topright = Color_table.at(m).topright;
		c_bottomleft = Color_table.at(m).bottomleft;
		c_bottomright = Color_table.at(m).bottomright;
		BlockRegeneration( x1, x2, y1, y2, c_topleft, c_topright, c_bottomleft, c_bottomright ,decoded_img);
		m++;
	}
	else
	{
		Image_Regeneration( (*Decoded_Node).leftchild, Color_table, m, decoded_img);
		Image_Regeneration( (*Decoded_Node).rightchild, Color_table, m, decoded_img);
	}
}

double BPP( vector<uchar> &Linear_tree_table, vector<color> &Color_table ,int width , int height)  //计算BPP
{
	//double bpp = (8.0*sizeof(uchar)*Linear_tree_table.size()+8.0*4.0*sizeof(uchar)*Color_table.size())/(width*height);//畅彤
    double bpp = (Linear_tree_table.size()+32.0*Color_table.size())/(width*height);//郑运平
	//printf("%d,%d\n",sizeof(int),sizeof(double));
	return bpp;
}

double PSNR( IplImage* original_img, IplImage* decoded_img) //计算PSNR
{
	int height = original_img->height;
	int width = original_img->width;
	double temp=0;
	double psnr;
	CvScalar g1,g2;
	double g3,g4;

	for(int y=0;y<height;y++)
	{
		for (int x=0;x<width;x++)
		{
			g1 = cvGet2D( original_img, y, x);
			g2 = cvGet2D( decoded_img, y, x);
			g3 = g1.val[0];
			g4 = g2.val[0];
			temp = temp + pow((g3-g4),2);
		}
	}
	psnr =10*log10((255.0*255.0*width*height)/temp);
	
	return psnr;
}

double CR( IplImage* original_img, vector<uchar> &Linear_tree_table, vector<color> &Color_table)
{
	
	double img_size;
	//double code_size;
	double cr;

	img_size = (original_img->width)*(original_img->height)*1.0;
	//code_size = 1.0*sizeof(uchar)*Linear_tree_table.size()+4.0*sizeof(uchar)*Color_table.size();
	//cr = img_size/code_size;
    cr=8.0/((Linear_tree_table.size()+32.0*Color_table.size())/(img_size));//郑运平

	return cr;
}
