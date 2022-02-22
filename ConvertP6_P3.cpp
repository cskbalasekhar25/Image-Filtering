#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <stdlib.h>
#include"Pixel.h"

void openIOfiles(ifstream& fin, ofstream& fout, char inputFilename[]);
void readHeader(ifstream& fin, ofstream& fout, int imageInfo[]);
void convertP6ToP3(ifstream& bin, ofstream& out, vector<vector<Pixel> >& image, int info[1]);
void writeHeader(ofstream& fout, char magicNumber[], char comment[], int w, int h, int maxPixelVal);
void readAndWriteImageData(ifstream& fin, ofstream& fout, vector<vector<Pixel> >& image, int w, int h);
void writeP3Image(ofstream& out, vector<vector<Pixel> >& image, char comment[], int maxColor);

const int MAXWIDTH(5);
const int MAXLEN(1000);
const char newline('\n');
const char terminator('@');
const char nullChar('\0');

/*----------------------------------------------------------------------------------------------*/
/*Function name:   openIOfiles                                                                   */
/*Input Arguments : Input and Output stream of a file, filename                                 */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The functions gets the name of the file to be opened and checks if it can be opened. Error   */
/*message is displayed if it cant be opened. Name of the output file is derived from input file */
/*and opened. A check is done if it is opened properly and error is displayed if not.           */
/*----------------------------------------------------------------------------------------------*/

void openIOfiles(ifstream& fin, ofstream& fout, char inputFilename[])
{
	//Input file is opened
	fin.open(inputFilename, ios::binary);

	//Output file is initialised 
	char* outputFilename;
	outputFilename = (char*)malloc(sizeof(inputFilename) + 10);

	//Check for proper opening of input file
	if (!fin)
	{
		cout << "could not open file: bye" << endl;
		exit(1);
	}

	//Output file is named and opened from input file plus the output format of the image
	strcpy(outputFilename, inputFilename);
	char* loc = strchr(outputFilename, '.');
	*loc = '\0';
	strcat(outputFilename, "P3.ppm");
	fout.open(outputFilename);

	//Check for proper opening of output file
	if (!fout)
	{
		cout << "cannot open output file: bye" << endl;
	}
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   convertP6ToP3                                                                 */
/*Input Arguments : Input and output stream, 2-D vector, int array                              */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The function is used to convert P6 file to P3 format and calls various UDF for the purpose   */
/*----------------------------------------------------------------------------------------------*/

void convertP6ToP3(ifstream& bin, ofstream& out, vector<vector<Pixel> >& image, int info[1])
{
	//Calling the function to read the header of the file
	readHeader(bin, out, info);

	int width = info[0], height = info[1];

	//this function reads and prints the data to the vector and output file
	readAndWriteImageData(bin, out, image, width, height);
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   readHeader                                                                    */
/*Input Arguments : Input and output stream, int array                                          */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The function is used to read the header from the P6 file and checks for the format. It exits */
/* the program if improper format is detected. write header is called to write the info in the  */
/* output file                                                                                  */
/*----------------------------------------------------------------------------------------------*/

void readHeader(ifstream& fin, ofstream& fout, int imageInfo[])
{
	// define and initialise input variables
	char bData[MAXLEN] = { 0 }, magicNumber[MAXWIDTH], comment[MAXLEN] = { "#" };
	int bIndex = 0, charCount = 0, infoCount = 0;
	char ch, aNumber[MAXWIDTH];

	// input first line of text header(magic number)
	// if the magic number is not P6 exit the program
	fin.getline(magicNumber, 3);
	if (strcmp(magicNumber, "P6") != 0)
	{
		cout << "unexpected file format\n";
		exit(1);
	}

	// clear bData array and reset bIndex
	// input next line of text header
	strcpy(bData, " ");
	bIndex = 0;

	fin.getline(bData, MAXLEN);

	do {
		// is this the beginning of a comment
		ch = bData[bIndex];
		if (ch == '#')
		{
			// comment has been read
			// get all characters until a newline is found
			charCount = 0;
			while (ch != terminator && charCount < MAXLEN)
			{
				comment[charCount] = ch;
				++bIndex;
				++charCount;
				ch = bData[bIndex];
				cout << ch;
			}

			if ((charCount == MAXLEN) && (ch != newline))
			{
				cout << "Comment exceeded max length of " << MAXLEN << endl;
				exit(1);
			}

			// get the next line of data
			strcpy(bData, " ");
			bIndex = 0;
			fin.seekg(1, fin.cur);
			fin.getline(bData, MAXLEN);

		}
		else
		{
			// this is not a comment
			// parse bData for image information
			charCount = 0;
			// look past whitespace
			while (bIndex < MAXLEN && isspace(bData[bIndex]))
			{
				++bIndex;
			}
			// may be the beginning of a decimal value
			while (bIndex < MAXLEN && isdigit(bData[bIndex]))
			{
				aNumber[charCount] = bData[bIndex];
				++bIndex;
				++charCount;
				if (charCount == MAXWIDTH)
				{
					cerr << "Maximum width of " << MAXWIDTH << " digits was exceeded.. " << endl;
					exit(1);
				}
			}

			// look at size of aNumber
			if (charCount > 0)
			{
				// we have image information, terminate string
				aNumber[charCount] = nullChar;
				// convert from ascii to integer
				imageInfo[infoCount] = atoi(aNumber);
				++infoCount;
				// verify input
				switch (infoCount)
				{
				case 1: cout << "A width of " << imageInfo[infoCount - 1] << " has been read " << endl;
					break;
				case 2: cout << "A height of " << imageInfo[infoCount - 1] << " has bene read " << endl;
					break;
				case 3: cout << "Maxcolor of " << imageInfo[infoCount - 1] << " has been read " << endl;
					break;
				}
			}
			else if (infoCount < 3)
			{
				// aNumber has 0 digits and infoCount < 3
				// we need more image information
				// get next line of data and parse for image information
				strcpy(bData, " ");
				bIndex = 0;
				fin.getline(bData, MAXLEN);
				//fin.seekg(1, fin.cur);

			}
		}
	} while (infoCount < 3 && !fin.eof());
	if (infoCount < 3)
	{
		cerr << "image information could not be found " << endl;
		exit(1);
	}
	// we have all of the information
	// write header to ascii file
	strcpy(magicNumber, "P3");
	writeHeader(fout, magicNumber, comment, imageInfo[0], imageInfo[1], imageInfo[2]);
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   writeHeader                                                                   */
/*Input Arguments : Output stream, character array with "P3" and custom comment,                */
/*                  3 integers with width and height of the image and max pixel value           */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The function displays the input parameters received into output file                         */
/*----------------------------------------------------------------------------------------------*/

void writeHeader(ofstream& fout, char magicNumber[], char comment[], int w, int h, int maxPixelVal)
{
	// write image imformation to output file
	fout << magicNumber << newline;
	fout << comment << newline;
	fout << w << ' ' << h << ' ' << maxPixelVal << newline;
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   readAndWriteImageData                                                         */
/*Input Arguments : Input and output stream, 2-D vector of image data, image height and width   */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The function reads the value from binary P6 file and assigns it to image vector using        */
/* setPixel()and output file.                                                                   */
/*----------------------------------------------------------------------------------------------*/

void readAndWriteImageData(ifstream& fin, ofstream& fout, vector<vector<Pixel> >& image, int w, int h)
{
	// read and write image data
	// define input variables

	int charCount = 0;
	char colorByte;
	unsigned char aChar;
	unsigned int triple[3];   // red, green, blue

	// allocate memory
	image.resize(h); // allocate h rows
	fin.seekg(0, fin.cur);

	for (int i = 0; i < h; i++)
	{
		image[i].resize(w);// for each row allocate w columns

		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				// read one byte
				fin.read(&colorByte, 1);

				// convert to unsigned char
				aChar = (unsigned char)colorByte;

				// save as unsigned int
				triple[k] = (unsigned int)aChar;

				// write as int
				fout << triple[k] << ' ';
			}
			// CR printed over 10 pixels
			++charCount;
			if (charCount == 10)
			{
				fout << "\r\n";
				charCount = 0;
			}
			image[i][j].setPixel(triple[0], triple[1], triple[2]);
		}
	}
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   writeP3Image                                                                  */
/*Input Arguments : Output stream, 2-D vector of image data, char comment to be displayed in    */
/*                  the image and max value of the pixel                                        */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The function writes the information from image vector to the output file                     */
/*----------------------------------------------------------------------------------------------*/


void writeP3Image(ofstream& out, vector<vector<Pixel> >& image, char comment[], int maxColor)
{
	//Initialise values
	int h, w, pCount(0);
	char magicNumber[3] = "P3";

	h = (int)image.size();
	w = (int)image[0].size();

	//Call the writeHeader function to display the correct details of P3 format in output file
	writeHeader(out, magicNumber, comment, w, h, maxColor);

	//Display the RGB pixel values into the file
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			out << image[i][j];
			++pCount;
			if (pCount == 10)
			{
				out << "\r\n";
				pCount = 0;
			}
			else out << ' ';
		}
	}
}