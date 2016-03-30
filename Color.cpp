//
//  Color.cpp
//  RDHcpp
//
//  Created by Wang on 3/30/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#include "Color.hpp"
/*
 *   Colour BMP image
 */

// Read Color BMP Image from Storage
bool
readRGB(const string& filePath,
        vector<vector<RGB_PIXEL>>& image,
        BMP_FILE_HEADER* bmpHeader,
        BMP_INFO* bmpInfo,
        vector<RGB_QUAD>& bmpQuad){
    
    const char* cPath = filePath.c_str();
    FILE *fp;
    if((fp = fopen(cPath, "rb")) == NULL)
    {
        cerr << "Error (readRGB): Cannot Open File: " << filePath << endl;
        return false;
    }
    fread(bmpHeader, sizeof(BMP_FILE_HEADER), 1, fp);
    fread(bmpInfo, sizeof(BMP_INFO), 1, fp);
    for (size_t i = 0; i < bmpInfo -> bClrUsed; i++){
        RGB_QUAD quad;
        fread(&quad, sizeof(RGB_QUAD), 1, fp);
        bmpQuad.push_back(quad);
    }
    size_t col = bmpInfo -> bWidth;
    size_t row = bmpInfo -> bHeight;
    for (size_t r = 0; r < row; r++) {
        vector<RGB_PIXEL> row;
        for (size_t c = 0; c < col; c++) {
            RGB_PIXEL pixel(0, 0, 0);
            fread(&pixel, sizeof(RGB_PIXEL), 1, fp);
            row.push_back(pixel);
        }
        image.push_back(row);
    }
    
    fclose(fp);
    
    return true;
}

// Write Color BMP Image into Storage
bool
writeRGB(const string& filePath,
         const vector<vector<RGB_PIXEL>>& image,
         const BMP_FILE_HEADER* bmpHeader,
         const BMP_INFO* bmpInfo,
         const vector<RGB_QUAD>& bmpQuad){
    
    const char* cPath = filePath.c_str();
    FILE *fp;
    if ((fp = fopen(cPath, "wb")) == NULL) {
        cerr << "Error (writeRGB): Cannot Write File: " << filePath << endl;
        return false;
    }
    fwrite(bmpHeader, sizeof(BMP_FILE_HEADER), 1, fp);
    fwrite(bmpInfo, sizeof(BMP_INFO), 1, fp);
    for (size_t i = 0; i < bmpQuad.size(); i++) {
        RGB_QUAD quad = bmpQuad[i];
        fwrite(&quad, sizeof(RGB_QUAD), 1, fp);
    }
    size_t col = bmpInfo -> bWidth;
    size_t row = bmpInfo -> bHeight;
    for (size_t r = 0; r < row; r++) {
        for (size_t c = 0; c < col; c++) {
            RGB_PIXEL pixel = image[r][c];
            fwrite(&pixel, sizeof(RGB_PIXEL), 1, fp);
        }
    }
    fclose(fp);
    
    return true;
}

// Calculate Color Image PSNR
double
calPSNR(const vector<vector<RGB_PIXEL>>& img1,
        const vector<vector<RGB_PIXEL>>& img2){
    
    double myPsnr = 0.0;
    myPsnr = 10 * log10(pow(255.0, 2.0) / calMSE(img1, img2));
    return myPsnr;
}


// Calculate Color Image MSE (Mean Square Error)
double
calMSE(const vector<vector<RGB_PIXEL>>& img1,
       const vector<vector<RGB_PIXEL>>& img2){
    
    double myMse = 0.0;
    for (size_t i = 1; i < 4; i++) {
        myMse += calMSE(getRGBLevel(img1, i), getRGBLevel(img2, i));
    }
    return myMse / 3.0;
}

// Get Color Image Level
vector<vector<BYTE>>
getRGBLevel(const vector<vector<RGB_PIXEL>>& img,
            const size_t& level){
    
    size_t row = img.size();
    size_t col = img[0].size();
    vector<vector<BYTE>> res;
    for (size_t r = 0; r < row; r++) {
        vector<BYTE> temp;
        for (size_t c = 0; c < col; c++) {
            switch (level) {
                case 1:
                    temp.push_back(img[r][c].bRed);
                    break;
                case 2:
                    temp.push_back(img[r][c].bGreen);
                    break;
                case 3:
                    temp.push_back(img[r][c].bBlue);
                    break;
                default:
                    printf("Error (GetRGBLevel): Get Wrong RGB Level %d\n", level);
                    break;
            }
        }
        res.push_back(temp);
    }
    return res;
}

// Transfer RGB_PIXEL Colour Image into int Type Colour Image
bool
clr2rgb(const vector<vector<RGB_PIXEL>>& img,
        vector<vector<int>>& imgR,
        vector<vector<int>>& imgG,
        vector<vector<int>>& imgB){
    
    size_t row = img.size();
    size_t col = img[0].size();
    imgR.clear();
    imgG.clear();
    imgB.clear();
    vector<int> ivec(col, 0);
    imgR = vector<vector<int>>(row, ivec);
    imgG = imgR;
    imgB = imgR;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            const RGB_PIXEL& p = img[i][j];
            imgR[i][j] = p.bRed;
            imgG[i][j] = p.bGreen;
            imgB[i][j] = p.bBlue;
        }
    }
    return true;
}

// Transfer int Type Colour Image into RGB_PIXEL Colour Image
bool
rgb2clr(const vector<vector<int>>& imgR,
        const vector<vector<int>>& imgG,
        const vector<vector<int>>& imgB,
        vector<vector<RGB_PIXEL>>& img){
    
    size_t row = imgR.size();
    size_t col = imgR[0].size();
    img.clear();
    vector<RGB_PIXEL> ivec(col, RGB_PIXEL(0, 0, 0));
    img = vector<vector<RGB_PIXEL>>(row, ivec);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            const int& red = imgR[i][j];
            const int& green = imgG[i][j];
            const int& blue = imgB[i][j];
            if (red > 255 || green > 255 || blue > 255) {
                printf("Error (rgb2clr): Overflow!!!");
                return false;
            }
            if (red < 0 || green < 0 || blue < 0) {
                printf("Error (rgb2clr): Underflow!!!");
                return false;
            }
            img[i][j] =
            RGB_PIXEL((BYTE)blue, (BYTE)green, (BYTE)red);
        }
    }
    return true;
}

// Transfer RGB_PIXEL Colour Image into YCbCr int and double Type Colour Image
bool
clr2ycbcr(const vector<vector<RGB_PIXEL>>& img,
          vector<vector<double>>& imgY,
          vector<vector<double>>& imgCb,
          vector<vector<double>>& imgCr){
    
    const string TAG = "clr2ycbcr";
    vector<vector<double>> weight = {
        {0.299, 0.587, 0.114, 0},
        {-0.169, -0.331, 0.499, 128},
        {0.499, -0.418, -0.0813, 128}};
    size_t row = img.size();
    size_t col = img[0].size();
    imgY = vector<vector<double>>(row, vector<double>(col, 0));
    imgCb = imgY;
    imgCr = imgY;
    for (size_t r = 0; r < row; r++) {
        for (size_t c = 0; c < col; c++) {
            double red = img[r][c].bRed;
            double green = img[r][c].bGreen;
            double blue = img[r][c].bBlue;
            imgY[r][c] =
            red * weight[0][0] + green * weight[0][1] +
            blue * weight[0][2] + weight[0][3];
            imgCb[r][c] =
            red * weight[1][0] + green * weight[1][1] +
            blue * weight[1][2] + weight[1][3];
            imgCr[r][c] =
            red * weight[2][0] + green * weight[2][1] +
            blue * weight[2][2] + weight[2][3];
        }
    }
//    printImage(imgYf, TAG);
//    printImage(imgCbf, TAG);
//    printImage(imgCrf, TAG);
    return true;
}

// Transfer YCbCr int and double Type Colour Image into RGB_PIXEL Colour Image
bool
ycbcr2clr(const vector<vector<double>>& imgY,
          const vector<vector<double>>& imgCb,
          const vector<vector<double>>& imgCr,
          vector<vector<RGB_PIXEL>>& img){
    /*
     *  R = round(Y +                 + 1.402(Cr - 128))
     *  G = round(Y - 0.344(Cb - 128) - 0.714(Cr - 128))
     *  B = round(Y + 1.772(Cb - 128))
     */
    const string TAG = "ycbcr2clr";
    vector<vector<double>> b = {
        {0,    0, -128},
        {0, -128, -128},
        {0, -128,    0}
    };
    vector<vector<double>> k = {
        {1.0,      0.0,    1.402},
        {1.0,   -0.344,    -0.714},
        {1.0,    1.772,     0.0}
    };
    size_t row = imgY.size();
    size_t col = imgY[0].size();
    img = vector<vector<RGB_PIXEL>>(row,
        vector<RGB_PIXEL>(col,RGB_PIXEL(0, 0, 0)));
    for (size_t r = 0; r < row; r++) {
//        cout << "Line: " << r << endl;
        for (size_t c = 0; c < col; c++) {
            int red =
            imgY[r][c] + k[0][1] * imgCb[r][c] +
            k[0][2] * (imgCr[r][c] - 128) + 0.5;
            int green =
            imgY[r][c] + k[1][1] * (imgCb[r][c] - 128) +
            k[1][2] * (imgCr[r][c] - 128) + 0.5;
            int blue =
            imgY[r][c] + k[2][1] * (imgCb[r][c] - 128) +
            k[2][2] * imgCr[r][c] + 0.5;
            if (red > 255 || green > 255 || blue > 255) {
                printf("Warning (%s), Overflow!!!", TAG.c_str());
                return false;
            }
            if (red < 0 || green < 0 || blue < 0) {
                printf("Warning (%s), Underflow!!!", TAG.c_str());
                return false;
            }
            img[r][c] = RGB_PIXEL(blue, green, red);
//            cout << "[" << red << "," << green << "," <<blue << "]";
         }
//        cout << endl;
    }
    return true;
}