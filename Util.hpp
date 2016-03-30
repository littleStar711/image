//
//  Util.hpp
//  RDH
//
//  Created by Wang on 3/12/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp

#include <stdio.h>
#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "map"
#include "math.h"
#include "numeric"
#pragma pack(2)
using namespace std;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

struct BMP_FILE_HEADER{
    WORD bType; /* 文件标识符 */
    DWORD bSize; /* 文件的大小 */
    WORD bReserved1; /* 保留值,必须设置为0 */
    WORD bReserved2; /* 保留值,必须设置为0 */
    DWORD bOffset; /* 文件头的最后到图像数据位开始的偏移量 */
};

struct BMP_INFO{
    DWORD bInfoSize; /* 信息头的大小 */
    DWORD bWidth; /* 图像的宽度 */
    DWORD bHeight; /* 图像的高度 */
    WORD bPlanes; /* 图像的位面数 */
    WORD bBitCount; /* 每个像素的位数 */
    DWORD bCompression; /* 压缩类型 */
    DWORD bmpImageSize; /* 图像的大小,以字节为单位 */
    DWORD bXPelsPerMeter; /* 水平分辨率 */
    DWORD bYPelsPerMeter; /* 垂直分辨率 */
    DWORD bClrUsed; /* 使用的色彩数 */
    DWORD bClrImportant; /* 重要的颜色数 */
};

struct RGB_QUAD{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReversed;
};

struct RGB_PIXEL{
    BYTE bBlue;
    BYTE bGreen;
    BYTE bRed;
    RGB_PIXEL(BYTE bl, BYTE gr, BYTE re): bBlue(bl), bGreen(gr), bRed(re){}
};


// Write Random Bit File
bool
writeFile(const string& filePath,
          const size_t& bitNum);

// Read File
vector<BYTE>
readFile(const string& filePath,
         const size_t& bitNum);

/*
 *  Grey BMP image
 */

// Read Grey BMP Image from Storage
bool
readBMP(const string& filePath,
        vector<vector<BYTE>> &image,
        BMP_FILE_HEADER *bmpHeader,
        BMP_INFO *bmpInfo,
        vector<RGB_QUAD> &bmpQuad);

// Write Grey BMP Image into Storage
bool
writeBMP(const string& filePath,
         const vector<vector<BYTE>>& image,
         const BMP_FILE_HEADER* bmpHeader,
         const BMP_INFO* bmpInfo,
         const vector<RGB_QUAD>& bmpQuad);

// Print BMP Image Header in Console
void
printBMP_FILE_HEADER(const BMP_FILE_HEADER *bmp_header);

// Print BMP Image Infro in Console
void
printBMP_INFO(const BMP_INFO *bmp_info);

// Print Vector
template <typename T>
inline void
printVector(const vector<T> vec, string tag) {
    cout << tag << " vector: ";
    for (size_t i = 0; i < vec.size(); i++) {
        //        cout << "[" << i << ",";
        //        cout << vec[i] << "]";
        printf("[%d,%u]", i, vec[i]);
    }
    cout <<endl;
}

// Print Image
template <typename T>
inline void
printImage(const vector<vector<T>> &img, string tag){
    cout << tag << " Image: " << endl;
    size_t maxrow = img.size();
    size_t maxcol = img[0].size();
    maxrow = (maxrow < 10? maxrow: 10);
    maxcol = (maxcol < 20? maxcol: 20);
    for (size_t r = 0; r < maxrow; r++) {
        cout << "Line: " << r << endl;
        for(size_t c = 0; c < maxcol; c++){
            cout << "[" << r << "," << c << "," << img[r][c] << "]";
//            printf("[%d,%d,%u]", r, c, img[r][c]);
        }
        cout << endl;
    }
}

// Calculate Histogram
template <typename T>
inline vector<int>
calHistogram(const vector<vector<T>> &img){
    size_t bit = 8; // 8 bit depth
    vector<int> hist(1 << bit, 0);
    size_t row = img.size();
    size_t col = img[0].size();
    for (size_t r = 0; r < row; r++)
        for (size_t c = 0; c < col; c++)
            hist[img[r][c]]++;
    return hist;
}

// Calculate Gery Image MSE (Mean Square Error)
double
calMSE(const vector<vector<BYTE>>&,
       const vector<vector<BYTE>>&);

// Calculate Grey Image PSNR
double
calPSNR(const vector<vector<BYTE>>&,
        const vector<vector<BYTE>>&);

// Calculate Grey Image Entropy
double
calEntropy(const vector<vector<BYTE>>&);

// Convert Byte to Double Grey Image
vector<vector<double>>
byte2Double(const vector<vector<BYTE>>&);

// Convert Double to Byte Grey Image
vector<vector<BYTE>>
double2Byte(const vector<vector<double>>&);

// Convert Byte to Int Grey Image
vector<vector<int>>
byte2Int(const vector<vector<BYTE>>&);

// Convert Int to Byte Grey Image
vector<vector<BYTE>>
int2Byte(const vector<vector<int>>&);

// Max Value
template <typename T>
inline T
maxValue(const vector<vector<T>>& img){
    T max = img[0][0];
    int row = img.size();
    int col = img[0].size();
    for (int r = 0; r < row; r++)
        for (int c = 0; c < col; c++)
            if (img[r][c] > max)
                max = img[r][c];
    return max;
}

// Min Value
template <typename T>
inline T
minValue(const vector<vector<T>>& img){
    T min = img[0][0];
    int row = img.size();
    int col = img[0].size();
    for (int r = 0; r < row; r++)
        for (int c = 0; c < col; c++)
            if (img[r][c] < min)
                min = img[r][c];
    return min;
}

// cout BYTE
ostream&
operator<<(ostream& ,
           BYTE b);

// cout RGB_PIXEL
ostream&
operator<<(ostream&,
           const RGB_PIXEL&);
#endif /* Util_hpp */
