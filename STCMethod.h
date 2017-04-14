#ifndef _STCMETHOD_H_
#define _STCMETHOD_H_

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include "commonfile.h"
#include <iostream>
using namespace std;

struct node
{
	int x1;
	int y1;
	int x2;
	int y2;
	int Direction;
	node* leftchild;
	node* rightchild;
};
struct color
{
	uchar topleft;
	uchar topright;
	uchar bottomleft;
	uchar bottomright;
};
extern vector<uchar> Linear_tree_table;
extern vector<color> Color_table;

int STCMethod_2( IplImage* img, IplImage* segmented_img, int Initial_Direction, double Threshold);
void B_Tree_Generation( node *STC_Node, IplImage* img, IplImage* segmented_img, double Threshold);
void B_Tree_Traversing( node *STC_Node, int *k);
void Linear_tree_table_Generation( node *STC_Node, IplImage *img);
void Table_Size();
node* Decoding(vector<uchar> &Linear_tree_table, vector<color> &Color_table, int Initial_Direction, IplImage *decoded_img, int width, int height);
void B_Tree_ReGeneration( node *Decoded_Node, vector<uchar> &Linear_tree_table, int &l);
void Image_Regeneration( node *Decoded_Node, vector<color> &Color_table, int &m, IplImage *decoded_img);

double BPP( vector<uchar> &Linear_tree_table, vector<color> &Color_table , int width, int height);
double PSNR( IplImage* original_img, IplImage* decoded_img);
double CR( IplImage* original_img, vector<uchar> &Linear_tree_table, vector<color> &Color_table);

#endif
