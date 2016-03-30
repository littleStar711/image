Reversible Data Hiding C++ Edition
===================================

===================================
Util.hpp
Description:
Useful struct and function.
Containing image reading and writing, 
grey image processing and useful equation calculating.
=====================================
Class: BMP_FILE_HEADER
Class: BMP_INFO
Class: RGB_QUAD

Function: readFile
Description:
Read random bit stream from disk into RAM
@Param: const string& filePath,
	const size_t& bitNum
@Return: const vector<BYTE>

Function: writeFile
Description:
Create random bit stream and store into disk
@Param: const string& filePath,
	const size_t& bitNum
@Return: bool

Function: readBMP
Description:
 Read grey BMP image from disk into RAM
@Param: const string& filePath,
	vector<vector<BYTE>>& image,
	BMP_FILE_HEADER* bmpheader,
	BMP_INFO* bmpinfo,
	vector<RGB_QUAD>& bmpquad
@Return: bool

Function: writeBMP
Description:
Write grey BMP image from RAM into disk
@Param: const string& filePath
	const vector<vector<BYTE>>& image,
	BMP_FILE_HEADER* bmpheader,
	BMP_INFO* bmpinfo,
	const vector<RGB_QUAD>& bmpquad
@Return: bool

Function: printBMP_FILE_HEADER
Description:
Print BMP_FILE_HEADER into console
@Param: const BMP_FILE_HEADER* bmp_header
@Return: void

Function: printBMP_INFO
Description:
Print BMP_INFO into console
@Param: const BMP_INFO* bmp_info
@Return: void

Function: printVector
Description:
A tempplate function
Print values of a vector
@Param: const vector<T>& vec,
	const string& msg
@Return: void

Function: printImage
Description:
A template function
Print grey BMP image
@Param: const vector<T>& image,
	const string& msg
@Return: void

Function: calHistogram
Description:
A template function
Calculate 8 bit depth grey BMP image histogram distribution
@Param: const vector<vector<T>> &image 
@Return: const vector<int>

Function: calMSE
Description:
Calculate grey BMP image MSE(Meas Square Error)
@Param: const vector<vector<BYTE>> &image1
	const vector<vector<BYTE>> &image2
@Return: double: MSE

Function: calPSNR
Description:
Calculate grey BMP image PSNR(Peak Signal to Nosie Ratio)
@Param: const vector<vector<BYTE>> &image1
	const vector<vector<BYTE>> &image2
@Return: double: PSNR

Function: calEntropy
Description:
Calculate grey BMP image entropy
@Param: const vector<vector<BYTE>>& image
@Return: double: entropy

Function: byte2Double
Description:
Transfer byte type grey BMP image into double type grey BMP image
@Param: const vector<vector<int>> &image
@Return: vector<vector<double>>: double type grey image

Function: double2Byte
Description:
Transfer double type grey BMP image into byte type grey BMP image
@Param: const vector<vector<double>> &image
@Return: vector<vector<BYTE>>: BYTE type grey image

Function: byte2Int
Description:
Transfer byte type grey BMP image into int type grey BMP image
@Param: const vector<vector<BYTE>>& image
@Return: vector<vector<int>>: int type grey image

Function: int2Byte
Description:
Transfer int type grey BMP image into byte type grey BMP image
@Param: const vector<vector<int>>& image
@Return: vector<vector<BYTE>>: BYTE type grey image

Function: maxValue
Description:
A template function
Find the max value of grey BMP image
@Param: const vector<vector<T>>& image
@Return: T

Function: minValue
Description:
A template function
Find the min value of grey BMP image
@Param: const vector<vector<T>>& image
@Return: T

==========================================
Color.hpp
Description:
Containing color image reading, writing and transferring.
==========================================
Class: RGB_PIXEL

Function: readRGB
Description:
Read colour BMP image from storage
@Param: const string& filePath,
	vector<vector<RGB_PIXEL>>& image,
	BMP_FILE_HEADER* bmpheader,
	BMP_INFO* bmpinfo,
	vector<RGB_QUAD>& bmpquad
@Return: bool

Function: writeRGB
Description:
Write colour BMP image into storage
@Param: const string& filePath
	const vector<vector<RGB_PIXEL>>& image,
	const BMP_FILE_HEADER* bmpheader,
	const BMP_INFO* bmpinfo,
	const vector<RGB_QUAD>& bmpquad
@Return: bool

Function: calPSNR
Description:
Calculate colour BMP image PSNR, overload
@Param: const vector<vector<RGB_PIXEL>>& image1,
	const vector<vector<RGB_PIXEL>>& image2
@Return: double: PSNR

Function: calMSE
Description:
Calculate Colour BMP image MSE, overload
@Param: const vector<vector<RGB_PIXEL>>&,
        const vector<vector<RGB_PIXEL>>&);
@Return: double: MSE

Function: getRGBLevel
Description:
Get RGB level
@Param: const vector<vector<RGB_PIXEL>>& image,
	const size_t& level
@Return: vector<vector<BYTE>>

Function: clr2rgb
Description:
Transfer RGB_PIXEL type colour BMP image into int type RGB image
@Param: const vector<vector<RGB_PIXEL>>& image,
	vector<vector<int>>& imgR,
	vector<vector<int>>& imgG,
	vector<vector<int>>& imgB
@Return: bool

Function: rgb2clr
Description:
Transfer int type RGB image into PEG_PIXEL type colour BMP image
@Param: const vector<vector<int>>& imgR,
	const vector<vector<int>>& imgG,
	const vector<vector<int>>& imgB,
	vector<vector<RGB_PIXEL>>& image
@Return: bool

Function: clr2ycbcr
Description:
Transfer RGB_PIXEL type colour BMP image into YCbCr image
@Param: const vector<vector<RGB_PIXEL>>& image,
	vector<vector<int>>& imgY,
	vector<vector<int>>& imgCb,
	vector<vector<int>>& imgCr,
	vector<vector<double>>& imgYf,
	vector<vector<double>>& imgCbf,
	vector<vector<double>>& imgCrf
@Return: bool

Function: ycbcr2clr
Description:
Transfer int and double type YCbCr image into RGB_PIXEL type colour BMP image
@Param: const vector<vector<int>>& imgY,
	const vector<vector<int>>& imgCb,
	const vector<vector<int>>& imgCr,
	const vector<vector<double>>& imgYf,
	const vector<vector<double>>& imgCbf,
	const vector<vector<double>>& imgCrf,
	vector<vector<RGB_PIXEL>>& image
@Return: bool