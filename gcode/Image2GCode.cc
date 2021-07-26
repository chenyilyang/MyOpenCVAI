#include <opencv2/opencv.hpp>
#include <iostream>

bool isG0Mode = true;
uint32_t StringLen = 0;
float quality;
uchar* gData;
char x_ValueString[10], y_ValueString[10], s_ValueString[10];
volatile uint16_t yLast = 0xFFFF;
#define RESOURCE_PATH = "/home/chenyiliang/OpenCV/MyOpenCVAI/resources"
#define OUTPUT_PATH = "/home/chenyiliang/OpenCV/MyOpenCVAI/resources"
float matrix[3][4]{
	{0, 0, 1.0 / 8, 1.0 / 8},
	{1.0 / 8, 1.0 / 8, 1.0 / 8, 0},
	{0, 1.0 / 8, 0, 0}
};

void image_atkinson(Mat &srcImg)
{
	cv::Mat img;
	cv::cvtColor(srcImg, img, COLOR_GRAY2BGRA);
	const int matrixHeight = 3;
	const int matrixWidth = 4;
	int matrixOffset = 0;
	for (int k = 1; k < matrixWidth; k++) {
		if (matrix[0][k] > 0.0) {
			matrixOffset = k - 1;
			break;
		}
	}

	double f_data = 0;
	uchar *pimgdata = img.ptr<uchar>(0);

	for (int y = 0; y < img.rows; y++) {
		const int reverse = (y & 1);
		for (int x = reverse ? img.cols - 1 : 0; reverse ? x >= 0 : x < img.cols; reverse ? x-- : x++) {
			const int index = (y * img.cols + x) << 2;
			const int origin = pimgdata[index];

			pimgdata[index] = origin > 127 ? 255 : 0;
			pimgdata[index + 1] = pimgdata[index];
			pimgdata[index + 2] = pimgdata[index];
			const int err = origin - pimgdata[index];

			for (int i = 0; i < matrixWidth; i++) {
				for (int j = 0; j < matrixHeight; j++) {
					if (matrix[j][i] > 0) {
						const int x2 = reverse ? x - (i - matrixOffset) : x + (i - matrixOffset);
						const int y2 = y + j;
						if (x2 >= 0 && x2 < img.cols && y2 < img.rows) {
							const int idx2 = index + ((x2 - x) << 2) + (y2 - y) * (img.cols << 2);
							f_data = matrix[j][i] * err + pimgdata[idx2];
							f_data = (f_data < 0) ? 0 : ((f_data > 255) ? 255 : f_data);
							pimgdata[idx2] = (f_data + 0.5);
						}
					}
				}
			}
		}
	}

	cv::cvtColor(img, srcImg, COLOR_BGRA2GRAY);
}

uint32_t float2string(uint32_t f, uint32_t p, char* s)
{
	uint32_t fi = p * f / quality + 0.5;
	memset(s, 0, 8);

	if (fi == 0)
	{
		s[0] = '0';
		return 1;
	}

	while (p > 1)
	{
		if (fi % 10)
		{
			break;
		}
		fi /= 10;
		p /= 10;
	}

	if (fi >= 100000)
	{
		s[0] = fi / 100000 + 0x30;
		s[1] = fi / 10000 % 10 + 0x30;
		s[2] = fi / 1000 % 10 + 0x30;
		s[3] = '.';
		s[4] = fi / 100 % 10 + 0x30;
		s[5] = fi / 10 % 10 + 0x30;
		s[6] = fi % 10 + 0x30;
		return 7;
	}
	else if (fi >= 10000)
	{
		if (p == 1000)
		{
			s[0] = fi / 10000 + 0x30;
			s[1] = fi / 1000 % 10 + 0x30;
			s[2] = '.';
			s[3] = fi / 100 % 10 + 0x30;
			s[4] = fi / 10 % 10 + 0x30;
			s[5] = fi % 10 + 0x30;
			return 6;
		}
		else
		{
			s[0] = fi / 10000 + 0x30;
			s[1] = fi / 1000 % 10 + 0x30;
			s[2] = fi / 100 % 10 + 0x30;
			s[3] = '.';
			s[4] = fi / 10 % 10 + 0x30;
			s[5] = fi % 10 + 0x30;
			return 6;
		}
	}
	else if (fi >= 1000)
	{
		if (p == 1000)
		{
			s[0] = fi / 1000 + 0x30;
			s[1] = '.';
			s[2] = fi / 100 % 10 + 0x30;
			s[3] = fi / 10 % 10 + 0x30;
			s[4] = fi % 10 + 0x30;
			return 5;
		}
		else if (p == 100)
		{
			s[0] = fi / 1000 + 0x30;
			s[1] = fi / 100 % 10 + 0x30;
			s[2] = '.';
			s[3] = fi / 10 % 10 + 0x30;
			s[4] = fi % 10 + 0x30;
			return 5;
		}
		else
		{
			s[0] = fi / 1000 + 0x30;
			s[1] = fi / 100 % 10 + 0x30;
			s[2] = fi / 10 % 10 + 0x30;
			s[3] = '.';
			s[4] = fi % 10 + 0x30;
			return 5;
		}
	}
	else if (fi >= 100)
	{
		if (p == 1000)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = fi / 100 + 0x30;
			s[3] = fi / 10 % 10 + 0x30;
			s[4] = fi % 10 + 0x30;
			return 5;
		}
		else if (p == 100)
		{
			s[0] = fi / 100 + 0x30;
			s[1] = '.';
			s[2] = fi / 10 % 10 + 0x30;
			s[3] = fi % 10 + 0x30;
			return 4;
		}
		else if (p == 10)
		{
			s[0] = fi / 100 + 0x30;
			s[1] = fi / 10 % 10 + 0x30;
			s[2] = '.';
			s[3] = fi % 10 + 0x30;
			return 4;
		}
		else
		{
			s[0] = fi / 100 + 0x30;
			s[1] = fi / 10 % 10 + 0x30;
			s[2] = fi % 10 + 0x30;
			return 3;
		}
	}
	else if (fi >= 10)
	{
		if (p == 1000)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = '0';
			s[3] = fi / 10 + 0x30;
			s[4] = fi % 10 + 0x30;
			return 5;
		}
		else if (p == 100)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = fi / 10 + 0x30;
			s[3] = fi % 10 + 0x30;
			return 4;
		}
		else if (p == 10)
		{
			s[0] = fi / 10 + 0x30;
			s[1] = '.';
			s[2] = fi % 10 + 0x30;
			return 3;
		}
		else
		{
			s[0] = fi / 10 + 0x30;
			s[1] = fi % 10 + 0x30;
			return 2;
		}
	}
	else
	{
		if (p == 1000)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = '0';
			s[3] = '0';
			s[4] = fi + 0x30;
			return 5;
		}
		else if (p == 100)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = '0';
			s[3] = fi + 0x30;
			return 4;
		}
		else if (p == 10)
		{
			s[0] = '0';
			s[1] = '.';
			s[2] = fi + 0x30;
			return 3;
		}
		else
		{
			s[0] = fi + 0x30;
			return 1;
		}
	}
}

uint32_t int2string(uint32_t d, char* s)
{
	memset(s, 0, 5);

	if (d >= 1000)
	{
		s[0] = d / 1000 + 0x30;
		s[1] = d / 100 % 10 + 0x30;
		s[2] = d / 10 % 10 + 0x30;
		s[3] = d % 10 + 0x30;
		return 4;
	}
	else if (d >= 100)
	{
		s[0] = d / 100 + 0x30;
		s[1] = d / 10 % 10 + 0x30;
		s[2] = d % 10 + 0x30;
		return 3;
	}
	else if (d >= 10)
	{
		s[0] = d / 10 + 0x30;
		s[1] = d % 10 + 0x30;
		return 2;
	}
	else
	{
		s[0] = d + 0x30;
		return 1;
	}
}

int string2int(char* str, int len)
{
	if (len == 1)
	{
		return str[0] - 0x30;
	}
	else if (len == 2)
	{
		return (str[0] - 0x30) * 10 + str[1] - 0x30;
	}
	else if (len == 3)
	{
		return (str[0] - 0x30) * 100 + (str[1] - 0x30) * 10 + str[2] - 0x30;
	}
	else
	{
		return 0;
	}
}

float string2float(char* str, int len)
{
	if (len == 1)
	{
		return (str[0] - 0x30) * 0.1;
	}
	else if (len == 2)
	{
		return (str[0] - 0x30) * 0.1 + (str[1] - 0x30) * 0.01;
	}
	else if (len == 3)
	{
		return (str[0] - 0x30) * 0.1 + (str[1] - 0x30) * 0.01 + (str[2] - 0x30) * 0.001;
	}
	else
	{
		return 0;
	}
}

void WriteGcodeString(uint32_t x, uint32_t y, uint32_t s)
{
	if (s != 0)
	{
		if (isG0Mode)
		{
			memcpy(gData, "G1 ", 3);
			gData += 3;
			isG0Mode = false;
		}
		memcpy(gData, "X", 1);
		gData += 1;
		StringLen = float2string(x, 1000, x_ValueString);
		memcpy(gData, x_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " S", 2);
		gData += 2;
		StringLen = int2string(s, s_ValueString);
		memcpy(gData, s_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, "\n", 1);
		gData += 1;
	}
	else
	{
		isG0Mode = true;
		memcpy(gData, "G0 X", 4);
		gData += 4;
		StringLen = float2string(x, 1000, x_ValueString);
		memcpy(gData, x_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " Y", 2);
		gData += 2;
		StringLen = float2string(y, 1000, y_ValueString);
		memcpy(gData, y_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " S0\n", 4);
		gData += 4;
	}
}

void GcodeCompress(char* gCodeBuffer, uint8_t* dataCompress, uint16_t sValue, uint16_t speed, bool isHead)
{
	uint8_t numLen = 0;
	uint16_t x, y, xf, yf;
	uint8_t* head = dataCompress;
  
	if (isHead) {

		*dataCompress++ = 0xEE;
		*dataCompress++ = 0xFF;

		*dataCompress++ = sValue;
		*dataCompress++ = sValue >> 8;

		*dataCompress++ = speed;
		*dataCompress++ = speed >> 8;

		char* fid = new char[80]();
		char* fidHead = fid;
		int idMax = quality - 1;
		for (int i = 0; i < idMax; i++)
		{
			fid[0] = float2string(i + 1, 1000, x_ValueString) - 2;
			memcpy(&fid[1], &x_ValueString[2], fid[0]);
			fid += 4;
		}

		memcpy(dataCompress, fidHead, 80);
		dataCompress += 80;
		dataCompress += 42;
	}

	while (gCodeBuffer[0] != 0)
	{
		if(gCodeBuffer[1]=='1')
		{
			gCodeBuffer += 4;
			numLen = 0;
			while (gCodeBuffer[0] != '.' && gCodeBuffer[0] != ' ')
			{
				gCodeBuffer++;
				numLen++;
			}
			x = (string2int(gCodeBuffer - numLen, numLen)) << 5;

			if (gCodeBuffer[0] == '.')
			{
				numLen = 0;
				gCodeBuffer++;
				while (gCodeBuffer[0] != ' ') 
				{ 
					gCodeBuffer++;
					numLen++; 
				}
				x += (string2float(gCodeBuffer - numLen, numLen) * quality + 0.5);
			}
			x |= 0x8000;
			memcpy(dataCompress, &x, 2);
			dataCompress += 2;
		}
		else if (gCodeBuffer[1] == '0')
		{
			gCodeBuffer += 4;
			numLen = 0;
			while (gCodeBuffer[0] != '.' && gCodeBuffer[0] != ' ')
			{
				gCodeBuffer++;
				numLen++;
			}
			x = (string2int(gCodeBuffer - numLen, numLen)) << 5;
			if (gCodeBuffer[0] == '.')
			{
				numLen = 0;
				gCodeBuffer++;
				while (gCodeBuffer[0] != ' ')
				{
					gCodeBuffer++;
					numLen++;
				}
				x += (string2float(gCodeBuffer - numLen, numLen) * quality + 0.5);
			}
			while (*gCodeBuffer++ != 'Y');
			numLen = 0;
			while (gCodeBuffer[0] != '.' && gCodeBuffer[0] != ' ')
			{
				gCodeBuffer++;
				numLen++;
			}
			y = (string2int(gCodeBuffer - numLen, numLen)) << 5;
			if (gCodeBuffer[0] == '.')
			{
				numLen = 0;
				gCodeBuffer++;
				while (gCodeBuffer[0] != ' ')
				{
					gCodeBuffer++;
					numLen++;
				}
				y += (string2float(gCodeBuffer - numLen, numLen) * quality + 0.5);
			}
			
			if (y != yLast)
			{
				yLast = y;
				x |= 0x4000;
				memcpy(dataCompress, &x, 2);
				dataCompress += 2;
				memcpy(dataCompress, &y, 2);
				dataCompress += 2;
			}
			else
			{
				memcpy(dataCompress, &x, 2);
				dataCompress += 2;
			}
		}
		while (*gCodeBuffer++ != '\n');
	}
	
	ofstream ff;
	if(isHead)
	    ff.open(OUTPUT_PATH + "/compress.bin", ios::out | ios::binary);
	else
		  ff.open(OUTPUT_PATH + "/compress.bin", ios::out | ios::app | ios::binary);

	ff.write((const char*)head, dataCompress - head);
	ff.flush();
	ff.close();
}


/*
* srcImg：openCV图像
* linePerMillimetre：每毫米(mm)多少行，即PC端软件的“质量”，范围1~20
* engraveWidth、engraveHeight，最终雕刻成品大小，单位毫米(mm)
* sMax：S最大值，即激光最大功率，范围1~1000
* isLineToLine：即PC软件的线到线处理，不做灰度抖动
* compressed: 是否压缩Gcode
*/

void ImageToGCode(Mat &srcImg, int linePerMillimetre, int engraveWidth, int engraveHeight, int sMax, int speed, bool isLineToLine, bool compressed)
{
	Mat M1 = imread(RESOURCE_PATH + "/lemu.jpg", 0);
	Mat M2;
	resize(M1, M2, Size(M1.cols * linePerMillimetre * engraveHeight / M1.rows, linePerMillimetre * engraveHeight), 0, 0, INTER_CUBIC);
	quality = linePerMillimetre * 1.0;
  
	if (isLineToLine == false)
	{
		image_atkinson(M2);
	}

	M2 = 255 - M2;
	flip(M2, M2, 0);
	uchar *imgdata = (uchar*)M2.data;

	uchar* gDataHead = new uchar[1024 * 1024 * 1]();
	gData = gDataHead;

	uchar* cDataHead = new uchar[1024 * 1024 * 1]();
	uchar* cData = cDataHead;
	bool ishead = true;

	ofstream f;
	f.open(OUTPUT_PATH + "/huahui.gcode", ios::out);
	f << "G90" << endl;
	f << "G0 X0 Y0 F" << speed << endl;
	f << "M3 S0" << endl;

	uint32_t x_Value = 0, y_Value = 0, s_Value = 0;
	uint32_t cacheLen = 0;

	for (int h = 0; h < M2.rows; h++)
	{
		const int reverse = (h & 1);

		x_Value = reverse ? M2.cols : 0;

		for (int w = reverse ? M2.cols - 1 : 0; reverse ? w > 0 : w < M2.cols; reverse ? w-- : w++)
		{
			if (isG0Mode == false && imgdata[h*M2.cols + w] != 0)
			{
				WriteGcodeString(x_Value, y_Value, 0);
			}
			x_Value = reverse ? x_Value - 1 : x_Value + 1;			
			if (imgdata[h*M2.cols + w] != imgdata[h*M2.cols + (reverse ? (w - 1) : (w + 1))])
			{
				s_Value = sMax * imgdata[h*M2.cols + w] / 255;
				WriteGcodeString(x_Value, y_Value, s_Value);
				cacheLen++;
			}
			if (((w == M2.cols - 2) && (reverse == 0)) || ((w == 1) && (reverse != 0)))
			{
				y_Value += 1;
				x_Value = reverse ? 0 : M2.cols;
				if (imgdata[h*M2.cols + (reverse ? (w - 1) : (w + 1))] != 0)
				{
					s_Value = sMax * imgdata[h*M2.cols + (reverse ? (w - 1) : (w + 1))] / 255;
					WriteGcodeString(x_Value, y_Value, s_Value);
					cacheLen += 2;
				}
				break;
			}
			if (cacheLen > 30000)
			{
				cacheLen = 0;

				f << gDataHead;
				f.flush();

        if(compressed)
        {
            GcodeCompress((char*)gDataHead, cDataHead, sMax, speed, ishead);
            if (ishead)
            {
              ishead = false;
            }
            memset(cDataHead, 0, 1024 * 1024 * 1);
        }

				memset(gDataHead, 0, 1024 * 1024 * 1);
				gData = gDataHead;
			}
		}
	}

	f << gDataHead;

	if(compressed)
  {
      GcodeCompress((char*)gDataHead, cDataHead, sMax, speed, ishead);
      if (ishead)
      {
        ishead = false;
      }
      memset(cDataHead, 0, 1024 * 1024 * 1);
  }

	f << "M5" << endl;
	f << "G0 X0 Y0 Z0" << endl;
	f.flush();
	f.close();

  if(compressed)
  {
      f.open(OUTPUT_PATH + "/compress.bin", ios::out | ios::app | ios::binary);
      f.write("\xEE\xFF", 2);
      f.flush();
      f.close();
  }

	delete []gDataHead;
  delete []cDataHead;
}

void WriteGcodeString2(uint32_t x, uint32_t y, int s)
{
	if (s != 0)
	{
		if (isG0Mode)
		{
			memcpy(gData, "G1 ", 3);
			gData += 3;
			isG0Mode = false;
		}
		memcpy(gData, "X", 1);
		gData += 1;
		StringLen = float2string(x, 1000, x_ValueString);
		memcpy(gData, x_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " Y", 2);
		gData += 2;
		StringLen = float2string(y, 1000, y_ValueString);
		memcpy(gData, y_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " S", 2);
		gData += 2;
		StringLen = int2string(s, s_ValueString);
		memcpy(gData, s_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, "\n", 1);
		gData += 1;
	}
	else
	{
		isG0Mode = true;
		memcpy(gData, "G0 X", 4);
		gData += 4;
		StringLen = float2string(x, 1000, x_ValueString);
		memcpy(gData, x_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " Y", 2);
		gData += 2;
		StringLen = float2string(y, 1000, y_ValueString);
		memcpy(gData, y_ValueString, StringLen);
		gData += StringLen;
		memcpy(gData, " S0\n", 4);
		gData += 4;
	}
}

void bwLabel(const uchar *bw, int *label, int h, int w, int sMax, int speed)
{
	memset(label, 0, h * w << 2);
	int maxComponents = (h * w >> 1) + 1; 
	int * const link = new int[maxComponents];
	int lb = 1, x, y, a, b, t, *p = label;
	link[0] = 0;
	if (bw[0]) {
		p[0] = lb;
		link[lb] = lb;
		lb++;
	}
	for (x = 1; x < w; x++) if (bw[x]) {
		if (p[x - 1])
			p[x] = p[x - 1];
		else {
			p[x] = lb;
			link[lb] = lb;
			lb++;
		}
	}
	bw += w, p += w;
	for (y = 1; y < h; y++, bw += w, p += w) {
		if (bw[0]) {
			if (p[-w])
				p[0] = p[-w];
			else {
				p[0] = lb;
				link[lb] = lb;
				lb++;
			}
		}
		for (x = 1; x < w; x++) if (bw[x]) {
			a = p[x - 1], b = p[x - w];
			if (a) {
				if (a == b)
					p[x] = a;
				else {
					t = a;
					while (a != link[a])
						a = link[a];
					p[x] = link[t] = a;
					if (b) {
						t = b;
						while (b != link[b])
							b = link[b];
						link[t] = b;
						if (a < b) link[b] = a; else link[a] = b;
					}
				}
			}
			else if (b) {
				t = b;
				while (b != link[b])
					b = link[b];
				p[x] = link[t] = b;
			}
			else {
				p[x] = lb;
				link[lb] = lb;
				lb++;
			}
		}
	}

	t = 1;
	for (x = 1; x < lb; x++)
		if (x == link[x]) {
			link[x] = -t; 
			t++;
		}
	for (x = 1; x < lb; x++) {
		y = x;
		while (link[y] >= 0)
			y = link[y];
		link[x] = link[y];
	}
	for (x = 1; x < lb; x++)
		link[x] = -link[x];

	p = label;
	for (y = 0; y < h; y++, p += w)
		for (x = 0; x < w; x++)
			p[x] = link[p[x]];

	delete[] link;

	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	uchar* gDataHead = new uchar[1024 * 1024 * 1]();
	gData = gDataHead;
	uint32_t cacheLen = 0;

	ofstream f;
	f.open(OUTPUT_PATH + "/fastModeGcode.gcode", ios::out);
	f << "G90" << endl;
	f << "G0 X0 Y0 F" << speed << endl;
	f << "M3 S0" << endl;
	
	Mat tmpimg;
	tmpimg.create(h, w, CV_8UC1);
	tmpimg = Scalar(0);
	uchar *tmpdata = (uchar*)tmpimg.data;

	Mat dstImage = Mat::zeros(h, w, CV_8UC3);

	unsigned int contours_size = 0;
	double maxval;
	

	for (int k = 1; k < t; k++)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if (label[j + i * w] == k)
				{
					tmpdata[j + i * w] = 255;
				}
			}
		}

		minMaxLoc(tmpimg, NULL, &maxval);
		while (maxval != 0)
		{
			findContours(tmpimg, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

			for (unsigned int i = 0; i < contours.size(); i++)
			{
				WriteGcodeString2(contours[i][0].x, contours[i][0].y, 0);
				cacheLen++;
				for (unsigned int j = 0; j < contours[i].size(); j++)
				{
					WriteGcodeString2(contours[i][j].x, contours[i][j].y, sMax);
					cacheLen++;
					if (cacheLen > 30000)
					{
						cacheLen = 0;

						f << gDataHead;
						f.flush();

						memset(gDataHead, 0, 1024 * 1024 * 1);
						gData = gDataHead;
					}
				}
				WriteGcodeString2(contours[i][0].x, contours[i][0].y, sMax);
				cacheLen++;

			}
			for (int index = 0; index >= 0; index = hierarchy[index][0])
			{
				drawContours(tmpimg, contours, index, Scalar(0), 1, 8, hierarchy);
			}
			minMaxLoc(tmpimg, NULL, &maxval);
		}
	}

	f << gDataHead;

	f << "M5" << endl;
	f << "G0 X0 Y0 Z0" << endl;
	f.flush();
	f.close();

	delete []gDataHead;
	delete []cDataHead;
}

/*
* 快速模式转Gcode
*/
void ImageFastModeToGcode(Mat &srcImg, int linePerMillimetre, int engraveWidth, int engraveHeight, int sMax, int speed)
{
	Mat M1 = imread(RESOURCE_PATH + "/lemu.jpg", 0);
	Mat M2;
	resize(M1, M2, Size(M1.cols * linePerMillimetre * engraveHeight / M1.rows, linePerMillimetre * engraveHeight), 0, 0, INTER_CUBIC);
	quality = linePerMillimetre * 1.0;
	M2 = M2 > 250;
	flip(M2, M2, 0);

	Mat labelImg;
	Mat label;
	label.create(M2.size(), CV_32SC1);
	labelImg.create(M2.size(), CV_8UC1);
	labelImg = Scalar(255);
	labelImg.setTo(Scalar(0), M2);
	bwLabel(labelImg.data, (int *)label.data, labelImg.rows, labelImg.cols, sMax, speed);
}

/*
* 切割转Gcode
*/
void ImageContoursToGCode(Mat &srcImg, int linePerMillimetre, int engraveWidth, int engraveHeight, int sMax, int speed)
{
	Mat M1 = imread(RESOURCE_PATH + "/lemu.jpg", 0);
	Mat M2;
	resize(M1, M2, Size(M1.cols * linePerMillimetre * engraveHeight / M1.rows, linePerMillimetre * engraveHeight), 0, 0, INTER_CUBIC);
	quality = linePerMillimetre * 1.0;
	M2 = M2 < 250;
	flip(M2, M2, 0);

	uchar* gDataHead = new uchar[1024 * 1024 * 1]();
	gData = gDataHead;

	ofstream f;
	f.open(OUTPUT_PATH + "/contoursGcode.gcode", ios::out);
	f << "G90" << endl;
	f << "G0 X0 Y0 F" << speed << endl;
	f << "M3 S0" << endl;

	uint32_t cacheLen = 0;

	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(M2, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	for (unsigned int i = 0; i < contours.size(); i++)
	{
		WriteGcodeString2(contours[i][0].x, contours[i][0].y, 0);
		cacheLen++;

		for (unsigned int j = 0; j < contours[i].size(); j++)
		{
			WriteGcodeString2(contours[i][j].x, contours[i][j].y, sMax);
			cacheLen++;

			if (cacheLen > 30000)
			{
				cacheLen = 0;

				f << gDataHead;
				f.flush();

				memset(gDataHead, 0, 1024 * 1024 * 1);
				gData = gDataHead;
			}

		}

		WriteGcodeString2(contours[i][0].x, contours[i][0].y, sMax);
		cacheLen++;
	}

	f << gDataHead;

	f << "M5" << endl;
	f << "G0 X0 Y0 Z0" << endl;
	f.flush();
	f.close();

	delete[]gDataHead;
}

int main() { 
	cv::Mat srcImg = cv::imread(RESOURCE_PATH + "/lemu.jpg");
	ImageToGCode(srcImg, 12, srcImg.cols(), srcImg.rows(), 1000, 2500, true, true);
	return 0;
}