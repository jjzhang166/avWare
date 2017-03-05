/***************************************************************************************
 *
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 *
 *  By downloading, copying, installing or using the software you agree to this license.
 *  If you do not agree to this license, do not download, install, 
 *  copy or use the software.
 *
 *  Copyright (C) 2010-2014, Happytimesoft Corporation, all rights reserved.
 *
 *  Redistribution and use in binary forms, with or without modification, are permitted.
 *
 *  Unless required by applicable law or agreed to in writing, software distributed 
 *  under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *  CONDITIONS OF ANY KIND, either express or implied. See the License for the specific
 *  language governing permissions and limitations under the License.
 *
****************************************************************************************/

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


