//
//  Color.hpp
//  RDHcpp
//
//  Created by Wang on 3/30/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <stdio.h>
#include "Util.hpp"
using namespace std;

/*
 *   Colour BMP image
 */

// Read Color BMP Image from Storage
bool
readRGB(const string& filePath,
        vector<vector<RGB_PIXEL>> &image,
        BMP_FILE_HEADER *bmpHeader,
        BMP_INFO *bmpInfo,
        vector<RGB_QUAD> &bmpQuad);

// Write Color BMP Image into Storage
bool
writeRGB(const string& filePath,
         const vector<vector<RGB_PIXEL>>& image,
         const BMP_FILE_HEADER* bmpHeader,
         const BMP_INFO* bmpInfo,
         const vector<RGB_QUAD>& bmpQuad);

// Calculate Color Image PSNR
double
calPSNR(const vector<vector<RGB_PIXEL>>&,
        const vector<vector<RGB_PIXEL>>&);

// Calculate Color Image MSE (Mean Square Error)
double
calMSE(const vector<vector<RGB_PIXEL>>&,
       const vector<vector<RGB_PIXEL>>&);

// Get RGB Level
vector<vector<BYTE>>
getRGBLevel(const vector<vector<RGB_PIXEL>>& img,
            const size_t& level);

// Transfer RGB_PIXEL Colour Image into int Type Colour Image
bool
clr2rgb(const vector<vector<RGB_PIXEL>>& image,
        vector<vector<int>>& imgR,
        vector<vector<int>>& imgG,
        vector<vector<int>>& imgB);

// Transfer int Type Colour Image into RGB_PIXEL Colour Image
bool
rgb2clr(const vector<vector<int>>& imgR,
        const vector<vector<int>>& imgG,
        const vector<vector<int>>& imgB,
        vector<vector<RGB_PIXEL>>& image);

// Transfer RGB_PIXEL Colour Image into YCbCr int and double Type Colour Image
bool
clr2ycbcr(const vector<vector<RGB_PIXEL>>& img,
          vector<vector<double>>& imgY,
          vector<vector<double>>& imgCb,
          vector<vector<double>>& imgCr);

// Transfer YCbCr int and double Type Colour Image into RGB_PIXEL Colour Image
bool
ycbcr2clr(const vector<vector<double>>& imgY,
          const vector<vector<double>>& imgCb,
          const vector<vector<double>>& imgCr,
          vector<vector<RGB_PIXEL>>& image);

#endif /* Color_hpp */
