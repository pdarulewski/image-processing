#include "functions.h"
#include <cmath>
#include <utility>
#include <numeric>
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <functional>
#include <Windows.h>

//// ---------------------------------------------------------Helper functions------------------------------------------------------------------

ImageF fillEdges(ImageF& im)
{
	ImageF newIm(im.width() + 2, im.height() + 2, im.depth(), im.spectrum(), 0);  // Image with additional edges.

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				newIm(i + 1, j + 1, k) = im(i, j, k);

	for (int k = 0; k < im.spectrum(); k++)
	{
		for (int i = 0; i < im.width(); i++)
		{
			newIm(i, 0, k) = im(i, 0, k);
			newIm(i, newIm.height() - 1, k) = im(i, im.height() - 1, k);
		}

		for (int j = 0; j < im.height(); j++)
		{
			newIm(0, j, k) = im(0, j, k);
			newIm(newIm.width() - 1, j, k) = im(im.width() - 1, j, k);
		}
	}

	return newIm;
}

ImageUI fillEdges(ImageUI& im)
{
    ImageUI newIm(im.width() + 2, im.height() + 2, im.depth(), im.spectrum(), 0);  // Image with additional edges.

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                newIm(i + 1, j + 1, k) = im(i, j, k);

    for (int k = 0; k < im.spectrum(); k++)
    {
        for (int i = 0; i < im.width(); i++)
        {
            newIm(i, 0, k) = im(i, 0, k);
            newIm(i, newIm.height() - 1, k) = im(i, im.height() - 1, k);
        }

        for (int j = 0; j < im.height(); j++)
        {
            newIm(0, j, k) = im(0, j, k);
            newIm(newIm.width() - 1, j, k) = im(im.width() - 1, j, k);
        }
    }

    return newIm;
}

float condition(float value)
{
	if (value < 0.0) return 0.0;
	if (value > 255.0) return 255.0;
	return value;
}

int condition(int value)
{
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

double condition(double value)
{
    if (value < 0.0) return 0.0;
    if (value > 255.0) return 255.0;
    return value;
}

float mx(ImageF& im)
{
	float mx = 0.0;

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				if (im(i, j, k) > mx) mx = im(i, j, k);

	return mx * mx;
}

long double mx(ImageD& im)
{
    double mx = 0.0;

    for (size_t i = 0; i < im.width(); i++)
        for (size_t j = 0; j < im.height(); j++)
            for (size_t k = 0; k < im.spectrum(); k++)
                if (im(i, j, k) > mx) mx = im(i, j, k);

    return mx;
}

int calcpix(ImageUI& im)
{
	return im.width() * im.height();
}

ImageUI copyEdges(ImageUI& im, ImageUI& newIm)
{
    for (int k = 0; k < im.spectrum(); k++)
    {
        for (int i = 0; i < im.width(); i++)
        {
            newIm(i, 0, k) = im(i, 0, k);
            newIm(i, im.height() - 1, k) = im(i, im.height() - 1, k);
        }

        for (int j = 0; j < im.height(); j++)
        {
            newIm(0, j, k) = im(0, j, k);
            newIm(im.width() - 1, j, k) = im(im.width() - 1, j, k);
        }
    }

    return newIm;
}

void rotate(ComplVec2D& data)
{
    ComplVec2D output(data.size());

    for (size_t i = 0; i < data.size(); i++)
    {
        ComplVec temp(data[i].size());

        for (size_t j = 0; j < data.size(); j++)
            temp[j] = data[j][i];

        output[i] = temp;
    }

    data = output;
}

ComplVecSpec imageToMatrix(ImageD& im)
{
    ComplVecSpec output(im.spectrum());

    for (size_t k = 0; k < im.spectrum(); k++)
    {
        ComplVec2D temp2D(im.height());

        for (size_t i = 0; i < im.height(); i++)
        {
            ComplVec temp1D(im.width());

            for (size_t j = 0; j < im.width(); j++)
                temp1D[j] = std::complex<double> {im(i, j, k), 0.0};

            temp2D[i] = temp1D;
        }

        output[k] = temp2D;
    }

    return output;
}

ImageD matrixToImage(const ComplVecSpec& data)
{
    ImageD im(data[0][0].size(), data[0].size(), 1, data.size(), 0);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) = condition(abs(data[k][i][j]));

    return im;
}

ImageD logRescale(ImageD& im)
{
    const long double max = mx(im);

    const long double rescaleFactor = 255.0 / (log(1.0 + max));

    long double pixel = 0;

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
            {
                pixel = abs(im(i, j, k));
                im(i, j, k) = log(1.0 + pixel) * rescaleFactor;
            }

    return im;
}

void swapParts(ImageD& im)
{
    ImageD tempIm = im;

    size_t midWidth = im.width() / 2;
    size_t midHeight = im.height() / 2;

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < midWidth; i++)
            for (size_t j = 0; j < midHeight; j++)
            {
                tempIm(i,j,k) = im(midWidth + i, midHeight + j, k);
                tempIm(midWidth + i, midHeight + j, k) = im(i, j, k);
                tempIm(midWidth + i, j, k) = im(i, midHeight + j, k);
                tempIm(i, midHeight + j, k) = im(midWidth + i, j, k);
            }

    im = tempIm;
}

//// ---------------------------------------------------------Elementary operations-------------------------------------------------------------

void brightness(ImageF& im, float value)
{
	if (value == 0.0) return;

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				im(i, j, k) = condition(im(i, j, k) + value);
}

void contrastLinear(ImageF& im, float value)
{
	if (value == 1.0) return;
	value = condition(value);

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				im(i, j, k) = condition(value * (im(i, j, k) - 128) + 128);
}

void contrastPower(ImageF& im, float value)
{
    if (value <= 0.0) throw "Value must be greater than 0.0!";

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                im(i, j, k) = static_cast<float>(pow(im(i, j, k) / 255.0, value) * 255.0);
}

void negative(ImageF& im)
{
	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				im(i, j, k) = static_cast<float>(255.0 - im(i, j, k));
}

//// --------------------------------------------------------Geometric operations-------------------------------------------------------------------

void hflip(ImageF& im)
{
	for (int i = 0; i < im.width() / 2; i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				std::swap(im(i, j, k), im(im.width() - i - 1, j, k));
}

void vflip(ImageF& im)
{
	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height() / 2; j++)
			for (int k = 0; k < im.spectrum(); k++)
				std::swap(im(i, j, k), im(i, im.height() - j - 1, k));
}

void dflip(ImageF& im)
{
	hflip(im);
	vflip(im);
}

void changeResolution(ImageF& im, float value)
{
	if (value == 1.0) return;
	if (value < 0.0) throw "You cannot change the resolution by a negative value!";

	unsigned newWidth = static_cast <unsigned> (im.width() * value);
	unsigned newHeight = static_cast <unsigned> (im.height() * value);

    float scaledX = static_cast<float> (im.width() - 1) / newWidth;
    float scaledY = static_cast<float> (im.height() - 1) / newHeight;

    float pixel1 = 0.0, pixel2 = 0.0, pixel3 = 0.0, pixel4 = 0.0;

    int newX = 0, newY = 0;

	ImageF newIm(newWidth, newHeight, im.depth(), im.spectrum(), 0);

	for (int i = 0; i < newIm.width(); i++)
		for (int j = 0; j < newIm.height(); j++)
            for (int k = 0; k < newIm.spectrum(); k++)
            {
                newX = static_cast<int> (scaledX * i);
                newY = static_cast<int> (scaledY * j);

                pixel1 = im(newX, newY, k);
                pixel2 = im(newX + 1, newY, k);
                pixel3 = im(newX, newY + 1, k);
                pixel4 = im(newX + 1, newY + 1, k);

                newIm(i, j, k) = (pixel1 + pixel2 + pixel3 + pixel4) / 4;  // Arithmetic mean.
            }

	im = newIm;
}

void changeResolution(ImageD& im, double value)
{
    if (value == 1.0) return;
    if (value < 0.0) throw "You cannot change the resolution by a negative value!";

    unsigned newWidth = static_cast <unsigned> (im.width() * value);
    unsigned newHeight = static_cast <unsigned> (im.height() * value);

    double scaledX = static_cast<float> (im.width() - 1) / newWidth;
    double scaledY = static_cast<float> (im.height() - 1) / newHeight;

    double pixel1 = 0.0, pixel2 = 0.0, pixel3 = 0.0, pixel4 = 0.0;

    int newX = 0, newY = 0;

    ImageD newIm(newWidth, newHeight, im.depth(), im.spectrum(), 0);

    for (int i = 0; i < newIm.width(); i++)
        for (int j = 0; j < newIm.height(); j++)
            for (int k = 0; k < newIm.spectrum(); k++)
            {
                newX = static_cast<int> (scaledX * i);
                newY = static_cast<int> (scaledY * j);

                pixel1 = im(newX, newY, k);
                pixel2 = im(newX + 1, newY, k);
                pixel3 = im(newX, newY + 1, k);
                pixel4 = im(newX + 1, newY + 1, k);

                newIm(i, j, k) = (pixel1 + pixel2 + pixel3 + pixel4) / 4;  // Arithmetic mean.
            }

    im = newIm;
}

//// --------------------------------------------------------------Noise removal-----------------------------------------------------------------

void alpha(ImageF& im, int d)
{
	float sum{ 0.0 };
    int count{ 0 }, m{ 3 }, n{ 3 }, trim{ static_cast<int>(d / 2) };
    std::array<float, 9> arr;

	if (d >= m * n || d < 0 || d & 1) throw "The parameter has to be a positive even number from the range [0, 8]!";

	ImageF newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
	im = fillEdges(im);

	for (int i = 1; i < im.width() - 1; i++)
		for (int j = 1; j < im.height() - 1; j++)
			for (int k = 0; k < im.spectrum(); k++)
			{
				count = 0;

				for (int x = -1; x < 2; x++)
					for (int y = -1; y < 2; y++)
						arr[count++] = im(i + x, j + y, k);
       
				std::sort(begin(arr), end(arr));

				for (int x = 0; x < trim; x++)
					arr[x] = arr[m * n - 1 - x] = 0.0;

				sum = static_cast<float>(std::accumulate(cbegin(arr), cend(arr), 0.0));
				sum /= ((m * n) - d);

				newIm(i - 1, j - 1, k) = sum;
			}

	im = newIm;
}

void cmean(ImageF& im, float order)
{
    float firstSum{ 0.0 }, secondSum{ 0.0 };
    std::array<float, 9> firstArr, secondArr;
    int count{ 0 };

    ImageF newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    im = fillEdges(im);

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                count = 0;

                for (int x = -1; x < 2; x++)
                    for (int y = -1; y < 2; y++)
                    {
                        firstArr[count] = pow(im(i + x, j + y, k), order + 1);
                        secondArr[count] = pow(im(i + x, j + y, k), order);
                        count++;
                    }

                firstSum = static_cast<float>(std::accumulate(cbegin(firstArr), cend(firstArr), 0.0));
                secondSum = static_cast<float>(std::accumulate(cbegin(secondArr), cend(secondArr), 0.0));

                newIm(i - 1, j - 1, k) = firstSum / secondSum;
            }

    im = newIm;
}

//// ------------------------------------------------------------------Means---------------------------------------------------------------------

void mse(ImageF& im, ImageF& newIm)
{
	if (im.width() != newIm.width() || im.height() != newIm.height() || im.spectrum() != newIm.spectrum()) throw "Sizes of the images are different!";

    float result[3] {};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				result[k] += ((im(i, j, k) - newIm(i, j, k)) * (im(i, j, k) - newIm(i, j, k)));

    for (int k = 0; k < im.spectrum(); k++)
        std::cout << "For spectrum " << k << ": " << result[k] / (im.width() * im.height()) << std::endl;
}

void pmse(ImageF& im, ImageF& newIm)
{
    if (im.width() != newIm.width() || im.height() != newIm.height() || im.spectrum() != newIm.spectrum()) throw "Sizes of the images are different!";

    float result[3] {};

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                result[k] += ((im(i, j, k) - newIm(i, j, k)) * (im(i, j, k) - newIm(i, j, k)));

    for (int k = 0; k < im.spectrum(); k++)
        std::cout << "For spectrum " << k << ": " << result[k] / (im.width() * im.height() * mx(im)) << std::endl;
}

void snr(ImageF& im, ImageF& newIm)
{
	if (im.width() != newIm.width() || im.height() != newIm.height() || im.spectrum() != newIm.spectrum()) throw "Sizes of the images are different!";

    float a[3] {};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				a[k] += (im(i, j, k) * im(i, j, k));

    float result[3]{};

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                result[k] += ((im(i, j, k) - newIm(i, j, k)) * (im(i, j, k) - newIm(i, j, k)));

    for (int k = 0; k < im.spectrum(); k++)
        std::cout << "For spectrum " << k << ": " << 10 * log10(a[k] / result[k]) << std::endl;
}

void psnr(ImageF& im, ImageF& newIm)
{
	if (im.width() != newIm.width() || im.height() != newIm.height() || im.spectrum() != newIm.spectrum()) throw "Sizes of the images are different!";

    float a = 0.0;
	float b = mx(im);

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
				a += b;

    float result[3] {};

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                result[k] += ((im(i, j, k) - newIm(i, j, k)) * (im(i, j, k) - newIm(i, j, k)));

    for (int k = 0; k < im.spectrum(); k++)
        std::cout << "For spectrum " << k << ": " << 10 * log10(a / result[k]) << std::endl;
}

void md(ImageF& im, ImageF& newIm)
{
	if (im.width() != newIm.width() || im.height() != newIm.height() || im.spectrum() != newIm.spectrum()) throw "Sizes of the images are different!";

    float a[3] {};
    float result[3] {};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
			{
				a[k] = abs((im(i, j, k) - newIm(i, j, k)));
				if (a[k] > result[k]) result[k] = a[k];
			}

    for (int k = 0; k < im.spectrum(); k++)
        std::cout << "For spectrum " << k << ": " << result[k] << std::endl;
}

//// ---------------------------------------------------------------------Histogram----------------------------------------------------------------

ImageUI histogram(ImageUI& im, int channel)
{
    auto mainSpectrum = im.spectrum();

    if (mainSpectrum == 3 && (channel < 1 || channel > 3)) throw "Channel must be from the range [1, 3] for coloured images!";
    if (mainSpectrum == 1 && channel != 1) throw "Channel must be exactly 1 for greyscale images!";

    channel--; // This is to "fix" the user-friendly value provided by the user.

    std::array<int, 256> hist{};

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            hist[im(i, j, channel)]++;

    ImageUI imHist(512, 256, 1, mainSpectrum, 255);  // 255 to make the whole image white.

    auto max = *std::max_element(cbegin(hist), cend(hist));

    for (auto& x : hist)
        x = static_cast<int>((static_cast<float>(x) / max) * 256); // Scaling the values. This causes the maximum value to be 256, which makes it reach the upper border of histogram image.

    if (mainSpectrum == 1)  // For greyscale images.
        for (unsigned i = 0, z = 0; i < hist.size(); i++, z+= 2)
            for (int j = 256 - hist[i]; j < 256; j++)  // This is to reverse a histogram and to avoid using costly flips.
            {                                          // z variable helps to create two columns for one pixel value to make the histogram look clearer.
                imHist(z, j, 0) = 0;
                imHist(z + 1, j, 0) = 0;
            }
               
    else // For colour images.
         for (unsigned i = 0, z = 0; i < hist.size(); i++, z += 2)
                for (int j = 256 - hist[i]; j < 256; j++)  // This is to reverse a histogram and to avoid using costly flips.
                {
                    if (channel != 0)                      // If this is not the channel we are interested in, we set its intensity to 0 to bring out the colour which is the most important.
                    {                                      // This way histograms for red channel are red, for green channel are green and so on.
                        imHist(z, j, 0) = 0; 
                        imHist(z + 1, j, 0) = 0;
                    } 

                    if (channel != 1)
                    {
                        imHist(z, j, 1) = 0;
                        imHist(z + 1, j, 1) = 0;
                    }

                    if (channel != 2)
                    {
                        imHist(z, j, 2) = 0;
                        imHist(z + 1, j, 2) = 0;
                    }
                }
   
    return imHist;
}

void hhyper(ImageUI& im, int gmin, int gmax)
{
    if (gmin > gmax) throw "Gmax has to be greater or equal to gmin!";
    if (gmin <= 0) throw "Gmin has to be from the range [0, 255]!";
    if (gmax >= 255) throw "Gmax has to be from the range [0, 255]!";

    ImageUI newIm(im.width(), im.height(), 1, im.spectrum(), 0);
    int hist[3][256] {};

    auto shortCalcpix = calcpix(im);

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                hist[k][im(i, j, k)]++;

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                float sum = 0.0f;

                for (unsigned m = 0; m <= im(i, j, k); m++)
                    sum += hist[k][m];

                sum /= shortCalcpix;

                int newPixel = static_cast<int> (gmin * pow((static_cast<float> (gmax) / gmin), sum));

                newIm(i, j, k) = static_cast<int> (condition(static_cast<float> (newPixel)));
            }

    im = newIm;
}

void intensityhhyper(ImageUI& im, int gmin, int gmax)
{
    if (gmin > gmax) throw "Gmax has to be greater or equal to gmin!";
    if (gmin <= 0) throw "Gmin has to be from the range [0, 255]!";
    if (gmax >= 255) throw "Gmax has to be from the range [0, 255]!";

    ImageUI intensityIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    ImageF newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    int hist[3][256]{};

    auto shortCalcpix = calcpix(im);

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                intensityIm(i, j, k) = (im(i, j, 0) + im(i, j, 1) + im(i, j, 2)) / 3;

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                hist[k][intensityIm(i, j, k)]++;


    for (int i = 0; i < intensityIm.width(); i++)
        for (int j = 0; j < intensityIm.height(); j++)
            for (int k = 0; k < intensityIm.spectrum(); k++)
            {
                float sum = 0.0f;

                for (unsigned m = 0; m <= intensityIm(i, j, k); m++)
                    sum += hist[k][m];

                sum /= shortCalcpix;

                int newPixel = static_cast<int> (gmin * pow((static_cast<float> (gmax) / gmin), sum));

                newIm(i, j, k) = condition(static_cast<float> (newPixel));

                newIm(i, j, k) = condition(static_cast<float> (im(i, j, k) * (newIm(i, j, 0) / intensityIm(i, j, 0))));
            }

    im = newIm;
}

//// ----------------------------------------------------------------------Evaluation--------------------------------------------------------------

void mean(ImageUI& im) // "mean", because "cmean" is occupied already
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += m * hist[k][m];

	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << result[k] / shortCalcpix << std::endl;
}

void cvariance(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	float mean[3]{};
	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			mean[k] += m * hist[k][m];

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 2) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << result[k] / shortCalcpix << std::endl;
}

void cstdev(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	float mean[3]{};
	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			mean[k] += m * hist[k][m];

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 2) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << sqrt(result[k] / shortCalcpix) << std::endl;
}

void cvarcoi(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	float mean[3]{};
	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			mean[k] += m * hist[k][m];

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 2) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << sqrt(result[k] / shortCalcpix) / (mean[k] / shortCalcpix) << std::endl;
}

void casyco(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	float mean[3]{};
	float cstdev[3]{};
	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			mean[k] += m * hist[k][m];

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			cstdev[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 2) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 3) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << result[k] / (shortCalcpix * pow(sqrt(cstdev[k] / shortCalcpix), 3)) << std::endl;
}

void cflaco(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	float mean[3]{};
	float cstdev[3]{};
	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			mean[k] += m * hist[k][m];

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			cstdev[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 2) * hist[k][m]);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(m - (mean[k] / shortCalcpix), 4) * hist[k][m] - 3);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << result[k] / (shortCalcpix * pow(sqrt(cstdev[k] / shortCalcpix), 4)) << std::endl;
}

void cvarcoii(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			result[k] += static_cast<float>(pow(hist[k][m], 2));

	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << static_cast<float>(result[k] / pow(shortCalcpix, 2)) << std::endl;
}

void centropy(ImageUI& im)
{
	int hist[3][256]{};
	float result[3]{};

	for (int i = 0; i < im.width(); i++)
		for (int j = 0; j < im.height(); j++)
			for (int k = 0; k < im.spectrum(); k++)
				hist[k][im(i, j, k)]++;

	int shortCalcpix = calcpix(im);

	for (int k = 0; k < im.spectrum(); k++)
		for (int m = 0; m < 256; m++)
			if (hist[k][m] != 0) result[k] += hist[k][m] * log2(static_cast<float> (hist[k][m]) / shortCalcpix);

	for (int k = 0; k < im.spectrum(); k++)
		std::cout << "For spectrum " << k + 1 << ": " << -result[k] / shortCalcpix << std::endl;
}

//// ---------------------------------------------------------------------Linear filter in spatial domain--------------------------------------------

void sexdeti(ImageUI& im, const std::vector<int>& mask)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    im = fillEdges(im);

    int minValue = std::numeric_limits<int>::max();
    int maxValue = std::numeric_limits<int>::min();

    int counter = 0;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                int newPixel = 0;

                for (int x = -1; x < 2; x++)
                    for (int y = -1; y < 2; y++)
                    {
                        newPixel = newPixel + (im(i + x, j + y, k) * mask[counter]);
                        ++counter;
                    }

                counter = 0;

                if (newPixel < minValue) minValue = newPixel;
                if (newPixel > maxValue) maxValue = newPixel;

                newIm(i - 1, j - 1, k) = newPixel;
            }

    double scale = 255.0 / (maxValue - minValue);
    int shift = -minValue;

    for (int i = 0; i < newIm.width(); i++)
        for (int j = 0; j < newIm.height(); j++)
            for (int k = 0; k < newIm.spectrum(); k++)
                newIm(i, j, k) = static_cast<unsigned> (scale * (newIm(i, j, k) + shift));

    im = newIm;
}

void sexdetio(ImageUI& im)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    im = fillEdges(im);

    int minValue = std::numeric_limits<int>::max();
    int maxValue = std::numeric_limits<int>::min();

    int newPixel = 0;
    int previousEdge = 0;
    int rightEdge = 0;

    for (int k = 0; k < im.spectrum(); k++)
        for (int j = 1, z = 1; j < im.height() - 1; j++, z++)
        {
            previousEdge = im(z - 1, j - 1, k) + im(z - 1, j, k) - im(z - 1, j + 1, k);

            for (int i = 1; i < im.width() - 1; i++)
            {
                rightEdge = im(i + 1, j - 1, k) + im(i + 1, j, k) - im(i + 1, j + 1, k);

                newPixel = rightEdge + previousEdge + im(i, j - 1, k) - 2 * im(i, j, k) - im(i, j + 1, k);

                if (newPixel < minValue) minValue = newPixel;
                if (newPixel > maxValue) maxValue = newPixel;

                newIm(i - 1, j - 1, k) = newPixel;

                previousEdge = rightEdge;
            }
        }

    double scale = 255.0 / (maxValue - minValue);
    int shift = -minValue;

    for (int i = 0; i < newIm.width(); i++)
        for (int j = 0; j < newIm.height(); j++)
            for (int k = 0; k < newIm.spectrum(); k++)
                newIm(i, j, k) = static_cast<unsigned> (scale * (newIm(i, j, k) + shift));

    im = newIm;
}

//// ---------------------------------------------------------------------Non-linear filter in spatial domain-----------------------------------------

void okirsf(ImageUI& im)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    im = fillEdges(im);

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                    int maxSum = 0, maxDifference = 0;

                    int S = 0, T = 0;

                    std::array<int, 8> arr{}; // An array to store the neighbourhood of a currently processed pixel.

                    arr[0] = im(i - 1, j - 1, k);
                    arr[1] = im(i, j - 1, k);
                    arr[2] = im(i + 1, j -1, k);
                    arr[3] = im(i + 1, j, k);
                    arr[4] = im(i + 1, j + 1, k);
                    arr[5] = im(i, j + 1, k);
                    arr[6] = im(i - 1, j + 1, k);
                    arr[7] = im(i - 1, j, k);

                    for (int z = 0; z < 8; z++)
                    {   
                        int difference = 0;

                        S = arr[(z) % 8] + arr[(z + 1) % 8] + arr[(z + 2) % 8];
                        T = arr[(z + 3) % 8] + arr[(z + 4) % 8] + arr[(z + 5) % 8] + arr[(z + 6) % 8] + arr[(z + 7) % 8];

                        difference = abs(5 * S - 3 * T);

                        maxDifference = std::max(maxDifference, difference);
                    }

                    maxSum = condition(std::max(1, maxDifference));

                    newIm(i - 1, j - 1, k) = maxSum;
            }

    im = newIm;
}

void orobertsi(ImageUI& im)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    im = fillEdges(im);

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
                newIm(i -1, j - 1, k) = condition(static_cast<int>(sqrt(pow(im(i, j, k) - im(i + 1, j + 1, k), 2) + pow(im(i, j + 1, k) - im(i + 1, j, k), 2))));

    im = newIm;
}

//// ---------------------------------------------------------------------Basic morphological operations-----------------------------------------------

struct StructeringElement
{
    std::vector<std::vector<int>> mask_{ 
                                         {0,0,0,
                                          0,1,1,
                                          0,0,0}, // I

                                         {0,0,0,
                                          0,1,0,
                                          0,1,0}, // II

                                         {1,1,1,
                                          1,1,1,
                                          1,1,1}, // III

                                         {0,1,0,
                                          1,1,1,
                                          0,1,0}, // IV                           1's say that the pixel should ba changed, 0's mean "do nothing"

                                         {0,0,0,
                                          0,1,1,
                                          0,1,0}, // V

                                         {0,0,0,
                                          1,0,1,
                                          0,0,0}, // VI

                                         {0,0,0,
                                          1,1,1,
                                          0,0,0}, // VII

                                         {0,0,0,
                                          0,0,1,
                                          0,1,0}, // VIII

                                         {0,0,0,
                                          1,1,0,
                                          1,0,0}, // IX

                                         {0,1,1,
                                          0,1,0,
                                          0,0,0}  // X

    };
};

struct HMTElementXI
{
    std::vector<std::vector<int>> mask_{
                                         {255, -1, -1,
                                          255, 0, -1,
                                          255, -1 , -1},

                                         {255, 255, 255,
                                          -1, 0, -1,
                                          -1, -1, -1},

                                         {-1, -1, 255,      // -1's mean inactive points, 255's white pixels and 0's black pixels
                                          -1, 0, 255,
                                          -1,-1, 255},

                                         {-1, -1, -1,
                                          -1, 0, -1,
                                          255, 255, 255}
    };

};

struct HMTElementXII
{
    std::vector<std::vector<int>> mask_{
                                        { 0, 0, 0,
                                         -1, 255, -1,
                                        255, 255 , 255 },

                                        { -1, 0, 0,
                                         255, 255, 0,
                                        255, 255, -1 },

                                        { 255, -1, 0,      // -1's mean inactive points, 255's white pixels and 0's black pixels
                                        255, 255, 0,
                                        255,-1, 0 },

                                        { 255, 255, -1,
                                        255, 255, 0,
                                        -1, 0, 0 },

                                        { 255, 255, 255,
                                        -1, 255, -1,
                                        0, 0, 0 },

                                        { -1, 255, 255,
                                        0, 255, 255,
                                        0, 0, -1 },

                                        { 0, -1, 255,     
                                        0, 255, 255,
                                        0,-1, 255 },

                                        { 0, 0, -1,
                                        0, 255, 255,
                                        -1, 255, 255 }
    };
};

struct HMTXIIReversed 
{
    std::vector<std::vector<int>> mask_{
                                        { 255, 255, 255,
                                        -1, 0, -1,
                                        0, 0, 0 },

                                        { -1, 255, 255,
                                        0, 0, 255,
                                        0, 0, -1 },

                                        { 0, -1, 255,      // -1's mean inactive points, 255's white pixels and 0's black pixels
                                        0, 0, 255,
                                        0,-1, 255 },

                                        { 0, 0, -1,
                                        0, 0, 255,
                                        -1, 255, 255 },

                                        { 0, 0, 0,
                                        -1, 0, -1,
                                        255, 255, 255 },

                                        { -1, 0, 0,
                                        255, 0, 0,
                                        255, 255, -1 },

                                        { 255, -1, 0,
                                        255, 0, 0,
                                        255,-1, 0 },

                                        { 255, 255, -1,
                                        255, 0, 0,
                                        -1, 0, 0 }
    };
};

struct Pruning
{
    std::vector<std::vector<int>> mask_{
                            { 255, -1, -1,
                            255, 0, 255,
                            255, 255, 255 },

                            { 255, 255, 255,
                            255, 0, -1,
                            255, 255, -1 },

                            { 255, 255, 255,      // -1's mean inactive points, 255's white pixels and 0's black pixels
                            255, 0, 255,
                            -1,-1, 255 },

                            { -1, 255, 255,
                            -1, 0, 255,
                            255, 255, 255 },

                            { -1, -1, 255,
                            255, 0, 255,
                            255, 255, 255 },

                            { 255, 255, -1,
                            255, 0, -1,
                            255, 255, 255 },

                            { 255, 255, 255,
                            255, 0, 255,
                            255,-1, -1 },

                            { 255, 255, 255,
                            -1, 0, 255,
                            -1, 255, 255 }
                        };
};

ImageUI dilation(ImageUI& im, int elem)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    newIm = copyEdges(im, newIm);

    StructeringElement se;

    auto mask = se.mask_[elem - 1];

    int counter = 0;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
                if (im(i, j, k) == 0)
                {
                    for (int y = -1; y < 2; y++)
                        for (int x = -1; x < 2; x++)
                        {
                            if (mask[counter] != 0) newIm(i + x, j + y, k) = 255;
                            counter++;
                        }

                    counter = 0;
                }

    return newIm;
}

ImageUI erosion(ImageUI& im, int elem)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 255);
    newIm = copyEdges(im, newIm);

    StructeringElement se;

    auto mask = se.mask_[elem - 1];

    int counter = 0;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
                if (im(i, j, k) == 0)
                {
                    for (int y = -1; y < 2; y++)
                        for (int x = -1; x < 2; x++)
                        {
                            if (mask[counter] != 0) newIm(i + x, j + y, k) = 0;
                            counter++;
                        }

                    counter = 0;
                }

    return newIm;
}

ImageUI opening(ImageUI& im, int elem)
{
    return dilation(erosion(im, elem), elem);
}

ImageUI closing(ImageUI& im, int elem)
{
    return erosion(dilation(im, elem), elem);
}

ImageUI HMT(ImageUI& im, int elem)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    newIm = copyEdges(im, newIm);

    HMTElementXI hmt;

    auto mask = hmt.mask_[elem - 1];

    int counter = 0;
    bool flag = true;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
                {
                for (int y = -1; y < 2; y++)
                    for (int x = -1; x < 2; x++)
                    {
                        if (mask[counter] != -1)
                        {
                            if (mask[counter] != im(i + x, j + y, k))
                            {
                                flag = false;
                                goto endLoop;
                            }
                        }
                        counter++;
                    }

                    endLoop:

                    if (flag) newIm(i, j, k) = 255;

                    counter = 0;
                    flag = true;
                }

    return newIm;
}

ImageUI M6(ImageUI& im, int elem)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    HMTXIIReversed hmt;

    auto mask = hmt.mask_[elem];

    int counter = 0;
    bool flag = true;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                    for (int y = -1; y < 2; y++)
                        for (int x = -1; x < 2; x++)
                        {
                            if (mask[counter] != -1)
                            {
                                if (mask[counter] != im(i + x, j + y, k))
                                {
                                    flag = false;
                                    goto endLoop;
                                }
                            }
                            counter++;
                        }

            endLoop:

                if (flag) newIm(i, j, k) = 255;

                counter = 0;
                flag = true;
            }

        for (int i = 0; i < im.width(); i++)
            for (int j = 0; j < im.height(); j++)
                for (int k = 0; k < im.spectrum(); k++)
                    newIm(i, j, k) = (im(i, j, k) || newIm(i, j, k)) * 255;

    elem++;

    if (elem == 8) return newIm;

    return M6(newIm, elem);
}

ImageUI M3(ImageUI& im, int elem, int x, int y)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    ImageUI tempIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    newIm(x, y, 0) = 255;
    newIm(x, y, 1) = 255;
    newIm(x, y, 2) = 255;

    do
    {
        tempIm = newIm;

        newIm = dilation(newIm, elem);
        
        for (int i = 0; i < im.width(); i++)
            for (int j = 0; j < im.height(); j++)
                for (int k = 0; k < im.spectrum(); k++)
                    newIm(i, j, k) = (newIm(i, j, k) && im(i, j, k)) * 255;


    } while (tempIm != newIm);
    

    return newIm;
}

ImageUI prune(ImageUI& im, int elem)
{
    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    Pruning hmt;

    auto mask = hmt.mask_[elem];

    int counter = 0;
    bool flag = true;

    for (int i = 1; i < im.width() - 1; i++)
        for (int j = 1; j < im.height() - 1; j++)
            for (int k = 0; k < im.spectrum(); k++)
            {
                for (int y = -1; y < 2; y++)
                    for (int x = -1; x < 2; x++)
                    {
                        if (mask[counter] != -1)
                        {
                            if (mask[counter] != im(i + x, j + y, k))
                            {
                                flag = false;
                                goto endLoop;
                            }
                        }
                        counter++;
                    }

            endLoop:

                if (flag) newIm(i, j, k) = 255;

                counter = 0;
                flag = true;
            }

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                newIm(i, j, k) = (im(i, j, k) || newIm(i, j, k)) * 255;

    elem++;

    if (elem == 8) return newIm;

    return prune(newIm, elem);
}

ImageUI regionGrowing(ImageUI& im, int x, int y, int threshold)
{
    if (x < 0 || x > im.width() || y < 0 || y > im.height()) throw "The coordinates are not correct!";

    ImageUI newIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    ImageUI tempIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);
    ImageUI greyIm(im.width(), im.height(), im.depth(), im.spectrum(), 0);

    for (int i = 0; i < im.width(); i++)
        for (int j = 0; j < im.height(); j++)
            for (int k = 0; k < im.spectrum(); k++)
                greyIm(i, j, k) = (im(i, j, 0) + im(i, j, 1) + im(i, j, 2)) / 3;

    im = greyIm;

    int seed = im(x, y, 0);

    newIm(x, y, 0) = 255;
    newIm(x, y, 1) = 255;
    newIm(x, y, 2) = 255;

    long long sum = seed;
    long long elements = 1;

    do
    {
        tempIm = newIm;

        for (int i = 1; i < im.width() - 1; i++)
            for (int j = 1; j < im.height() - 1; j++)
                for (int k = 0; k < im.spectrum(); k++)
                {
                    if (newIm(i, j, k) == 255)
                    {
                        for (int x = -1; x < 2; x++)
                            for (int y = -1; y < 2; y++)
                            {
                                long long difference = abs((sum / elements) - im(i + x, j + y, k));
                                if (difference <= threshold)
                                {
                                    newIm(i + x, j + y, k) = 255;
                                    elements++;
                                    sum += im(i + x, j + y, k);
                                }
                            }
                    }
                }

    } while (tempIm != newIm);

    return newIm;
}

//// ---------------------------------------------------------------------------Fourier transformation-----------------------------------------------------------------------------------

ComplVec DFT1D(ComplVec data, bool inversion)
{
    ComplVec output(data.size());

    const int size = data.size();
    const double sign = inversion ? 1.0 : -1.0;

    std::complex<double> imUnit{ 0.0, sign };

    for (size_t i = 0; i < size; i++)
    {
        std::complex<double> value{};

        for (size_t j = 0; j < size; j++)
            value += data[j] * exp((imUnit * 2.0 * PI * static_cast<double> (i) * static_cast<double> (j)) / static_cast<double> (size));

        value /= static_cast<double> (sqrt(size));

        output[i] = value;
    }

    return output;
}

ComplVec2D DFT2D(ComplVec2D& data, bool inversion)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i] = DFT1D(data[i], inversion);

    rotate(data);

    for (size_t i = 0; i < data.size(); i++)
        data[i] = DFT1D(data[i], inversion);

    rotate(data);

    return data;
}

ComplVecSpec DFT(ComplVecSpec& data, bool inversion)  
{
    ComplVecSpec temp(data.size());

    for (size_t k = 0; k < data.size(); k++)
        temp[k] = DFT2D(data[k], inversion);

    return temp;
}  

ComplVec FFT1D(ComplVec& data, bool inversion)  
{
    const int size = data.size();
    const double sign = inversion ? 1.0 : -1.0;

    std::complex<double> imUnit{ 0.0, sign };

    if (size <= 1) return data;

    ComplVec evenFragments(size / 2);
    ComplVec oddFragments(size / 2);

    for (size_t i = 0; i < size / 2; i++)
    {
        evenFragments[i] = data[2 * i];
        oddFragments[i] = data[2 * i + 1];
    }

    evenFragments = FFT1D(evenFragments, inversion);
    oddFragments = FFT1D(oddFragments, inversion);

    for (size_t i = 0; i < size / 2; i++)
    {
        std::complex<double> temp = exp((imUnit * 2.0 * PI * static_cast<double> (i)) / static_cast<double> (size));
        temp *= oddFragments[i];

        data[i] = evenFragments[i] + temp;
        data[i + size / 2] = evenFragments[i] - temp;
    }

    return data;
}

ComplVec2D FFT2D(ComplVec2D& data, bool inversion)
{
    for (size_t i = 0; i < data.size(); i++)
        data[i] = FFT1D(data[i], inversion);

    rotate(data);

    for (size_t i = 0; i < data.size(); i++)
        data[i] = FFT1D(data[i], inversion);

    rotate(data);

    //if (inversion)
    //{
        for (size_t i = 0; i < data.size(); i++)
            for (size_t j = 0; j < data.size(); j++)
                data[i][j] /= static_cast<double> (sqrt(data.size() * data.size()));
    //}

    return data;
}

ComplVecSpec FFT(ComplVecSpec& data, bool inversion) 
{
    ComplVecSpec temp(data.size());

    for (size_t k = 0; k < data.size(); k++)
        temp[k] = FFT2D(data[k], inversion);

    return temp;
}

ImageD getMagnitude(const ComplVecSpec& data)
{
    ImageD im(data[0].size(), data[0].size(), 1, data.size(), 0);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) = abs(data[k][i][j]);
                
    im = logRescale(im);

    swapParts(im);

    return im;
}

ImageD getPhase(const ComplVecSpec& data)
{
    ImageD im(data[0].size(), data[0].size(), 1, data.size(), 0);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) = arg(data[k][i][j]);

    swapParts(im);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) *= 255.0 / (2 * PI);

    return im;
}

ImageD getReal(const ComplVecSpec& data)
{
    ImageD im(data[0].size(), data[0].size(), 1, data.size(), 0);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) = data[k][i][j].real();

    im = logRescale(im);

    swapParts(im);

    return im;
}

ImageD getImaginary(const ComplVecSpec& data)
{
    ImageD im(data[0].size(), data[0].size(), 1, data.size(), 0);

    for (size_t k = 0; k < im.spectrum(); k++)
        for (size_t i = 0; i < im.height(); i++)
            for (size_t j = 0; j < im.width(); j++)
                im(i, j, k) = data[k][i][j].imag();

    im = logRescale(im);

    swapParts(im);

    return im;
}

void visualise(int what, const ComplVecSpec& data, std::string name)
{
    ImageD im(data[0].size(), data[0].size(), 1, data.size(), 0);
    ComplVecSpec fourierVec = data;

    if (name == "FFT") fourierVec = FFT(fourierVec, false);
    else if (name == "DFT") fourierVec = DFT(fourierVec, false);

    if (what == 1)
    {
        im = getMagnitude(fourierVec);
        im.save("magnitude.bmp");
    }

    else if (what == 2)
    {
        im = getPhase(fourierVec);
        im.save("phase.bmp");
    }

    else if (what == 3)
    {
        im = getReal(fourierVec);
        im.save("real.bmp");
    }

    else if (what == 4)
    {
        im = getImaginary(fourierVec);
        im.save("imaginary.bmp");
    }
}

void lowpassFilter(ComplVecSpec& data, int radius)
{
    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    ImageD magnitude(width, height, 1, spectrum, 0.0);
    ImageD mask(width, height, 1, 1, 0.0);

    data = FFT(data, false);

    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
        {
            if (sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) <= radius) mask(i, j, 0) = 1.0;
            if (sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) <= radius) mask(i, j, 0) = 1.0;
            if (sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) <= radius) mask(i, j, 0) = 1.0;
            if (sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) <= radius) mask(i, j, 0) = 1.0;
        }

    swapParts(mask);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask(i, j, 0);

    magnitude = getMagnitude(data);

    magnitude.save("lowpass_magnitude.bmp");

    data = FFT(data, true);
}

void highpassFilter(ComplVecSpec& data, int radius)
{
    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    ImageD magnitude(width, height, 1, spectrum, 0.0);
    ImageD mask(width, height, 1, 1, 1.0);

    data = FFT(data, false);

    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
        {
            if (sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) <= radius) mask(i, j, 0) = 0.0;
            if (sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) <= radius) mask(i, j, 0) = 0.0;
            if (sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) <= radius) mask(i, j, 0) = 0.0;
            if (sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) <= radius) mask(i, j, 0) = 0.0;
        }

    mask(width / 2, height / 2, 0) = 1.0; // Leaving the DC component untouched.

    swapParts(mask);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask(i, j, 0);

    magnitude = getMagnitude(data);

    magnitude.save("highpass_magnitude.bmp");

    data = FFT(data, true);
}

void bandpassFilter(ComplVecSpec& data, int innerRadius, int outerRadius)
{
    if (innerRadius >= outerRadius) throw "The inner radius should be smaller then the outer one!";

    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    ImageD magnitude(width, height, 1, spectrum, 0.0);
    ImageD mask(width, height, 1, 1, 0.0);

    data = FFT(data, false);

    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
        {
            if (sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) > innerRadius && 
                sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 1.0;

            if (sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) > innerRadius && 
                sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 1.0;

            if (sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) > innerRadius && 
                sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 1.0;

            if (sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) > innerRadius && 
                sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 1.0;
        }

    mask(width / 2, height / 2, 0) = 1.0; // Leaving the DC component untouched.

    swapParts(mask);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask(i, j, 0);

    magnitude = getMagnitude(data);

    magnitude.save("bandpass_magnitude.bmp");

    data = FFT(data, true);
}

void bandcutFilter(ComplVecSpec& data, int innerRadius, int outerRadius)
{
    if (innerRadius >= outerRadius) throw "The inner radius should be smaller then the outer one!";

    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    ImageD magnitude(width, height, 1, spectrum, 0.0);
    ImageD mask(width, height, 1, 1, 1.0);

    data = FFT(data, false);

    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
        {
            if (sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) > innerRadius &&
                sqrt(pow(i - height / 2, 2) + pow(j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 0.0;

            if (sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) > innerRadius &&
                sqrt(pow(height - i - height / 2, 2) + pow(width - j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 0.0;

            if (sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) > innerRadius &&
                sqrt(pow(height - i - height / 2, 2) + pow(j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 0.0;

            if (sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) > innerRadius &&
                sqrt(pow(i - height / 2, 2) + pow(width - j - width / 2, 2)) <= outerRadius) mask(i, j, 0) = 0.0;
        }

    swapParts(mask);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask(i, j, 0);

    magnitude = getMagnitude(data);

    magnitude.save("bandcut_magnitude.bmp");

    data = FFT(data, true);
}

void highpassEdge(ComplVecSpec& data, ImageD& mask)
{
    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    double resize = width / mask.width();

    changeResolution(mask, resize);

    data = FFT(data, false);

    mask(width / 2, height / 2, 0) = 255.0;
    mask(width / 2, height / 2, 1) = 255.0;
    mask(width / 2, height / 2, 2) = 255.0;

    swapParts(mask);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask(i, j, k) / 255.0;

    data = FFT(data, true);
}

void phaseModifyingFilter(ComplVecSpec& data, int x, int y)
{
    int spectrum = data.size();
    int width = data[0].size();
    int height = data[0][0].size();

    std::complex<double> imUnit{ 0.0, 1.0 };

    ComplVec2D mask(height, ComplVec(width, { 0.0, 0.0 }));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            mask[i][j] = exp(imUnit * (-i * x * 2 * PI / height + -j * y * 2 * PI / width + (x + y) * PI));

    data = FFT(data, false);

    for (size_t k = 0; k < spectrum; k++)
        for (size_t i = 0; i < height; i++)
            for (size_t j = 0; j < width; j++)
                data[k][i][j] *= mask[i][j];

    data = FFT(data, true);
}