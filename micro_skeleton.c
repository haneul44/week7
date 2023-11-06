#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


#include <math.h>
#include <stdio.h>
#include <float.h>


void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);

int main()
{
 
	int height;
	int width;
	int channel;

	char command;

 	printf("Take a picture? (y/n)\n");
 	scanf("%c", &command);

 	if(command == 'n')
 		exit(1);


 	printf("Cheeze !\r\n");
	system("raspistill -w 640 -h 480 -t 10 -o image.bmp");
 	
 	
 	unsigned char* imgIn = stbi_load("image.bmp", &width, &height, &channel, 3);


	unsigned char* imgOut_mirror = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_grayScale = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_sobelFiltering = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
 	
	mirror_transform(imgIn, height, width, channel, imgOut_mirror);
	grayScale_transform(imgIn, height, width, channel, imgOut_grayScale);
	sobelFiltering_transform(imgOut_grayScale, height, width, channel, imgOut_sobelFiltering);


	stbi_write_bmp("image_mirror.bmp", width, height, channel, imgOut_mirror);
	stbi_write_bmp("image_grayScale.bmp", width, height, channel, imgOut_grayScale);
	stbi_write_bmp("image_sobelFiltering.bmp", width, height, channel, imgOut_sobelFiltering);
	
	stbi_image_free(imgIn);
	free(imgOut_mirror);
	free(imgOut_grayScale);
	free(imgOut_sobelFiltering);
 

	return 0;
}

void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c;

	for (h = 0; h < height; h++)
	{
		for (w = 0; w < width; w++)
		{
			for (c = 0; c < channel; c++)
			{
				out[h*width*channel+w*channel+c] = in[h*width*channel+((width-w)-1)*channel+c];
			}
		}
	}
}

void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c, avg;

	for (h = 0; h < height; h++)
	{
		for (w = 0; w < width; w++)
		{
			avg = 0;

			for (c = 0; c < channel; c++)
			{
				avg += in[h*width*channel+w*channel+c];
			}
		
			for (c = 0; c < channel; c++)
			{
				out[h*width*channel+w*channel+c] = avg/3;
			}
		}
	}
}

void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	int h, w, c, avg, x_filter, y_filter;
	int sum = 0;
	int x_conv[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
	int y_conv[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };
	unsigned char* gray = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 640 * 480);

	for (h = 0; h < height; h++)
	{
		for (w = 0; w < width; w++)
		{
			avg = 0;

			for (c = 0; c < channel; c++)
			{
				avg += in[h * width * channel + w * channel + c];
			}

			for (c = 0; c < channel; c++)
			{
				gray[h * width * channel + w * channel + c] = avg / 3;
			}
		}
	}
	for (h = 0; h <= height - 2; h++)
	{
		for (w = 0; w <= width - 2; w++)
		{
			x_filter = 0, y_filter = 0;
			c = 0;

			x_filter = x_filter + x_conv[0][0] * gray[h * width * channel + w * channel + c] + x_conv[1][0] * gray[(h + 1) * width * channel + w * channel + c] + x_conv[2][0] * gray[(h + 2) * width * channel + w * channel + c] + x_conv[0][2] * gray[h * width * channel + (w + 2) * channel + c] + x_conv[1][2] * gray[(h + 1) * width * channel + (w + 2) * channel + c] + x_conv[2][2] * gray[(h + 2) * width * channel + (w + 2) * channel + c];
			y_filter = y_filter + y_conv[0][0] * gray[h * width * channel + w * channel + c] + y_conv[0][1] * gray[h * width * channel + (w + 1) * channel + c] + y_conv[0][2] * gray[h * width * channel + (w + 2) * channel + c] + y_conv[2][0] * gray[(h + 2) * width * channel + w * channel + c] + y_conv[2][1] * gray[(h + 2) * width * channel + (w + 1) * channel + c] + y_conv[2][2] * gray[(h + 2) * width * channel + (w + 2) * channel + c];

			sum = abs(x_filter) + abs(y_filter);
				
			out[(h + 1) * width * channel + (w + 1) * channel] = sum;
			out[(h + 1) * width * channel + (w + 1) * channel + 1] = sum;
			out[(h + 1) * width * channel + (w + 1) * channel + 2] = sum;
		}
	}
}
