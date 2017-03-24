//
//  image.h
//  mhxy
//
//  Created by oceancx on 16/8/15.
//  Copyright (c) 2016�� oceancx. All rights reserved.
//

#ifndef mhxy_image_h
#define mhxy_image_h

//========= ������������ ==========
typedef long long			int64;
typedef unsigned long long	uint64;
typedef int					int32;
typedef unsigned int		uint32;
typedef short				int16;
typedef unsigned short		uint16;
typedef char				int8;
typedef unsigned char		uint8;
//=================================



typedef unsigned  char BYTE;
typedef unsigned  short WORD;
typedef unsigned  int DWORD;
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
	uint8 IdLength;				// ͼ����Ϣ�ֶ�(Ĭ��:0)
	uint8 ColorMapType;			// ��ɫ�������(Ĭ��0)
	uint8 ImageType;			// ͼ��������(֧��2��10)
	uint16 ColorMapFirstIndex;	// ��ɫ�������(Ĭ��:0)
	uint16 ColorMapLength;		// ��ɫ��ĳ���(Ĭ��:0)
	uint8 ColorMapEntrySize;	// ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
	uint16 XOrigin;				// ͼ��X�������ʼλ��(Ĭ��:0)
	uint16 YOrigin;				// ͼ��Y�������ʼλ��(Ĭ��:0)
	uint16 ImageWidth;			// ͼ��Ŀ��
	uint16 ImageHeight;			// ͼ��ĸ߶�
	uint8 PixelDepth;			// ͼ��ÿ���ش洢ռ��λ��
	uint8 ImageDescruptor;		// ͼ�������ַ��ֽ�(Ĭ��:0����֧��16bit 565��ʽ)
};
#pragma pack(pop)//�ָ�����״̬


#endif
