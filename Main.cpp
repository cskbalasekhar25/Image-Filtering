//Include all the necessary header files required for the Program

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <stdlib.h>
#include"Pixel.h"

using namespace std;

//List all the prototypes of the function to be accessed from main()

void openIOfiles(ifstream& fin, ofstream& fout, char inputFilename[]);
void readHeader(ifstream& fin, ofstream& fout, int imageInfo[]);
void convertP6ToP3(ifstream& bin, ofstream& out, vector<vector<Pixel> >& image, int info[1]);
void writeHeader(ofstream& fout, char magicNumber[], char comment[], int w, int h, int maxPixelVal);
void readAndWriteImageData(ifstream& fin, ofstream& fout, vector<vector<Pixel> >& image, int w, int h);
void writeP3Image(ofstream& out, vector<vector<Pixel> >& image, char comment[], int maxColor);
void smooth(vector<vector<Pixel> >& image);
void sharpen(vector<vector<Pixel> >& image);
void edgeDetection(vector<vector<Pixel> >& image);


/*-----------------------------------------------------------------------------------------------*/
/*Function name:   main                                                                          */
/*Input Arguments : None                                                                         */
/*Return values : 0                                                                              */
/*--------------------------------------DESCRIPTION----------------------------------------------*/
/*This function gets the value of the P6 image file name and firstly converts it into P3 format  */
/*It then calls user-defined functions to perform filtering operations on the image              */
/*-----------------------------------------------------------------------------------------------*/

int main()
{
	//Initialise input and output streams for files
	ifstream f;
	ofstream o,os,osh,oe,oa;

	//Initialise the variables for the program
	char filename[100],fSmooth[100],fSharpen[100],fEdge[100],fAll[100];
	char comments[100] = "#The PPM file after smooth filtering";
	char commentsh[100] = "#The PPM file after sharpen filtering";
	char commente[100] = "#The PPM file after Edge detection filtering";
	char commentall[100] = "#The PPM file after all filtering";
	int maxcolour = 255;
	int info[3];

	//Get the name of the file to open from the user
	cout << "*"<<endl;
	cout << "FILTERING OPERATIONS IN A P6 IMAGE"<<endl;
	cout << "*"<<endl;
	cout << "Enter the name of the P6 image file: ";
	cin >> filename;

	//Pass the input and output file streams and filename for opening the file
	openIOfiles(f, o, filename);
	cout << endl << "File opened" << endl;

	//Input the name of all the output files with the input filename followed by filtering operation

	//Smoothening output file
	strcpy(fSmooth, filename);
	char* loc1 = strchr(fSmooth, '.');
	*loc1 = '\0';
	strcat(fSmooth, "smooth.ppm");

	//Sharpening output file
	strcpy(fSharpen, filename);
	char* loc2 = strchr(fSharpen, '.');
	*loc2 = '\0';
	strcat(fSharpen, "sharpen.ppm");

	//Edge Detection output file
	strcpy(fEdge, filename);
	char* loc3 = strchr(fEdge, '.');
	*loc3 = '\0';
	strcat(fEdge, "edge.ppm");

	//All filters applied
	strcpy(fAll, filename);
	char* loc = strchr(fAll, '.');
	*loc = '\0';
	strcat(fAll, "all.ppm");

	//Create a 2-D vector with Pixel object as an element
	vector<vector<Pixel>> Image;	

	//Call this function to convert P6 to P3 image
	convertP6ToP3(f, o, Image, info);

	//Close the unnecessary files to free up memory
	f.close();

	//Duplicate the image vector to display the results of filtering operations seperately
	
	vector<vector<Pixel>> Smooth;
	vector<vector<Pixel>> Sharpen;
	vector<vector<Pixel>> Edge;
	Smooth = Image;
	Sharpen = Image;
	Edge = Image;

	//Open the output files in which filtering results have to displayed and call the corresponding function followed by writing function

	//Smoothing
	os.open(fSmooth);
	smooth(Smooth);
	writeP3Image(os, Smooth, comments, maxcolour);
	os.close();

	//Sharpening
	osh.open(fSharpen);
	sharpen(Sharpen);
	writeP3Image(osh, Sharpen, commentsh, maxcolour);
	osh.close();

	//Edge Detection
	oe.open(fEdge);
    edgeDetection(Edge);
	writeP3Image(oe, Edge, commente, maxcolour);
	oe.close();

	//All Filtering Operations
	oa.open(fAll);
	smooth(Image);
	sharpen(Image);
	edgeDetection(Image);
	writeP3Image(oa, Image, commentall, maxcolour);
	oa.close();

	//Returning 0
	return 0;
}











