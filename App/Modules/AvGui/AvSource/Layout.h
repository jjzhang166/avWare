#ifndef _LAYOUT_H_
#define _LAYOUT_H_


#define LAYOUT_MODE_ONE			1
#define LAYOUT_MODE_FOUR		4
#define LAYOUT_MODE_SIX			6
#define LAYOUT_MODE_EIGHT		8
#define LAYOUT_MODE_NINE		9
#define LAYOUT_MODE_SIXTEEN		16
#define LAYOUT_MODE_TWENTYFIVE	25
#define LAYOUT_MODE_THIRTYSIZ	36
#define LAYOUT_MODE_SIZTYFOUR	64


typedef struct
{
 	int left; 	
 	int right;
 	int top;
 	int bottom;

 	QString filePath;
} LayoutZone;

typedef QList<LayoutZone> LayoutZoneList;

typedef struct 
{
    int rows;
    int cols;
    LayoutZoneList zones;
} Layout;


#endif


