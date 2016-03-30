//
//  Hs.hpp
//  RDH
//
//  Created by Wang on 3/14/16.
//  Copyright © 2016 Wang. All rights reserved.
//


#ifndef Hs_hpp
#define Hs_hpp

#include <stdio.h>
#include "Util.hpp"
#endif /* Hs_hpp */


using namespace std;
struct hs_threshold{
    vector<int> peakPoint;  // Selected peak value in every round
    vector<int> zeroPoint;  // Selected zero value in every round
    vector<int> bookkeeping; // Bookkeeping bits in every round
    vector<int> bitNum;   // To be embedded bits in every round
    int totalBit;   // To be embedded bits in all round
};

// Histogram Shifting Method
bool
hs_img_main(vector<vector<BYTE>> img, vector<BYTE> data, int bitNum);

// Embed Data into Grey Image
hs_threshold
hs_img_embed_main(vector<vector<BYTE>> &img, const vector<BYTE> &data_emb, int bitNum);

// Extract Data from Grey Image
bool
hs_img_recover_main(vector<vector<BYTE>> &img,
                    vector<BYTE> &data_re, hs_threshold thres);

// Is Enough Embed
bool
hs_img_embed_isEnough(vector<int> hist, int bitNum);

// Embed Shift Histogram
bool
hs_img_embed_shift(vector<vector<BYTE>> &img,
                     hs_threshold & thrs, vector<BYTE> &record);

// Recover Histogram
bool
hs_img_recover_shift(vector<vector<BYTE>> &img,
                     hs_threshold &thrs, vector<BYTE> &record);

// One Round Embed
bool
hs_img_embed_round(vector<vector<BYTE>> &img, const vector<BYTE> &data,
                   const vector<BYTE> &record, hs_threshold &thrs);

// One Round Extract
bool
hs_img_recover_round(vector<vector<BYTE>> &img, vector<BYTE> &data,
                   vector<BYTE> &record, hs_threshold &thrs);