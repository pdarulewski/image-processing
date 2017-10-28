#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define _USE_MATH_DEFINES

#include "CImg.h"
#include <string>
#include <vector>
#include <complex>

using namespace cimg_library;

using ImageF = CImg<float>;
using ImageUI = CImg<unsigned int>;
using ImageD = CImg<double>;
using ComplVec = std::vector<std::complex<double>>;
using ComplVec2D = std::vector<ComplVec>;
using ComplVecSpec = std::vector<ComplVec2D>;

const double PI = M_PI;

// -----------------------------------------------------------------------------HELPER FUNCTIONS-------------------------------------------------------------------------------------

float mx(ImageF&);  // Compute the squared maximal value of pixels in the whole image.
long double mx(ImageD&); // Compute the maximal value of pixels in the whole image.
float condition(float); // To prevent values of pixels from going out of bounds [0, 255].
int condition(int);
double condition(double);
ImageF fillEdges(ImageF&); // Extend an image to correctly invoke noise removing functions.
ImageUI fillEdges(ImageUI&);
int calcpix(ImageUI&);
ImageUI copyEdges(ImageUI&, ImageUI&);

void rotate(ComplVec2D&);

ComplVecSpec imageToMatrix(ImageD&);
ImageD matrixToImage(const ComplVecSpec&);
ImageD logRescale(ImageD&);
void swapParts(ImageD&);


// ------------------------------------------------------------------------------TASK 1 ---------------------------------------------------------------------------------------------

void brightness(ImageF&, float);
void contrastLinear(ImageF&, float);
void contrastPower(ImageF&, float);   // MODIFICATION OF THE PROGRAM
void negative(ImageF&);

void hflip(ImageF&);
void vflip(ImageF&);
void dflip(ImageF&);
void changeResolution(ImageF&, float);
void changeResolution(ImageD&, double);

void alpha(ImageF&, int);
void cmean(ImageF&, float);

void mse(ImageF&, ImageF&);
void pmse(ImageF&, ImageF&);
void snr(ImageF&, ImageF&);
void psnr(ImageF&, ImageF&);
void md(ImageF&, ImageF&);

// -------------------------------------------------------------------------------TASK 2 ---------------------------------------------------------------------------------------------------

ImageUI histogram(ImageUI&, int);
void hhyper(ImageUI&, int, int);
void intensityhhyper(ImageUI&, int, int); // MODIFICATION OF THE PROGRAM

void mean(ImageUI&);
void cvariance(ImageUI&);
void cstdev(ImageUI&);
void cvarcoi(ImageUI&);
void casyco(ImageUI&);
void cflaco(ImageUI&);
void cvarcoii(ImageUI&);
void centropy(ImageUI&);

void sexdeti(ImageUI&, const std::vector<int>&);
void sexdetio(ImageUI&);

void okirsf(ImageUI&);
void orobertsi(ImageUI&);  // MODIFICATION OF THE PROGRAM

// -------------------------------------------------------------------------------TASK 3 ---------------------------------------------------------------------------------------------------

struct StructeringElement;
struct HMTElementXI;
struct HMTElementXII;
struct HMTXIIReversed;
struct Pruning;

ImageUI dilation(ImageUI&, int);
ImageUI erosion(ImageUI&, int);
ImageUI opening(ImageUI&, int);
ImageUI closing(ImageUI&, int);
ImageUI HMT(ImageUI&, int);

ImageUI M6(ImageUI&, int);
ImageUI prune(ImageUI&, int);
ImageUI M3(ImageUI&, int, int, int);  // MODIFICATION OF THE PROGRAM

ImageUI regionGrowing(ImageUI&, int, int, int);

// --------------------------------------------------------------------------------TASK 4 ---------------------------------------------------------------------------------------------------------

ComplVec DFT1D(ComplVec, bool);
ComplVec2D DFT2D(ComplVec2D&, bool);
ComplVecSpec DFT(ComplVecSpec&, bool);

ComplVec FFT1D(ComplVec&, bool);
ComplVec2D FFT2D(ComplVec2D&, bool);
ComplVecSpec FFT(ComplVecSpec&, bool);

ImageD getMagnitude(const ComplVecSpec&);
ImageD getPhase(const ComplVecSpec&);
ImageD getReal(const ComplVecSpec&);
ImageD getImaginary(const ComplVecSpec&);

void visualise(int, const ComplVecSpec&, std::string);

void lowpassFilter(ComplVecSpec&, int);
void highpassFilter(ComplVecSpec&, int);
void bandpassFilter(ComplVecSpec&, int, int);
void bandcutFilter(ComplVecSpec&, int, int);
void highpassEdge(ComplVecSpec&, ImageD&);
void phaseModifyingFilter(ComplVecSpec&, int, int);

#endif // FUNCTIONS_H_