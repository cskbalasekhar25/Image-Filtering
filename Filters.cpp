//Include all the necessary files for the program

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <stdlib.h>
#include"Pixel.h"

//Funtion identifiers are listed below

void smooth(vector<vector<Pixel> >& image);
void sharpen(vector<vector<Pixel> >& image);
void edgeDetection(vector<vector<Pixel> >& image);

const int MAXWIDTH(5);
const int MAXLEN(1000);
const char newline('\n');
const char terminator('@');
const char nullChar('\0');

/*----------------------------------------------------------------------------------------------*/
/*Function name:   smooth                                                                        */
/*Input Arguments : 2-D vector with Pixel object                                                */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The functions gets vector matrix as the parameter which has the information about the image  */
/*the RGB pixel values are manipulated by the given kernal funtion and the input vector is      */
/*modified                                                                                      */
/*----------------------------------------------------------------------------------------------*/

void smooth(vector<vector<Pixel> >& Image)
{
	//Creating a copy of the input 2-D vector
	int h = Image.size();
	int w = Image[0].size();
	double val = 4;
	vector<vector<Pixel>> mat;
	mat = Image;

	//Creating a Pixel object to store intermediate values
	Pixel sum;

	//A double nested for loop is created and the body contains the logic to manipulate all the elements
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
		{
			//Code for calculation and assignment of the value
			sum = (mat[i + 1][j]) + (mat[i - 1][j]) + (mat[i][j + 1]) + (mat[i][j - 1]);
			sum = (sum / (val));
			Image[i][j] = (sum);

			//This Pixel funtion makes sure the RGB values are under the MAXVAL limit
			Image[i][j].reset();
		}
	cout << "Smoothing Successful" << endl;
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   sharpen                                                                       */
/*Input Arguments : 2-D vector with Pixel object                                                */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The functions gets vector matrix as the parameter which has the information about the image  */
/*to be sharpened. The values are converted to int before getting manipulated by kernam matrix. */
/*It is again converted back to unsigned before getting assigned to Pixel object                */
/*----------------------------------------------------------------------------------------------*/

void sharpen(vector<vector<Pixel> >& image)
{
	//Initialise the values required for the program
	int r = 0;
	int g = 0;
	int b = 0;
	unsigned int triple[3];

	int h = image.size();
	int w = image[0].size();

	//Kernel matrix used for Sharpening
	int sharpen[3][3] = { {-1,-1,-1},{-1,12,-1}, {-1,-1,-1} };

	//Creating a copy of vector matrix
	vector<vector<Pixel>> mat2;
	mat2 = image;


	//Four layered For loop is used to change the RGB pixel values according to sharpen kernel
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
		{
			for (int k = 0; k < 3; k++)
			{

				for (int l = 0; l < 3; l++)
				{
					//RGB values are calculated seperately
					int r1 = (int)(mat2[i + k - 1][j + l - 1].getRed());
					r = r + r1 * sharpen[k][l];

					int g1 = (int)(mat2[i + k - 1][j + l - 1].getGreen());
					g = g + g1 * sharpen[k][l];

					int b1 = (int)(mat2[i + k - 1][j + l - 1].getBlue());
					b = b + b1 * sharpen[k][l];

				}

			}

			//The unsigned value of a negative number is a large positive number and will produce errors
			//If the RGB value is negative, it gets assigned the least possible value
			if (r < 0)
				r = 0;

			if (g < 0)
				g = 0;

			if (b < 0)
				b = 0;

			//Converting int to unsigned for assigning to Pixel object
			triple[0] = (unsigned)r;
			triple[1] = (unsigned)g;
			triple[2] = (unsigned)b;

			//setPixel() funtion assigns the passed value to RGB of Pixel Object respectively
			image[i][j].setPixel(triple[0], triple[1], triple[2]);

			//This Pixel funtion makes sure the RGB values are under the MAXVAL limit
			image[i][j].reset();

			//resetting values for next loop
			r = g = b = 0;
		}
	cout << "Sharpen successful" << endl;
}

/*----------------------------------------------------------------------------------------------*/
/*Function name:   edgeDetection                                                                 */
/*Input Arguments : 2-D vector with Pixel object                                                */
/*Return values : none                                                                          */
/*--------------------------------------DESCRIPTION---------------------------------------------*/
/* The functions gets vector matrix as the parameter which has the information about the image  */
/*These values are manipulated by 2 kernels-horizontal and vertical. The derivative of both the */
/*values are calculated and assigned as the new value              */
/*----------------------------------------------------------------------------------------------*/

void edgeDetection(vector<vector<Pixel> >& image)
{
	//Initialise the values required for the program
	int r = 0, rh = 0, rv = 0;
	int g = 0, gh = 0, gv = 0;
	int b = 0, bh = 0, bv = 0;

	unsigned int triple[3];

	int h = image.size();
	int w = image[0].size();

	//Horizontal and Vertical Kernel
	int hor[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };
	int ver[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };	

	//Creating a copy of the input vector
	vector<vector<Pixel>> mat2;
	mat2 = image;

	//Using for loops to calculate the horizontal and vertical pixel values of the vector
	for (int i = 1; i < h - 1; i++)
	{
		for (int j = 1; j < w - 1; j++)
		{
			for (int k = 0; k < 3; k++)
			{


				for (int l = 0; l < 3; l++)
				{
					int r1 = (int)(mat2[i + k - 1][j + l - 1].getRed());
					rh = rh + r1 * hor[k][l];
					rv = rv + r1 * ver[k][l];
	
					int g1 = (int)(mat2[i + k - 1][j + l - 1].getGreen());
					gh = gh + g1 * hor[k][l];
					gv = gv + g1 * ver[k][l];

					int b1 = (int)(mat2[i + k - 1][j + l - 1].getBlue());
					bh = bh + b1 * hor[k][l];
					bv = bv + b1 * ver[k][l];

				}

			}

			//Calculating the derivative of both kernels
			r = sqrt(pow(rh, 2) + pow(rv, 2));
			g = sqrt(pow(gh, 2) + pow(gv, 2));
			b = sqrt(pow(bh, 2) + pow(bv, 2));

			//The unsigned value of a negative number is a large positive number and will produce errors
			//If the RGB value is negative, it gets assigned the least possible value
			if (r < 0)
				r = 0;

			if (g < 0)
				g = 0;

			if (b < 0)
				b = 0;

			//Converting int to unsigned for assigning to Pixel object
			triple[0] = (unsigned)r;
			triple[1] = (unsigned)g;
			triple[2] = (unsigned)b;

			//setPixel() funtion assigns the passed value to RGB of Pixel Object respectively
			image[i][j].setPixel(triple[0], triple[1], triple[2]);

			//This Pixel funtion makes sure the RGB values are under the MAXVAL limit
			image[i][j].reset();

			//resetting values for next loop
			r = rv = rh = g = gh = gv = b = bh = bv = 0;

		}
	}
	cout << "Edge detection successful" << endl;
}