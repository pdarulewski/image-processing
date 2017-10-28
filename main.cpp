#include "functions.h"
#include <iostream>
#include <string>
#include <iterator>

using namespace std;

int main(int argc, char** argv)
{
	ImageF img1, img2;
    ImageUI img3, img4;
    ImageD img5, img6;

	const char* name{ "processed_image.bmp" };

	try
	{

		if (static_cast<string>(argv[1]) == "--help")
		{
            cout << "\n\nHELP MENU:\nThis a program used for processing images.\nThe basic input of the program is <program name> <--command> <value> "
                << "<directory and name of a processed image> (where to save it and with what name, if this argument is omitted the new image will be "
                << "saved in the invoking directory of the program with the name \"processed_image.jpg\". Symbols such as <>, (), [], {} are not allowed.\n\n"
                << "THE LIST OF PROVIDED ACTIONS:\n\n"
                << "1. ELEMENTARY OPERATIONS:\n\n"
                << "a) --brightness <value> <input file> <output file>\t Increases or decreases brightness by a given value. "
                << "You can enter any value but the whole range of brightness is [-255.0, 255.0].\n"
                << "b) --contrastLinear <value> <input file> <output file>\t Increases or decreases contrast by a given value (a linear function).\n "
                << "b)* --contrastBetter <value> <input file> <output file>\t Increases or decreases contrast by a given value (a power function). "
                << "You can enter any value but the whole range of contrast is [0.0, 255.0].\n"
                << "c) --negative <input file> <output file>\t Creates a negative version of an image.\n\n"
                << "2. GEOMETRIC OPERATIONS:\n\n"
                << "a) --hflip <input file> <output file>\t Performs a horizontal flip of an image.\n"
                << "b) --vflip <input file> <output file>\t Performs a vertical flip of an image.\n"
                << "c) --dflip <input file> <output file>\t Performs a diagonal flip of an image.\n"
                << "d) --shrink <value> <input file> <output file>\t Shrinks an image by a given value. "
                << "You can enter any value but the whole range of shrinking values is [0, 1].\n"
                << "e) --enlarge <value> <input file> <output file>\t Enlarges and image by a given value. "
                << "You can enter any value greater or equal than 1 but too large quantities may be processed very long or the program might throw an exception if the value is too big.\n\n"
                << "3. NOISE REMOVAL:\n\n"
                << "a) --alpha <value> <actual image> <processed image>\t Removes noise using an alpha-trimmed mean filter. The value needs to be from the range [0, 8].\n"
                << "b) --cmean <value> <actual image> <processed image>\t Removes noise using a contraharmonic mean filter. The value called the order is a real number.\n\n"
                << "4. ANALYSIS OF NOISE REMOVAL:\n\n"
                << "a) --mse <actual image> <processed image>\t Calculates mean square error.\n"
                << "b) --pmse <actual image> <processed image>\t Calculates peak mean square error.\n"
                << "c) --snr <actual image> <processed image>\t Calculates signal to noise ratio.\n"
                << "d) --psnr <actual image> <processed image>\t Calculates peak signal to noise ratio.\n"
                << "e) --md <actual image> <processed image>\t Calculates maximum difference.\n\n"
                << "5. HISTOGRAM:\n\n"
                << "a) --histogram <value> <input file> <output file>\t Creates a histogram based on a given image. The value corresponds to the channel which you want to bring out in the histogram. Remember that greyscale images have only one channel, whereas colour images can have more than one channel (in case of this program we consider three channels: red, green and blue).\n\n"
                << "6. IMAGE QUALITY IMPROVEMENT:\n\n"
                << "a) --hhyper <value a> <value b> <input file> <output file>\t Improves quality of an image using hyperbolic final probability density function. Values \"a\" and \"b\" are new minimal and maximal brightness values, respectively. Value \"a\" should be smaller or equal to \"b\".\n"
                << "b) --inhhyper <value a> <value b> <input file> <output file>\t Improves quality of an image using hyperbolic final probability density function. This version of the function takes into consideration different values of pixels for different channels and creates an image of a better quality. Values \"a\" and \"b\" are new minimal and maximal brightness values, respectively. Value \"a\" should be smaller or equal to \"b\".\n\n"
                << "7. IMAGE CHARACTERISTICS:\n\n"
                << "a) --mean <input file>\t Calculates mean of a given image.\n"
                << "b) --cvariance <input file>\t Calculates variance of a given image.\n"
                << "c) --cstdev <input file>\t Calculates standard deviation of a given image.\n"
                << "d) --cvarcoi <input file>\t Calculates variation coefficient I of a given image.\n"
                << "e) --casyco <input file>\t Calculates asymmetry coefficient of a given image.\n"
                << "f) --cflaco <input file>\t Calculates flattening coefficient of a given image.\n"
                << "g) --cvarcoii <input file>\t Calculates variation coefficient II of a given image.\n"
                << "h) --centropy <input file>\t Calculates information source entropy of a given image.\n\n"
                << "8. LINEAR IMAGE FILTRATION ALGORITHM:\n\n"
                << "a) --sexdeti <input file> <output file>\t Performs an extraction of details of a given image with the use of a mask specified by a user (only 3x3 masks can be used).\n"
                << "b) --sexdetio <input file> <output file>\t Performd a directed extraction of details in the north direction pf a given image. It works faster than the universal one.\n\n"
                << "9. NON-LINEAR IMAGE FILTRATION ALGORITHM:\n\n"
                << "a) --okirsf <input file> <output file>\t Uses the Kirsch operator mathematial function over the pixels of a given image to extract the details in a nondirected way.\n"
                << "b) --orobertsi <input file> <output file>\t Uses the Robert's operator I mathematical function over the pixels of a given image to extract the details in a nondirected way.\n\n"
                << "10. BASIC MORPHOLOGICAL OPERATIONS:\n\n"
                << "a) --dilation <value> <input file> <output file>\t Performs a dilation operation on a given image using the structering element specified by the \"value\". Available elements are listed from 1 to 10.\n"
                << "b) --erosion <value> <input file> <output file>\t Performs an erosion operation on a given image using the structering element specified by the \"value\". Available elements are listed from 1 to 10.\n"
                << "c) --opening <value> <input file> <output file>\t Performs an opening operation on a given image using the structering element specified by the \"value\". Available elements are listed from 1 to 10.\n"
                << "d) --closing <value> <input file> <output file>\t Performs a closing operation on a given image using the structering element specified by the \"value\". Available elements are listed from 1 to 10.\n"
                << "e) --HMT <value> <input file> <output file>\t Performs a Hit-and-Miss transform on a given image using the structering element specified by the \"value\". Available elements are listed from 1 to 4.\n\n"
                << "11. ADVANCED MORPHOLOGICAL OPERATIONS:\n\n"
                << "a) --M6 <input file> <output file>\t Determines a skeleton of background of a given image by zone of influence using the thickening operation.\n"
                << "b) --prune <input file> <output file>\t Prunes the skeleton obtained by the M6 function in order to produce the SKIZ (Voronoi diagram) of the skeletonised background.\n"
                << "c) --M3 <input file> <value_1> <value_2> <value_3>\t Performs the M3 operation on a given image. The parameters are the number of the mask, x-coordinate, and y-coordinate, respectively.\n\n"
                << "12. IMAGE SEGMENTATION:\n\n"
                << "a) --regionGrowing <value_1> <value_2> <value_3> <input file> <output file>\t Performs the region growing operation on a given image using a specified pixel as a seed. The parameters are x-coordinate, y-coordinate and threshold, respectively. The first two determine the seed point and the threshold tells how big a difference between the seed and a currently considered pixel can be.\n\n"
                << "13. IMAGE FILTRATION IN FREQUENCY DOMAIN:\n\n"
                << "a) --DFT <value> <input file>\t Performs the Discrete Fourier Transform on a given image and creates an output image based on the \"value\". The possible options for \"value\" are: 1 - magnitude, 2 - phase, 3 - real part, 4 - imaginary part.\n"
                << "b) --FFT <value> <input file>\t  Performs the Fast Fourier Transform on a given image and creates an output image based on the \"value\". The possible options for \"value\" are: 1 - magnitude, 2 - phase, 3 - real part, 4 - imaginary part.\n"
                << "c) --lowpass <value> <input file> <output file>\t Filtrates a given image in frequency domain leaving only low frequencies in a radius from the centre of magnitude specified in \"value\". This filter effectively cuts high frequencies.\n"
                << "d) --highpass <value> <input file> <output file>\t Filtrates a given image in frequency domain leaving only high frequencies further from a radius from the centre of magnitude specified in \"value\". This filter effectively cuts low frequencies.\n"
                << "e) --bandpass <value_1> <value_2> <input file> <output file>\t Filtrates a given image in frequency domain cutting frequencies in the centre of the area specified in \"value_1\" and those that are further away from the radius specified in \"value_2\". This filter leaves a band of frequencies.\n"
                << "f) --bandcut <value_1> <value_2> <input file> <output file>\t Filtrates a given image in frequency domain leaving frequencies in the centre of the area specified in \"value_1\" and those that are further away from the radius specified in \"value_2\". This filter cuts a band of frequencies.\n"
                << "g) --highpassEdge <input file> <mask> <output file>\t Filtrates a given image in frequency domain with the use of the specified mask to detect edges in a given image.\n"
                << "h) --modifyPhase <intput file> <value_1> <value_2> <output file>\t Moves in phase the specified parts of a given image. \"Value_1\" and \"value_2\" are used to determine the shift in phase.\n\n";

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--brightness")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			brightness(img1, stof(argv[2]));
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--contrastLinear")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			contrastLinear(img1, stof(argv[2]));
			img1.save(name);

			return 0;
		}

        if (static_cast<string>(argv[1]) == "--contrastPower")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

            if (argc == 5) name = argv[4];
            img1.assign(argv[3]);
            contrastPower(img1, stof(argv[2]));
            img1.save(name);

            return 0;
        }

		if (static_cast<string>(argv[1]) == "--negative")
		{
			if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

			if (argc == 4) name = argv[3];
			img1.assign(argv[2]);
			negative(img1);
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--hflip")
		{
			if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

			if (argc == 4) name = argv[3];
			img1.assign(argv[2]);
			hflip(img1);
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--vflip")
		{
			if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

			if (argc == 4) name = argv[3];
			img1.assign(argv[2]);
			vflip(img1);
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--dflip")
		{
			if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

			if (argc == 4) name = argv[3];
			img1.assign(argv[2]);
			dflip(img1);
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--shrink")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (stof(argv[2]) > 1.0) throw "Too large value, you need to use --enlarge!";
			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			changeResolution(img1, stof(argv[2]));
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--enlarge")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (stof(argv[2]) < 1.0) throw "Too small value, you need to use --shrink!";
			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			changeResolution(img1, stof(argv[2]));
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--alpha")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			alpha(img1, stoi(argv[2]));
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cmean")
		{
			if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

			if (argc == 5) name = argv[4];
			img1.assign(argv[3]);
			cmean(img1, stof(argv[2]));
			img1.save(name);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--mse")
		{
			if (argc != 4) throw "Incorrect syntax. Please, see --help.";

			img1.assign(argv[2]);
			img2.assign(argv[3]);
            cout << "\nMean square error is:\n";
            mse(img1, img2);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--pmse")
		{
			if (argc != 4) throw "Incorrect syntax. Please, see --help.";

			img1.assign(argv[2]);
			img2.assign(argv[3]);
            cout << "\nPeak mean square error is:\n";
            pmse(img1, img2);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--snr")
		{
			if (argc != 4) throw "Incorrect syntax. Please, see --help.";

			img1.assign(argv[2]);
			img2.assign(argv[3]);
            cout << "\nSignal to noise ratio is:\n";
            snr(img1, img2);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--psnr")
		{
			if (argc != 4) throw "Incorrect syntax. Please, see --help.";

			img1.assign(argv[2]);
			img2.assign(argv[3]);
            cout << "\nPeak signal to noise ratio is:\n";
            psnr(img1, img2);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--md")
		{
			if (argc != 4) throw "Incorrect syntax. Please, see --help.";

			img1.assign(argv[2]);
			img2.assign(argv[3]);
            cout << "\nMaximum difference is:\n";
            md(img1, img2);

			return 0;
		}

        if (static_cast<string>(argv[1]) == "--histogram")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img4 = histogram(img3, stoi(argv[2]));
            img4.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--hhyper")
        {
            if (argc == 2 || argc > 6) throw "Incorrect syntax. Please, see --help.";

            if (argc == 6) name = argv[5];
            img3.assign(argv[4]);
            hhyper(img3, stoi(argv[2]), stoi(argv[3]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--inhhyper")
        {
            if (argc == 2 || argc > 6) throw "Incorrect syntax. Please, see --help.";

            if (argc == 6) name = argv[5];
            img3.assign(argv[4]);
            intensityhhyper(img3, stoi(argv[2]), stoi(argv[3]));
            img3.save(name);

            return 0;
        }

		if (static_cast<string>(argv[1]) == "--mean")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nMean is:\n";
			mean(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cvariance")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nVariance is:\n";
			cvariance(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cstdev")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nStandard deviation is:\n";
			cstdev(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cvarcoi")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nVariation coefficient I is:\n";
			cvarcoi(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--casyco")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nAsymmetry coefficient I is:\n";
			casyco(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cflaco")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nFlattening coefficient is:\n";
			cflaco(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--cvarcoii")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nVariation coefficient II is:\n";
			cvarcoii(img3);

			return 0;
		}

		if (static_cast<string>(argv[1]) == "--centropy")
		{
			if (argc != 3) throw "Incorrect syntax. Please, see --help.";

			img3.assign(argv[2]);
			cout << "\nInformation source entropy is:\n";
			centropy(img3);

			return 0;
		}

        if (static_cast<string>(argv[1]) == "--sexdeti")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            cout << "\nPlease, specify the mask row by row (when you are done, type a random character which is neither an integer nor a space / enter and then hit enter):\n";

            istream_iterator<int> myCin(cin), eos;
            vector<int> mask(myCin, eos);

            if (mask.size() != 9) throw "Only 3x3 masks are allowed!";

            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);

            sexdeti(img3, mask);
             
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--sexdetio")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";
         
            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);

            sexdetio(img3);
             
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--okirsf")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);
            okirsf(img3);
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--orobertsi")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);
            orobertsi(img3);
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--dilation")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 10) throw "You can only choose the structering element from 1 to 10!";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img3 = dilation(img3, stoi(argv[2]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--erosion")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 10) throw "You can only choose the structering element from 1 to 10!";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img3 = erosion(img3, stoi(argv[2]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--opening")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 10) throw "You can only choose the structering element from 1 to 10!";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img3 = opening(img3, stoi(argv[2]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--closing")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 10) throw "You can only choose the structering element from 1 to 10!";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img3 = closing(img3, stoi(argv[2]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--HMT")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 4) throw "You can only choose the structering element from 1 to 4!";

            if (argc == 5) name = argv[4];
            img3.assign(argv[3]);
            img3 = HMT(img3, stoi(argv[2]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--M6")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);

            do
            {
                img4 = img3;
                img3 = M6(img3, 0);

            } while (img4 != img3);

            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--M3")
        {
            if (argc == 2 || argc > 7) throw "Incorrect syntax. Please, see --help.";
            if (stoi(argv[3]) < 1 || stoi(argv[3]) > 10) throw "You can only choose the structering element from 1 to 10!";

            if (argc == 7) name = argv[6];
            img3.assign(argv[2]);
            img3 = M3(img3, stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--prune")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (argc == 4) name = argv[3];
            img3.assign(argv[2]);

            do
            {
                img4 = img3;
                img3 = prune(img3, 0);

            } while (img4 != img3);

            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--regionGrowing")
        {
            if (argc == 2 || argc > 7) throw "Incorrect syntax. Please, see --help.";

            if (argc == 7) name = argv[6];
            img3.assign(argv[5]);
            img3 = regionGrowing(img3, stoi(argv[2]), stoi(argv[3]), stoi(argv[4]));
            img3.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--FFT")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 4) throw "You must specify the visualisation argument from 1 to 4!";

            img5.assign(argv[3]);

            ComplVecSpec image = imageToMatrix(img5);

            visualise(stoi(argv[2]), image, "FFT");

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--DFT")
        {
            if (argc == 2 || argc > 4) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 1 || stoi(argv[2]) > 4) throw "You must specify the visualisation argument from 1 to 4!";

            img5.assign(argv[3]);

            ComplVecSpec image = imageToMatrix(img5);

            visualise(stoi(argv[2]), image, "DFT");

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--lowpass")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 0) throw "You cannot specify a negative radius!";

            if (argc == 5) name = argv[4];

            img5.assign(argv[3]);

            ComplVecSpec image = imageToMatrix(img5);

            lowpassFilter(image, stoi(argv[2]));
            
            img5 = matrixToImage(image);
            
            img5.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--highpass")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 0) throw "You cannot specify a negative radius!";

            if (argc == 5) name = argv[4];

            img5.assign(argv[3]);

            ComplVecSpec image = imageToMatrix(img5);

            highpassFilter(image, stoi(argv[2]));

            img5 = matrixToImage(image);

            img5.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--bandpass")
        {
            if (argc == 2 || argc > 6) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 0 || stoi(argv[3]) < 0) throw "You cannot specify a negative radius!";

            if (argc == 6) name = argv[5];

            img5.assign(argv[4]);

            ComplVecSpec image = imageToMatrix(img5);

            bandpassFilter(image, stoi(argv[2]), stoi(argv[3]));

            img5 = matrixToImage(image);

            img5.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--bandcut")
        {
            if (argc == 2 || argc > 6) throw "Incorrect syntax. Please, see --help.";

            if (stoi(argv[2]) < 0 || stoi(argv[3]) < 0) throw "You cannot specify a negative radius!";

            if (argc == 6) name = argv[5];

            img5.assign(argv[4]);

            ComplVecSpec image = imageToMatrix(img5);

            bandcutFilter(image, stoi(argv[2]), stoi(argv[3]));

            img5 = matrixToImage(image);

            img5.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--highpassEdge")
        {
            if (argc == 2 || argc > 5) throw "Incorrect syntax. Please, see --help.";

            if (argc == 5) name = argv[4];

            img5.assign(argv[2]);

            img6.assign(argv[3]);

            ComplVecSpec image = imageToMatrix(img5);

            highpassEdge(image, img6);

            img5 = matrixToImage(image);

            img5.save(name);

            return 0;
        }

        if (static_cast<string>(argv[1]) == "--modifyPhase")
        {
            if (argc == 2 || argc > 6) throw "Incorrect syntax. Please, see --help.";

            if (argc == 6) name = argv[5];

            img5.assign(argv[4]);

            ComplVecSpec image = imageToMatrix(img5);

            phaseModifyingFilter(image, stoi(argv[2]), stoi(argv[3]));

            img5 = matrixToImage(image);

            img5.save(name);

            return 0;
        }

		throw "Incorrect syntax. Please, see --help.";

	}

	catch (const char* e)
	{
	cerr << endl << e << endl;
	cin.ignore();

	return 1;
	}
	
	catch (CImgException& e)
	{
		cerr << endl << e.what() << endl;
		cin.ignore();

		return 2;
	}

	catch (...)
	{
		cerr << "\nUnknown error!" << endl;
		cin.ignore();

		return 3;
	}
}
