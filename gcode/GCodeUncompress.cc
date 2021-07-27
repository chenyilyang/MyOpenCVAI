#include <iostream>
#include <fstream>
#include <ios>
static uint8_t info[128];
uint16_t yLastv = 0;
const std::string RESOURCE_PATH = "/Users/mac/OpenCV/MyOpenCVAI/MyOpenCVAI/resources";
const std::string OUTPUT_PATH = "/Users/mac/OpenCV/MyOpenCVAI/MyOpenCVAI/output";

int int2string(int d, char *s)
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

int GcodeUnCompress(uint8_t *dataCompress, uint16_t dataCompressLen, uint8_t *gcodeBuf, uint16_t gcodeBufLen)
{
	uint8_t slen;
	char str[10];
	uint8_t * tmpGcodeHead = gcodeBuf;
	uint8_t *gcodeBufHead = gcodeBuf;
	uint8_t *fchar = &info[6];
	uint16_t sValue = *(uint16_t *)&info[2];
	uint16_t *dataHead;
	uint16_t xi, yi, xf, yf;
	int inRemainLen;

	memset(gcodeBuf, 0, gcodeBufLen);

	if (*(uint16_t *)dataCompress == 0xFFEE && dataCompressLen >= 128)
	{
		memcpy(info, dataCompress, 128);
		dataCompress += 128;
		dataCompressLen -= 128;
		memcpy(gcodeBuf, "G90\nG0 X0 Y0 F", 14);
		slen = int2string(*(uint16_t *)&info[4], str);
		memcpy(&gcodeBuf[14], str, slen);
		memcpy(&gcodeBuf[14 + slen], "\n", 1);
		memcpy(&gcodeBuf[15 + slen], "M3 S0\n", 6);
		gcodeBuf += (21 + slen);
	}

	dataHead = (uint16_t *)(dataCompress);
	inRemainLen = dataCompressLen - ((uint8_t *)dataHead - dataCompress);

	while (inRemainLen >= 2 && (dataHead[0] != 0x0000) && (dataHead[0] != 0xFFEE) && (gcodeBufLen - (gcodeBuf - gcodeBufHead)) >= 30)
	{
		if (dataHead[0] & 0x8000)
		{
			memcpy(gcodeBuf, "G1 X", 4);
			gcodeBuf += 4;
			xi = (dataHead[0] & 0x3FE0) >> 5;
			slen = int2string(xi, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			xf = dataHead[0] & 0x1f;
			if (xf)
			{
				memcpy(gcodeBuf, ".", 1);
				gcodeBuf++;
				memcpy(gcodeBuf, &fchar[(xf - 1) * 4 + 1], fchar[(xf - 1) * 4]);
				gcodeBuf += fchar[(xf - 1) * 4];
			}
			memcpy(gcodeBuf, " S", 2);
			gcodeBuf += 2;
			slen = int2string(sValue, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			memcpy(gcodeBuf, "\n", 1);
			gcodeBuf++;
			dataHead++;
		}
		else if ((dataHead[0] & 0x4000) && inRemainLen >= 4)
		{
			memcpy(gcodeBuf, "G0 X", 4);
			gcodeBuf += 4;
			xi = (dataHead[0] & 0x3FE0) >> 5;
			slen = int2string(xi, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			xf = dataHead[0] & 0x1f;
			if (xf)
			{
				memcpy(gcodeBuf, ".", 1);
				gcodeBuf++;
				memcpy(gcodeBuf, &fchar[(xf - 1) * 4 + 1], fchar[(xf - 1) * 4]);
				gcodeBuf += fchar[(xf - 1) * 4];
			}
			dataHead++;
			memcpy(gcodeBuf, " Y", 2);
			gcodeBuf += 2;
			yLastv = dataHead[0];
			yi = (dataHead[0] & 0x3FE0) >> 5;
			slen = int2string(yi, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			yf = dataHead[0] & 0x1f;
			if (yf)
			{
				memcpy(gcodeBuf, ".", 1);
				gcodeBuf++;
				memcpy(gcodeBuf, &fchar[(yf - 1) * 4 + 1], fchar[(yf - 1) * 4]);
				gcodeBuf += fchar[(yf - 1) * 4];
			}
			memcpy(gcodeBuf, " S0\n", 4);
			gcodeBuf += 4;
			dataHead++;
		}
		else if ((dataHead[0] & 0xC000) == 0)
		{
			memcpy(gcodeBuf, "G0 X", 4);
			gcodeBuf += 4;
			xi = (dataHead[0] & 0x3FE0) >> 5;
			slen = int2string(xi, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			xf = dataHead[0] & 0x1f;
			if (xf)
			{
				memcpy(gcodeBuf, ".", 1);
				gcodeBuf++;
				memcpy(gcodeBuf, &fchar[(xf - 1) * 4 + 1], fchar[(xf - 1) * 4]);
				gcodeBuf += fchar[(xf - 1) * 4];
			}
			memcpy(gcodeBuf, " Y", 2);
			gcodeBuf += 2;
			yi = (yLastv) >> 5;
			slen = int2string(yi, str);
			memcpy(gcodeBuf, str, slen);
			gcodeBuf += slen;
			yf = yLastv & 0x1f;
			if (yf)
			{
				memcpy(gcodeBuf, ".", 1);
				gcodeBuf++;
				memcpy(gcodeBuf, &fchar[(yf - 1) * 4 + 1], fchar[(yf - 1) * 4]);
				gcodeBuf += fchar[(yf - 1) * 4];
			}
			memcpy(gcodeBuf, " S0\n", 4);
			gcodeBuf += 4;
			dataHead++;
		}
		else
		{
			return inRemainLen;
		}
		inRemainLen = dataCompressLen - ((uint8_t *)dataHead - dataCompress);
	}

	if (inRemainLen >= 2 && dataHead[0] == 0xFFEE && (gcodeBufLen - (gcodeBuf - gcodeBufHead)) >= 20)
	{
		memcpy(gcodeBuf, "M5\nG0 X0 Y0 Z0\n", 15);
	}

	// return inRemainLen;
	return gcodeBuf - tmpGcodeHead;
}

uint8_t gcodeBuf[1024];
//这里假设存储在Flash里面的压缩的gcode大小为1KByte
int gcodeCompressLength = 1024;

int main()
{
	uint8_t spiFlashBuf[150];
	int remainlen = 0;
	int addrOffset = 0;
	int buflen = 0;
	std::ifstream infstream;
	infstream.open(OUTPUT_PATH + "/huahui.gcode.bin", std::ifstream::binary);
	infstream.seekg(0, std::ios::end);
	gcodeCompressLength = infstream.tellg();
	std::cout << "gcodeCompressLength = " << gcodeCompressLength << std::endl;
	infstream.seekg(0, std::ios::beg);
	std::ofstream outfstream;
	outfstream.open(OUTPUT_PATH + "/huahui_uncompress.gcode", std::ios::out);
	while (addrOffset < gcodeCompressLength)
	{
		memset(spiFlashBuf, 0, 150);
		memset(gcodeBuf, 0, 1024);
		infstream.read((char *)spiFlashBuf, 150);
		buflen = infstream.gcount();
		std::cout << "buflen = " << buflen << std::endl;
		// spiReadBytes(addrOffset, spiFlashBuf, 150);  //从SPI Flash 读取压缩的gcode
		remainlen = GcodeUnCompress(spiFlashBuf, buflen, gcodeBuf, 1024);
		std::cout << "readed = " << remainlen << std::endl;
		addrOffset += buflen;
		std::cout << "addrOffset = " << addrOffset << std::endl;
		/*
		*  这里就可以对解压缩出来的gcodeBuf进行处理了
		*  TODO
		*
		*/
		outfstream.write((const char *)gcodeBuf, remainlen);
	}
	infstream.close();
	outfstream.close();
	return 0;
}