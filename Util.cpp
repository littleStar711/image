//
//  Util.cpp
//  RDH
//
//  Created by Wang on 3/12/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#include "Util.hpp"

bool
writeFile(const string& filePath,
          const size_t& bitNum){
    /*  Write Random Bit File
     *
     */
    ofstream fopen(filePath.c_str());
    if (!fopen) {
        printf("Error (writeFile): File Open Exception\n\t: %s\n",
               filePath.c_str());
        return false;
    }
    if (bitNum <= 0) {
        return false;
    }
    for (size_t i = 0; i < bitNum; i++){
        BYTE ch = (rand() & 1 );
        fopen.put(ch);
    }
    fopen.close();
    fopen.clear();
    return true;
}

vector<BYTE>
readFile(const string& filePath,
         const size_t& bitNum){
    /*  Read Random File
     *
     */
    
    ifstream fin(filePath.c_str());
    vector<BYTE> res;
    if (!fin){
        printf("Error (readFile): Read File Exception\n\t:%s\n",
               filePath.c_str());
        return res;
    }
    if (bitNum <= 0) {
        return res;
    }
    char ch;
    for (size_t i = 0; (i < bitNum); i++) {
        fin.get(ch);
        res.push_back(ch);
    }
    
    fin.close();
    fin.clear();
    return res;
}

bool
readBMP(const string& filePath,
        vector<vector<BYTE>> &image,
        BMP_FILE_HEADER *bmpHeader,
        BMP_INFO *bmpInfo,
        vector<RGB_QUAD> &bmpQuad){
    /*  Read Grey BMP Image from Storage
     *
     *
     */
    
    const char* cPath = filePath.c_str();
    FILE *fp;
    if((fp = fopen(cPath, "rb")) == NULL)
    {
        cerr << "Error (readBMP): Cannot Open File: " << filePath << endl;
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
        vector<BYTE> row;
        for (size_t c = 0; c < col; c++) {
            BYTE pixel;
            fread(&pixel, sizeof(BYTE), 1, fp);
            row.push_back(pixel);
        }
        image.push_back(row);
    }
    
    fclose(fp);
    
    return true;
}

bool
writeBMP(const string& filePath,
         const vector<vector<BYTE>>& image,
         const BMP_FILE_HEADER* bmpHeader,
         const BMP_INFO* bmpInfo,
         const vector<RGB_QUAD>& bmpQuad){
    /* Write Grey BMP Image into Storage
     *
     */
    
    const char* cPath = filePath.c_str();
    FILE *fp;
    if ((fp = fopen(cPath, "wb")) == NULL) {
        cerr << "Error (writeBMP): Cannot Write File: " << filePath << endl;
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
            BYTE pixel = image[r][c];
            fwrite(&pixel, sizeof(BYTE), 1, fp);
        }
    }
    fclose(fp);
    return true;
}

// print BMP_FILE_HEADER
void
printBMP_FILE_HEADER(const BMP_FILE_HEADER *bmp_header){

    char* info = "BMP_HEADER";
    printf("\n%s Type:\t%X\n", info, bmp_header -> bType);
    printf("%s Size:\t%u\n", info, bmp_header -> bSize);
    printf("%s Reserved 1:\t%u\n", info, bmp_header -> bReserved1);
    printf("%s Reserved 2:\t%u\n", info, bmp_header -> bReserved2);
    printf("%s Offset:\t%u\n", info, bmp_header -> bOffset);
}

// print BMP_INFO
void
printBMP_INFO(const BMP_INFO *bmp_info){

    char* info = "BMP_INFO";
    printf("\n%s Info Size:\t%u\n", info, bmp_info -> bInfoSize);
    printf("%s Width:\t%u\n", info, bmp_info -> bWidth);
    printf("%s Height:\t%u\n", info, bmp_info -> bHeight);
    printf("%s Planes Per Pixel:\t%u\n", info, bmp_info -> bPlanes);
    printf("%s Bit Per Pixel:\t%u\n", info, bmp_info -> bBitCount);
    printf("%s Compression:\t%u\n", info, bmp_info -> bCompression);
    printf("%s Image Size(Byte):\t%u\n", info, bmp_info -> bmpImageSize);
    printf("%s X Pels Per Meter:\t%u\n", info, bmp_info -> bXPelsPerMeter);
    printf("%s Y Pels Per Meter:\t%u\n", info, bmp_info -> bYPelsPerMeter);
    printf("%s Color Used:\t%u\n", info, bmp_info -> bClrUsed);
    printf("%s Color Important:\t%u\n", info, bmp_info -> bClrImportant);
}

// Calculate Gery Image MSE (Mean Square Error)
double
calMSE(const vector<vector<BYTE>>& img1,
       const vector<vector<BYTE>>& img2){
    
    if (img1.size() != img2.size() || img1[0].size() != img2[0].size()){
        printf("Error (CalMSE): Images' Size Must Same!");
        exit(-1);
    }
    size_t row = img1.size();
    size_t col = img1[0].size();
    double myMse = 0.0;
    for (size_t r = 0; r < row; r++) {
        for (size_t c = 0; c < col; c++) {
            double p1 = img1[r][c];
            double p2 = img2[r][c];
            myMse += pow( p1 - p2, 2.0);
        }
    }
    myMse /= row * col;
    return myMse;
}

// Calculate Grey Image PSNR
double
calPSNR(const vector<vector<BYTE>>& img1,
        const vector<vector<BYTE>>& img2){

    double myPsnr = 0.0;
    myPsnr = 10 * log10(pow(255.0, 2.0) / calMSE(img1, img2));
    return myPsnr;
}

// Calculate Grey Image Entropy
double
calEntropy(const vector<vector<BYTE>>& img){

    double myEntropy = 0.0;
    vector<int> hist = calHistogram(img);
    double pixelNum = accumulate(hist.begin(), hist.end(), 0);
    for (size_t i = 0; i < hist.size(); i++) {
        
        double temp = hist[i] / pixelNum;
        temp = temp * log2(temp);
        if (isnan(temp)) {
            temp = 0;
        }
        myEntropy += temp;
    }
    return -1.0 * myEntropy;
}

// Convert Byte to Double Grey Image
vector<vector<double>>
byte2Double(const vector<vector<BYTE>>& img){
    
    size_t row = img.size();
    size_t col = img[0].size();
    vector<vector<double>> res;
    for (size_t i = 0; i < row; i++) {
        vector<double> temp;
        for (size_t j = 0; j < col; j++) {
            temp.push_back(img[i][j]);
        }
        res.push_back(temp);
    }
    return res;
}

// Convert Double to Byte Grey Image
vector<vector<BYTE>>
double2Byte(const vector<vector<double>>& img){
    
    size_t row = img.size();
    size_t col = img[0].size();
    vector<vector<BYTE>> res;
    for (size_t i = 0; i < row; i++) {
        vector<BYTE> temp;
        for (size_t j = 0; j < col; j++) {
            double p = img[i][j];
            if (p > 0xff){
                continue;
//                printf("Warning (Double2Byte): Could Cause Overflow!\n");
            }
            else if (p < 0){
                continue;
//                printf("Warning (Double2Byte): Could Cause Underflow!\n");
            }
            temp.push_back(BYTE(p));
        }
        res.push_back(temp);
    }
    return res;
}

// Convert Byte to Int Grey Image
vector<vector<int>>
byte2Int(const vector<vector<BYTE>>& img){
    size_t row = img.size();
    size_t col = img[0].size();
    vector<vector<int>> res;
    for (size_t i = 0; i < row; i++) {
        vector<int> temp;
        for (size_t j = 0; j < col; j++) {
            temp.push_back(img[i][j]);
        }
        res.push_back(temp);
    }
    return res;
}

// Convert Int to Byte Grey Image
vector<vector<BYTE>>
int2Byte(const vector<vector<int>>& img){
    size_t row = img.size();
    size_t col = img[0].size();
    vector<vector<BYTE>> res;
    for (size_t i = 0; i < row; i++) {
        vector<BYTE> temp;
        for (size_t j = 0; j < col; j++) {
            int p = img[i][j];
            if (p > 0xff){
                continue;
                //                printf("Warning (int2Byte): Could Cause Overflow!\n");
                //                exit(-1);
            }
            else if (p < 0){
                continue;
                //                printf("Warning (int2Byte): Could Cause Underflow!\n");
                //               exit(-1);
            }
            temp.push_back(BYTE(p));
        }
        res.push_back(temp);
    }
    return res;
}

// cout BYTE
ostream&
operator<<(ostream& os,
           BYTE b){
    printf("%u", b);
    return os;
}

// cout RGB_PIXEL
ostream&
operator<<(ostream& os,
           const RGB_PIXEL& p){
    cout <<p.bRed << "," << p.bGreen << "," << p.bBlue;
    return os;
}



