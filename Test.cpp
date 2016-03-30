//
//  Test.cpp
//  RDH
//
//  Created by Wang on 3/19/16.
//  Copyright © 2016 Wang. All rights reserved.
//

#include "Test.hpp"
const string SrcDir = "/Users/wang/Code/Cpp/RDHcpp/RDHcpp/BMP_Images/";
const string SrcImg[] = {"Lena_color.bmp", "Baboon_color.bmp",
    "Airplane_color.bmp", "Barbara_color.bmp"};
void
test_util_cpp(){
    string files[] = {"Lena.bmp", "Baboon.bmp", "Barbara.bmp", "Lena_color.bmp"};
    if (true) { // RGB image read and write test
        int i = 3;
        cout << files[i] <<endl;
        vector<vector<RGB_PIXEL>> IMG, I1;
        BMP_FILE_HEADER bmpHeader;
        BMP_INFO bmpInfo;
        vector<RGB_QUAD> QUAD;
        readRGB(SrcDir + files[i], IMG, &bmpHeader, &bmpInfo, QUAD);
        printBMP_FILE_HEADER(&bmpHeader);
        printBMP_INFO(&bmpInfo);
        vector<vector<int>> imgR, imgG, imgB;
        clr2rgb(IMG, imgR, imgG, imgB);
        //        printImage(imgR, "vector");
        rgb2clr(imgR, imgG, imgB, I1);
        cout << "PSNR: " << calPSNR(IMG, I1) << endl;
    }
    else{ //Grey image read and write test
        for (int i = 1; i < 2; i++ ){
            cout << files[i] <<endl;
            vector<vector<BYTE>> IMG;
            BMP_FILE_HEADER bmpHeader;
            BMP_INFO bmpInfo;
            vector<RGB_QUAD> QUAD;
            readBMP(SrcDir + files[i], IMG, &bmpHeader, &bmpInfo, QUAD);
            printBMP_FILE_HEADER(&bmpHeader);
            printBMP_INFO(&bmpInfo);
            vector<vector<double>> DIMG = byte2Double(IMG);
            vector<int> myhist = calHistogram(IMG);
            
            vector<vector<BYTE>> IMG2 = double2Byte(DIMG);
            cout << "PSNR: " << calPSNR(IMG2, IMG) << endl;
            cout << "Entropy: " << calEntropy(IMG) <<endl;
            // Warning : writeBMP filePath != readBMP filePath !!!
            writeBMP(SrcDir + "wr_" + files[i], IMG2, &bmpHeader, &bmpInfo, QUAD);
        }
    }
}

// Color Test
void
test_color_cpp(){
    const string TAG = "colorTest";
    int idx = 3;
    const string ImgPath = SrcDir + SrcImg[idx];
    const string WrImgPath = SrcDir + "wr_" + SrcImg[idx];
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<RGB_PIXEL>> I0, I1;
    vector<vector<double>> imgYf, imgCbf, imgCrf;
    cout << SrcImg[idx] << " processing..." << endl;
    // Read color image
    readRGB(ImgPath, I0, &bmp_header, &bmp_info, bmp_quad);
    I1 = I0;
    // PSNR
    cout << TAG << " psnr=" << calPSNR(I1, I0) << endl;
    // clr -> ycbcr
    clr2ycbcr(I1, imgYf, imgCbf, imgCrf);
    // ycbcr -> clr
    ycbcr2clr(imgYf, imgCbf, imgCrf, I1);
    // PSNR
    cout << TAG << " psnr=" << calPSNR(I1, I0) << endl;
    // Write color image
    writeRGB(WrImgPath, I1, &bmp_header, &bmp_info, bmp_quad);
}

void
test_pe_lena(){
    string TAG = "test_pe_lena";
    string ImgPath = SrcDir + "Lena.bmp";
    string DataPath = SrcDir + "randomBit";
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<BYTE>> IMG;
    // Read image
    readBMP(ImgPath, IMG, &bmp_header, &bmp_info, bmp_quad);
    // Read data
    vector<double> bpp = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    
    DWORD bit_embed = bmp_info.bWidth * bmp_info.bHeight;
    vector<pe_threshold> thres;
    thres.push_back(pe_threshold(bit_embed * 0.01, -4, 14, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.02,  4, 26, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.03,  3, 21, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.04, -4, 36, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.05,  3, 32, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.1,  2, 48, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.2, -1,100, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.3,  2,100, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.4, -2,250, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.5, -3,200, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.6, -4,250, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.7, -5,550, 0, 0));
    for (int i = 0; i < thres.size(); i++) {
        // Read Data (bpp * Width * Height)
        vector<BYTE> data = readFile(DataPath, thres[i].totalBit);
        // Print Infomation
        printf("IMG = %s, Bits = %d\n", "Lena.bmp", thres[i].totalBit);
        printf("Size = %ux%u\n", bmp_info.bWidth, bmp_info.bHeight);
        vector<vector<BYTE>> img = IMG;
        vector<BYTE> Do = data;
        if (!pe_img_embed_main(img, Do, thres[i])) {
            printf("Error : Cannot Embed All Data.\n");
            continue;
        }
        vector<vector<BYTE>> Im = img;
        printf("%s: PSNR(Io,Im) = %2.2f dB, BPP = %1.2f\n",
               TAG.c_str(), calPSNR(Im, IMG), bpp[i]);
        vector<BYTE> Dr;
        pe_img_recover_main(img, Dr, thres[i]);
        vector<vector<BYTE>> Ir = img;
        if (calPSNR(Ir, IMG) != INFINITY ||
            !equal(Dr.begin(), Dr.end(), Do.begin())){
            cout << "FATAL ERROR" <<endl;
        }
        else{
            cout << "CORRECT " << endl;
        }
        printf("[PSNR(Ir,Io) = %f, ", calPSNR(Ir, IMG));
        cout <<"Data Recover:" <<boolalpha << equal(Dr.begin(), Dr.end(), Do.begin()) << "]" << endl<< endl;
    }
}

void
test_pe_baboon(){
    string TAG = "test_pe_baboon";
    string ImgPath = SrcDir + "Baboon.bmp";
    string DataPath = SrcDir + "randomBit";
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<BYTE>> IMG;
    // Read image
    readBMP(ImgPath, IMG, &bmp_header, &bmp_info, bmp_quad);
    // Read data
    vector<double> bpp = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    
    DWORD bit_embed = bmp_info.bWidth * bmp_info.bHeight;
    vector<pe_threshold> thres;
    thres.push_back(pe_threshold(bit_embed * 0.01, -5,  85, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.02,  3, 110, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.03,  3, 180, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.04,  3, 310, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.05, -3, 510, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.1, -3, 540, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.2, -4, 660, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.3, -7, 800, 4, 0));
    thres.push_back(pe_threshold(bit_embed *  0.4,-11,1100, 7, 0));
    thres.push_back(pe_threshold(bit_embed *  0.5,-12,2400, 9, 0));
    thres.push_back(pe_threshold(bit_embed *  0.6,-13,2200,17,15));
    thres.push_back(pe_threshold(bit_embed *  0.7,-13,2400,22, 0));
    for (int i = 0; i < thres.size(); i++) {
        // Read Data (bpp * Width * Height)
        vector<BYTE> data = readFile(DataPath, thres[i].totalBit);
        // Print Infomation
        printf("IMG = %s, Bits = %d\n", "Baboon.bmp", thres[i].totalBit);
        printf("Size = %ux%u\n", bmp_info.bWidth, bmp_info.bHeight);
        vector<vector<BYTE>> img = IMG;
        vector<BYTE> Do = data;
        if (!pe_img_embed_main(img, Do, thres[i])) {
            printf("Error : Cannot Embed All Data.\n");
            continue;
        }
        vector<vector<BYTE>> Im = img;
        printf("%s: PSNR(Io,Im) = %2.2f dB, BPP = %1.2f\n",
               TAG.c_str(), calPSNR(Im, IMG), bpp[i]);
        vector<BYTE> Dr;
        pe_img_recover_main(img, Dr, thres[i]);
        vector<vector<BYTE>> Ir = img;
        if (calPSNR(Ir, IMG) != INFINITY ||
            !equal(Dr.begin(), Dr.end(), Do.begin())){
            cout << "FATAL ERROR" <<endl;
        }
        else{
            cout << "CORRECT " << endl;
        }
        printf("[PSNR(Ir,Io) = %f, ", calPSNR(Ir, IMG));
        cout <<"Data Recover:" <<boolalpha << equal(Dr.begin(), Dr.end(), Do.begin()) << "]" << endl<< endl;
    }
}

void
test_pe_woman(){
    string TAG = "test_pe_woman";
    string ImgPath = SrcDir + "Woman.bmp";
    string DataPath = SrcDir + "randomBit";
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<BYTE>> IMG;
    // Read image
    readBMP(ImgPath, IMG, &bmp_header, &bmp_info, bmp_quad);
    // Read data
    vector<double> bpp = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    
    DWORD bit_embed = bmp_info.bWidth * bmp_info.bHeight;
    vector<pe_threshold> thres;
    thres.push_back(pe_threshold(bit_embed * 0.01, -2,   8, 0, 1));
    thres.push_back(pe_threshold(bit_embed * 0.02,  1,  17, 0, 1));
    thres.push_back(pe_threshold(bit_embed * 0.03,  0,   6, 0, 1));
    thres.push_back(pe_threshold(bit_embed * 0.04,  0,   7, 0, 1));
    thres.push_back(pe_threshold(bit_embed * 0.05,  0,   7, 0, 1));
    thres.push_back(pe_threshold(bit_embed *  0.1,  0,  11, 0, 1));
    thres.push_back(pe_threshold(bit_embed *  0.2,  1,  50, 1, 1));
    thres.push_back(pe_threshold(bit_embed *  0.3, -1, 200, 1, 3));
    thres.push_back(pe_threshold(bit_embed *  0.4, -2,  80, 2, 3));
    thres.push_back(pe_threshold(bit_embed *  0.5, -2,  60, 3, 4));
    thres.push_back(pe_threshold(bit_embed *  0.6, -2, 600, 3, 5));
    thres.push_back(pe_threshold(bit_embed *  0.7, -2, 300, 4, 4));
    for (int i = 0; i < thres.size(); i++) {
        // Read Data (bpp * Width * Height)
        vector<BYTE> data = readFile(DataPath, thres[i].totalBit);
        // Print Infomation
        printf("IMG = %s, Bits = %d\n", "Woman.bmp", thres[i].totalBit);
        printf("Size = %ux%u\n", bmp_info.bWidth, bmp_info.bHeight);
        vector<vector<BYTE>> img = IMG;
        vector<BYTE> Do = data;
        if (!pe_img_embed_main(img, Do, thres[i])) {
            printf("Error : Cannot Embed All Data.\n");
            continue;
        }
        vector<vector<BYTE>> Im = img;
        printf("%s: PSNR(Io,Im) = %2.2f dB, BPP = %1.2f\n",
               TAG.c_str(), calPSNR(Im, IMG), bpp[i]);
        vector<BYTE> Dr;
        pe_img_recover_main(img, Dr, thres[i]);
        vector<vector<BYTE>> Ir = img;
        if (calPSNR(Ir, IMG) != INFINITY ||
            !equal(Dr.begin(), Dr.end(), Do.begin())){
            cout << "FATAL ERROR" <<endl;
        }
        else{
            cout << "CORRECT " << endl;
        }
        printf("[PSNR(Ir,Io) = %f, ", calPSNR(Ir, IMG));
        cout <<"Data Recover:" <<boolalpha << equal(Dr.begin(), Dr.end(), Do.begin()) << "]" << endl<< endl;
    }
}

void
test_pe_airplane(){
    string TAG = "test_pe_airplane";
    string ImgPath = SrcDir + "Airplane.bmp";
    string DataPath = SrcDir + "randomBit";
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<BYTE>> IMG;
    // Read image
    readBMP(ImgPath, IMG, &bmp_header, &bmp_info, bmp_quad);
    // Read data
    vector<double> bpp = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    
    DWORD bit_embed = bmp_info.bWidth * bmp_info.bHeight;
    vector<pe_threshold> thres;
    thres.push_back(pe_threshold(bit_embed * 0.01,  1,   3, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.02,  1,   4, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.03,  1,   4, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.04,  1,   6, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.05,  1,   6, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.1,  1,  10, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.2, -1,  17, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.3,  1,  26, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.4, -2,  37, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.5, -3,  52, 3, 0));
    thres.push_back(pe_threshold(bit_embed *  0.6, -4,  94, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.7, -5, 220, 4, 1));
    for (int i = 0; i < thres.size(); i++) {
        // Read Data (bpp * Width * Height)
        vector<BYTE> data = readFile(DataPath, thres[i].totalBit);
        // Print Infomation
        printf("IMG = %s, Bits = %d\n", "Airplane.bmp", thres[i].totalBit);
        printf("Size = %ux%u\n", bmp_info.bWidth, bmp_info.bHeight);
        vector<vector<BYTE>> img = IMG;
        vector<BYTE> Do = data;
        if (!pe_img_embed_main(img, Do, thres[i])) {
            printf("Error : Cannot Embed All Data.\n");
            continue;
        }
        vector<vector<BYTE>> Im = img;
        printf("%s: PSNR(Io,Im) = %2.2f dB, BPP = %1.2f\n",
               TAG.c_str(), calPSNR(Im, IMG), bpp[i]);
        vector<BYTE> Dr;
        pe_img_recover_main(img, Dr, thres[i]);
        vector<vector<BYTE>> Ir = img;
        if (calPSNR(Ir, IMG) != INFINITY ||
            !equal(Dr.begin(), Dr.end(), Do.begin())){
            cout << "FATAL ERROR" <<endl;
        }
        else{
            cout << "CORRECT " << endl;
        }
        printf("[PSNR(Ir,Io) = %f, ", calPSNR(Ir, IMG));
        cout <<"Data Recover:" <<boolalpha << equal(Dr.begin(), Dr.end(), Do.begin()) << "]" << endl<< endl;
    }
}

void
test_pe_barbara(){
    string TAG = "test_pe_barbara";
    string ImgPath = SrcDir + "Barbara.bmp";
    string DataPath = SrcDir + "randomBit";
    BMP_FILE_HEADER bmp_header;
    BMP_INFO bmp_info;
    vector<RGB_QUAD> bmp_quad;
    vector<vector<BYTE>> IMG;
    // Read image
    readBMP(ImgPath, IMG, &bmp_header, &bmp_info, bmp_quad);
    // Read data
    vector<double> bpp = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
    
    DWORD bit_embed = bmp_info.bWidth * bmp_info.bHeight;
    vector<pe_threshold> thres;
    thres.push_back(pe_threshold(bit_embed * 0.01, -4,  18, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.02, -4,  30, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.03, -4,  38, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.04, -3,  23, 0, 0));
    thres.push_back(pe_threshold(bit_embed * 0.05, -3,  31, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.1,  2,  65, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.2, -3,  85, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.3, -3, 100, 0, 0));
    thres.push_back(pe_threshold(bit_embed *  0.4, -4, 120, 0, 4));
    thres.push_back(pe_threshold(bit_embed *  0.5, -6, 250, 0, 2));
    thres.push_back(pe_threshold(bit_embed *  0.6, -6, 320, 0, 3));
    thres.push_back(pe_threshold(bit_embed *  0.7, -6, 250, 0, 0));
    for (int i = 0; i < thres.size(); i++) {
        // Read Data (bpp * Width * Height)
        vector<BYTE> data = readFile(DataPath, thres[i].totalBit);
        // Print Infomation
        printf("IMG = %s, Bits = %d\n", "Barbara.bmp", thres[i].totalBit);
        printf("Size = %ux%u\n", bmp_info.bWidth, bmp_info.bHeight);
        vector<vector<BYTE>> img = IMG;
        vector<BYTE> Do = data;
        if (!pe_img_embed_main(img, Do, thres[i])) {
            printf("Error : Cannot Embed All Data.\n");
            continue;
        }
        vector<vector<BYTE>> Im = img;
        printf("%s: PSNR(Io,Im) = %2.2f dB, BPP = %1.2f\n",
               TAG.c_str(), calPSNR(Im, IMG), bpp[i]);
        vector<BYTE> Dr;
        pe_img_recover_main(img, Dr, thres[i]);
        vector<vector<BYTE>> Ir = img;
        if (calPSNR(Ir, IMG) != INFINITY ||
            !equal(Dr.begin(), Dr.end(), Do.begin())){
            cout << "FATAL ERROR" <<endl;
        }
        else{
            cout << "CORRECT " << endl;
        }
        printf("[PSNR(Ir,Io) = %f, ", calPSNR(Ir, IMG));
        cout <<"Data Recover:" <<boolalpha << equal(Dr.begin(), Dr.end(), Do.begin()) << "]" << endl<< endl;
    }
}