#include "MergingMethod.h"

vector<coordinate> Coordinate;
vector<Region> Merged_Region;
int reg_num = 0;
int bitnum = 0;
int cur_block = -1;
double threshuod_U = 0;
double threshuod_Var = 0;

void MergingMethod( IplImage* img, vector<uchar> &Linear_tree_table, vector<color> &Color_table, int Initial_Direction)
{
	int direction = Initial_Direction;
	Segment* UpperLeft = new Segment;
	Segment* Upper = new Segment;
	Segment* UpperRight = NULL;
	Segment* PreLowerLeft = NULL;

	UpperLeft->PreLink = NULL;
	UpperLeft->Length = img->height;
	UpperLeft->ActiveELink = NULL;
	UpperLeft->SucLink = Upper;

	Upper->PreLink = UpperLeft;
	Upper->Length = img->width;
	Upper->ActiveELink = NULL;
	Upper->SucLink = NULL;

	node* STC_Root = new node;
	STC_Root = Decoding( Linear_tree_table, Color_table, Initial_Direction, NULL, img->width, img->height);
	Coordinate_vector_Generation( STC_Root, Coordinate);
	Region** all_region = new Region*[Coordinate.size()];

	//for(int x=0;x<20;x++)		//������ֵ���֮��
	//	printf("%d\n",Coordinate.size());

	Region_Segm( UpperLeft, UpperRight, PreLowerLeft, 0, 0, img->width, img->height, Linear_tree_table, Color_table, all_region, direction);
	printf("( %d )\n",reg_num);
}

void Region_Segm( Segment* UpperLeft, Segment* &UpperRight, Segment* &PreLowerLeft, int Xleft, int Yupper , int width, int height, vector<uchar> &Linear_tree_table, vector<color> &Color_table, Region** all_region, int &direction)
{
	Segment* UR = NULL;///////////
	Segment* PLL = NULL;
	Segment* Useless = NULL;
	char bit;///////////////////
	printf("( %d )\n",direction);
	if (UpperLeft->Length > height)
	{
		Left_Split( UpperLeft);
	}
	if (UpperLeft->SucLink->Length > width)
	{
		Up_Split( UpperLeft->SucLink);
	}

	bit = Get_bit(Linear_tree_table);/////////////////
	
	if ( bit==0 )
	{
		if( direction%2 == 1 )
		{
			direction++;
			if( width%2==0 )
			{
				Region_Segm( UpperLeft, UR		   , PreLowerLeft	, Xleft			 , Yupper			, width/2	, height	, Linear_tree_table, Color_table, all_region, direction);
				Region_Segm( UR		  , UpperRight , Useless		, Xleft+width/2	 , Yupper			, width/2	, height	, Linear_tree_table, Color_table, all_region, direction);
			}
			else
			{
				Region_Segm( UpperLeft, UR		   , PreLowerLeft	, Xleft			 , Yupper			, width/2+1	, height	, Linear_tree_table, Color_table, all_region, direction);
				Region_Segm( UR		  , UpperRight , Useless		, Xleft+width/2+1, Yupper			, width/2	, height	, Linear_tree_table, Color_table, all_region, direction);
			}
		}
		else if( direction%2 == 0 )
		{
			direction++;
			if( height%2==0 )
			{
				Region_Segm( UpperLeft, UpperRight , PLL			, Xleft			 , Yupper			, width		, height/2	, Linear_tree_table, Color_table, all_region, direction);
				Region_Segm( PLL	  , Useless	   , PreLowerLeft	, Xleft			 , Yupper+height/2	, width		, height/2	, Linear_tree_table, Color_table, all_region, direction);
			}
			else
			{
				Region_Segm( UpperLeft, UpperRight , PLL			, Xleft			 , Yupper			, width		, height/2+1, Linear_tree_table, Color_table, all_region, direction);
				Region_Segm( PLL	  , Useless	   , PreLowerLeft	, Xleft			 , Yupper+height/2+1, width		, height/2  , Linear_tree_table, Color_table, all_region, direction);
			}
		}
		else
		{
		}
	}
	else
	{
		cur_block++;
		Leaf_Operation( UpperLeft, UpperRight, PreLowerLeft, Xleft, Yupper, width, height, Linear_tree_table, Color_table, all_region);
	}/////////////////////////////////
}

void Leaf_Operation( Segment* UpperLeft ,Segment* &UpperRight , Segment* &PreLowerLeft, int Xleft, int Yupper, int width, int height, vector<uchar> &Linear_tree_table, vector<color> &Color_table, Region** all_region)
{
	int length = height;
	Segment* start_segment = UpperLeft;
	while ( (length = height-start_segment->Length) != 0 )		//Ѱ��waveform���账�����ʼ����
	{
		start_segment = start_segment->PreLink;
	}
	PreLowerLeft = start_segment->PreLink;		//Ѱ�ҵ���ֵ��PLL���Ա�֮��ʹ��
	//length = 0;

	Region* current_region = new Region;
	all_region[cur_block] = current_region;////////////////

	////////////////////////////////////////////////////////////////////
	/*���������㣬Ȼ������һ�𣬱�ʾһ��Ԥ��������¾��ο��inactive��*/
	Vertex* edge_topleft	= new Vertex;
	Vertex* edge_topright	= new Vertex;
	Vertex* edge_bottomleft	= new Vertex;

	edge_topleft->x			= Xleft;
	edge_topleft->y			= Yupper;
	edge_topleft->Next		= edge_topright;

	edge_topright->x		= Xleft + width - 1;
	edge_topright->y		= Yupper;
	edge_topright->Next		= NULL;

	edge_bottomleft->x		= Xleft;
	edge_bottomleft->y		= Yupper + height - 1;
	edge_bottomleft->Next	= edge_topleft;
	////////////////////////////////////////////////////////////////////
	/*���������ߣ���ʾԤ��������¾��ο��һ��inactive�ߺ�����active�ߣ�
	����������β���*/
	Edge* active_right		= new Edge;
	Edge* active_down		= new Edge;
	Edge* inactive			= new Edge;

	active_right->PreLink	= inactive;
	active_right->First		= NULL;
	active_right->Last		= NULL;
	active_right->Reg		= current_region;
	active_right->SucLink	= active_down;
	
	active_down->PreLink	= active_right;
	active_down->First		= NULL;
	active_down->Last		= NULL;
	active_down->Reg		= current_region;
	active_down->SucLink	= inactive;
	
	inactive->PreLink		= active_down;
	inactive->First			= edge_bottomleft;
	inactive->Last			= edge_topright;
	inactive->Reg			= NULL;
	inactive->SucLink		= active_right;
	////////////////////////////////////////////////////////////////////
	/*���ñ�����ز���*/
	current_region->Mean		 = Ubi(Color_table.at(1),Coordinate.at(1).x1,Coordinate.at(1).y1,Coordinate.at(1).x2,Coordinate.at(1).y2);
	current_region->Var			 = Varbi(Color_table.at(1),Coordinate.at(1).x1,Coordinate.at(1).y1,Coordinate.at(1).x2,Coordinate.at(1).y2);
	current_region->Size		 = width*height;
	current_region->Father		 = NULL;
	current_region->Count		 = 0;
	current_region->SegmentCount = 2;
	current_region->EdgeLink	 = inactive;
	////////////////////////////////////////////////////////////////////
	//reg_num++;////////////////
	while(1)
	{
		if( (start_segment->ActiveELink!=NULL) && (start_segment->ActiveELink->Reg!=NULL) )
		{
			Region* neighbour_region = start_segment->ActiveELink->Reg;
			Region* neighbour_father = FindParent(neighbour_region);
			Region* current_father = FindParent(current_region);
			if(neighbour_father == current_father)
			{
			}
			else
			{
				double merged_region_var = Varc( current_region->Size, neighbour_father->Size, current_region->Var, neighbour_father->Var, neighbour_father->Mean, current_region->Mean);
				if( abs((current_region->Mean)-(neighbour_father->Mean))<=threshuod_U && merged_region_var<=threshuod_Var)		//���Ժϲ�
				{
					neighbour_father->Mean = Uc( current_region->Size, neighbour_father->Size, current_region->Mean, neighbour_father->Mean);
					neighbour_father->Size = neighbour_father->Size + current_region->Size;
					neighbour_father->Var  = merged_region_var;
					current_region->Father = neighbour_father;
					//reg_num--;///////////////////////
				}
				else	//���ܺϲ�
				{
				}
			}
		}
		if( (length = length + start_segment->Length) != (width+height) )
		{
			start_segment = start_segment->SucLink;
		}
		else
		{
			break;
		}
	}

	//�˿鴦����ɣ��޸�waveform������״̬

	Segment* down_segment = new Segment;
	Segment* right_segment = new Segment;

	down_segment->PreLink = PreLowerLeft;
	down_segment->Length = width;
	down_segment->ActiveELink = active_down;
	down_segment->SucLink = right_segment;
	if( PreLowerLeft != NULL )
	{
		PreLowerLeft->SucLink = down_segment;
	}

	right_segment->PreLink = down_segment;
	right_segment->Length = height;
	right_segment->ActiveELink = active_right;
	if( start_segment->SucLink != NULL )
	{
		right_segment->SucLink = start_segment->SucLink;
		start_segment->SucLink->PreLink = right_segment;
	}
	else
	{
		right_segment->SucLink = NULL;
	}
	UpperRight = right_segment;
}
//------------------------------------------------------------------
//������ı߷ָż�����֣�����ǰ�̺�
//------------------------------------------------------------------
void Left_Split( Segment* Left)
{
	Segment* firsthalf = new Segment;

	firsthalf->PreLink		= Left->PreLink;
	firsthalf->Length		= Left->Length/2;
	firsthalf->ActiveELink	= Left->ActiveELink;
	firsthalf->SucLink		= Left;

	Left->PreLink = firsthalf;
	if(Left->Length%2 == 1)
	{
		Left->Length = firsthalf->Length + 1;
	}
	else
	{
		Left->Length = firsthalf->Length;
	}

	if(firsthalf->PreLink != NULL)
	{
		firsthalf->PreLink->SucLink = firsthalf;
	}
}
//------------------------------------------------------------------
//�����ϵı߷ָż�����֣�����ǰ�����
//------------------------------------------------------------------
void Up_Split( Segment* Upper)
{
	Segment* firsthalf		= new Segment;

	firsthalf->PreLink		= Upper->PreLink;
	firsthalf->Length		= Upper->Length/2;
	firsthalf->ActiveELink	= Upper->ActiveELink;
	firsthalf->SucLink		= Upper;

	Upper->PreLink = firsthalf;
	if(Upper->Length%2 == 1)
	{
		Upper->Length = firsthalf->Length;
		( firsthalf->Length )++;
	}
	else
	{
		Upper->Length = firsthalf->Length;
	}

	firsthalf->PreLink->SucLink = firsthalf;
}

Region* FindParent( Region* region)
{
	while(region->Father!=NULL)
	{
		region = region->Father;
	}
	return region;
}

void Coordinate_vector_Generation( node* Decoded_Node, vector<coordinate> &Coordinate)
{
	if(Decoded_Node->leftchild == NULL)
	{
		struct coordinate coor;
		coor.x1 = Decoded_Node->x1;
		coor.y1 = Decoded_Node->y1;
		coor.x2 = Decoded_Node->x2;
		coor.y2 = Decoded_Node->y2;
		Coordinate.push_back(coor);
	}
	else
	{
		Coordinate_vector_Generation( Decoded_Node->leftchild, Coordinate);
		Coordinate_vector_Generation( Decoded_Node->rightchild, Coordinate);
	}
}