#ifndef WAS_H
#define WAS_H 
#include "mhxy.h"
#include <string>
#include <fstream>
#include <memory.h>
using namespace std;
class WAS
{
public:

	// 精灵动画的文件头
	struct Header
	{
		uint16 flag;		// 精灵文件标志 SP 0x5053
		uint16 len;		// 文件头的长度 默认为 12
		uint16 group;		// 精灵图片的组数，即方向数
		uint16 frame;		// 每组的图片数，即帧数
		uint16 width;		// 精灵动画的宽度，单位像素
		uint16 height;		// 精灵动画的高度，单位像素
		uint16 key_x;		// 精灵动画的关键位X
		uint16 key_y;		// 精灵动画的关键位Y
	};

	// 帧的文件头
	struct FrameHeader
	{
		int key_x;			// 图片的关键位X
		int key_y;			// 图片的关键位Y
		int width;			// 图片的宽度，单位像素
		int height;			// 图片的高度，单位像素
	};


	WAS(std::string filename, int offset, int size);
	WAS(fstream &infile, int offset, int size);
	static uint32 RGB565to888(uint16 color, uint8 alpha); // 565转888
	static uint16 Alpha565(uint16 src, uint16 des, uint8 alpha);
	static uint8 MixAlpha(uint8 color, uint8 alpha);
	~WAS();
	Header mHeader;
	uint32 mPalette32[256];
	uint32* mFrameIndecies;
	std::string mFileName;
	int mFileOffset;
	int mFileSize;

};
#endif