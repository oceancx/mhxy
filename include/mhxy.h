//
//  mhxy
//
//  Created by oceancx on 16/8/15.
//  Copyright (c) 2016�� oceancx. All rights reserved.
//

#ifndef mhxy_h
#define mhxy_h

#include <stdint.h>
// #include<string>
// using namespace std;

// namespace XYQ
// {
//     string ROOT_DIR="./";
//     string WDF_DIR="./data/";
//     string MAP_DIR="./data/sence";
// }


//========= ������������ ==========
typedef int64_t     int64;
typedef int32_t     int32;
typedef int16_t     int16;
typedef int8_t      int8;

typedef uint64_t    uint64;
typedef uint32_t    uint32;
typedef uint16_t    uint16;
typedef uint8_t     uint8;

typedef float       float32;
typedef double      float64;
typedef long double float128;

typedef unsigned  char BYTE;
typedef unsigned  short WORD;
typedef unsigned  int DWORD;
//=================================

//�������ͼ�����ݽṹ
#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	int biWidth;
	int biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
}BITMAPINFO;

typedef struct BITMAP_FILE_TAG {
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	RGBQUAD palette[256];
	BYTE *buffer;
}BITMAP_FILE, *BITMAP_FILE_PTR;
#pragma pack()

#pragma pack(push) //�������״̬
#pragma pack(1)//�趨Ϊ4�ֽڶ���
// TGA �ļ�ͷ
struct TGA_FILE_HEADER
{
	uint8 IdLength;             // ͼ����Ϣ�ֶ�(Ĭ��:0)
	uint8 ColorMapType;         // ��ɫ�������(Ĭ��0)
	uint8 ImageType;            // ͼ��������(֧��2��10)
	uint16 ColorMapFirstIndex;  // ��ɫ�������(Ĭ��:0)
	uint16 ColorMapLength;      // ��ɫ��ĳ���(Ĭ��:0)
	uint8 ColorMapEntrySize;    // ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
	uint16 XOrigin;             // ͼ��X�������ʼλ��(Ĭ��:0)
	uint16 YOrigin;             // ͼ��Y�������ʼλ��(Ĭ��:0)
	uint16 ImageWidth;          // ͼ��Ŀ��
	uint16 ImageHeight;         // ͼ��ĸ߶�
	uint8 PixelDepth;           // ͼ��ÿ���ش洢ռ��λ��
	uint8 ImageDescruptor;      // ͼ�������ַ��ֽ�(Ĭ��:0����֧��16bit 565��ʽ)
};
#pragma pack(pop)//�ָ�����״̬



#endif
