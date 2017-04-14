#ifndef _MERGINGMETHOD_H_
#define _MERGINGMETHOD_H_

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include "commonfile.h"
#include "STCMethod.h"
#include <iostream>
using namespace std;

extern int reg_num;
extern int bitnum ;
extern int cur_block ;
extern double threshuod_U ;
extern double threshuod_Var ;

struct coordinate
{
	int x1;
	int y1;
	int x2;
	int y2;
};

struct Vertex
{
	int			x;
	int			y;
	Vertex*		Next;
};

struct Edge;
struct Region
{
	double		Mean;			//平均灰度值
	double		Var;			//灰度值方差
	int			Size;			//边长？？？？？？？？？？面积
	Region*		Father;
	int			Count;			//子孙区域数量
	int			SegmentCount;	//和waveform共享的边数
	Edge*		EdgeLink;
};

struct Edge
{
	Edge*		PreLink;	
	Vertex*		First;			//边的始点
	Vertex*		Last;			//边的终点
	Region*		Reg;			//所对应的区域
	Edge*		SucLink;
};

struct Segment
{
	Segment*	PreLink;
	int			Length;			//小段长度
    Edge*		ActiveELink;	//活动边界
	Segment*	SucLink;
};

void MergingMethod( IplImage* img, vector<uchar> &Linear_tree_table, vector<color> &Color_table, int Initial_Direction);
void Region_Segm( Segment* UpperLeft, Segment* &UpperRight, Segment* &PreLowerLeft, int Xleft , int Yupper , int width, int height, vector<uchar> &Linear_tree_table, vector<color> &Color_table, Region** all_region, int &direction);
void Left_Split( Segment* Left);
void Up_Split( Segment* Upper);
void Leaf_Operation( Segment* UpperLeft ,Segment* &UpperRight , Segment* &PreLowerLeft, int Xleft, int Yupper, int width, int height, vector<uchar> &Linear_tree_table, vector<color> &Color_table, Region** all_region);
Region* FindParent( Region* region);
void Coordinate_vector_Generation( node* Decoded_Node, vector<coordinate> &Coordinate);

inline double Gs_t( uchar g1, uchar g2, int w)		//上边缘单位距离灰度变化值
{
    return (g2-g1)/(w-1.0);
}

inline double Gs_b( uchar g3, uchar g4, int w)		//下边缘单位距离灰度变化值
{
    return (g4-g3)/(w-1.0);
}

inline double Gs_l( uchar g1, uchar g3, int h)		//左边缘单位距离灰度变化值
{
    return (g3-g1)/(h-1.0);
}

inline double Gs_r( uchar g2, uchar g4, int h)		//右边缘单位距离灰度变化值
{
    return (g4-g2)/(h-1.0);
}

inline double C_w( int w)
{
    return (2*w-1)/(6*(w-1.0));
}

inline double C_h( int h)
{
    return (2*h-1)/(6*(h-1.0));
}

//inline double D1( uchar g1, uchar g2, uchar g3, uchar g4, int w, int h)		//
//{
//    return (g4-g3-g2+g1)/((w-1.0)*(h-1.0));
//}

inline double Ubi( color Color, int x1, int y1, int x2, int y2)	//色块的平均灰度值
{
	double ubi;
	uchar g1, g2, g3, g4;
	g1 = Color.topleft;
	g2 = Color.topright;
	g3 = Color.bottomleft;
	g4 = Color.bottomright;

	if ( x1!=x2 && y1!=y2 )
	{
		ubi = (g1+g2+g3+g4)/4.0;
	}
	else if ( x1!=x2 && y1==y2 )
	{
    	ubi = (g1+g2)/2.0;
	}
	else if ( x1==x2 && y1!=y2 )
	{
    	ubi = (g1+g3)/2.0;
	}
	else if ( x1==x2 && y1==y2 )
	{
    	ubi = g1;
	}
	else
	{
	}
	
	return ubi;
}

inline double SquareSumbi( color Color, int x1, int y1, int x2, int y2)		//色块灰度值的平方和
{
	double squaresum;
	int w = x2-x1+1;
	int h = y2-y1+1;
	double cw = C_w(w);
	double ch = C_h(h);
	uchar g1, g2, g3, g4;
	g1 = Color.topleft;
	g2 = Color.topright;
	g3 = Color.bottomleft;
	g4 = Color.bottomright;

	if (x1!=x2 && y1!=y2)
	{
		squaresum = 1.0*w*h*(
				(0.5-cw-ch)*(g1*g4+g2*g3)
				+cw*(g2*g4+g1*g3)+ch*(g3*g4+g1*g2)
				+cw*ch*(g4-g3-g2+g1)*(g4-g3-g2+g1)
				);
	}
	else if ( x1!=x2 && y1==y2 )
	{
		squaresum = 1.0*w*(
				g1*g2
				+cw*(g2-g1)*(g2-g1)
				);
	}
	else if ( x1==x2 && y1!=y2 )
	{
		squaresum = 1.0*h*(
				g1*g3
				+ch*(g3-g1)*(g3-g1)
				);
	}
	else if ( x1==x2 && y1==y2 )
	{
		squaresum = 1.0*g1*g1;
	}
	else
	{
	}

	return squaresum;
}


inline double Varbi( color Color, int x1, int y1, int x2, int y2)		//色块灰度值的方差
{
	double varbi;
	int w = x2-x1+1;
	int h = y2-y1+1;
	double cw = C_w(w);
	double ch = C_h(h);
	uchar g1, g2, g3, g4;
	g1 = Color.topleft;
	g2 = Color.topright;
	g3 = Color.bottomleft;
	g4 = Color.bottomright;

	if (x1!=x2 && y1!=y2)
	{
		double ubi = Ubi( Color, x1, y1, x2, y2);
		varbi = (0.5-cw-ch)*(g1*g4+g2*g3)
				+cw*(g2*g4+g1*g3)+ch*(g3*g4+g1*g2)
				+cw*ch*(g4-g3-g2+g1)*(g4-g3-g2+g1)
				-ubi*ubi;
	}
	else if ( x1!=x2 && y1==y2 )
	{
		varbi = (cw-0.25)*(g2-g1)*(g2-g1);
	}
	else if ( x1==x2 && y1!=y2 )
	{
		varbi = (ch-0.25)*(g3-g1)*(g3-g1);
	}
	else if ( x1==x2 && y1==y2 )
	{
		varbi = 0;
	}
	else
	{
	}

	return varbi;
}

inline double Uc( int na, int nb, double ua, double ub)		//色块合并后的平均灰度值
{
	return (na*ua+nb*ub)/(na+nb);
}

inline double Varc( int na, int nb, double vara, double varb, double ua, double ub)		//色块合并后的灰度值的方差
{
	return (na*vara+nb*varb)/(na+nb) + (na*nb*(ua-ub)*(ua-ub))/((na+nb)*(na+nb));
}

inline char Get_bit( vector<uchar> &Linear_Tree_Table)		//从树的编码中提取一单位
{
	return Linear_Tree_Table[bitnum++];
}

#endif