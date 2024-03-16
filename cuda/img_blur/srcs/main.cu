/**
* Developed By Karan Bhagat
* February 2017
* compile with: nvcc image_stub.cu -o image_stub 
**/

#include "hello.h"

//number of channels i.e. R G B
#define CHANNELS 3
#define BLUR_SIZE 4

unsigned char* loadPPM(const char* path, int* width, int* height);
void writePPM(const char* path, unsigned char* img, int width, int height);

__global__ 	void kernel_blur(unsigned char* d_rgb_image,unsigned char*d_blur_image,
                        int rows,int cols,int bsize) 
{
	int c=threadIdx.x+blockIdx.x*blockDim.x;
	int r=threadIdx.y+blockIdx.y*blockDim.y;
	
    unsigned int red  =0;
    unsigned int green=0;
    unsigned int blue =0;
    int num=0;
	for (int i=-bsize;i<bsize+1;i++)
		for (int j=-bsize;j<bsize+1;j++) {
			if((r+j<0)||(r+j>rows-1)||(c+i<0)||(c+i>cols-1)) continue; 
			red   +=d_rgb_image[3*((r+i)*cols+c+j)];
			green +=d_rgb_image[3*((r+i)*cols+c+j)+1];
			blue  +=d_rgb_image[3*((r+i)*cols+c+j)+2];
			num++;
		}
	
	d_blur_image[3*(c+r*cols)]=(unsigned char)   (red/num);
    d_blur_image[3*(c+r*cols)+1]=(unsigned char) (green/num);
    d_blur_image[3*(c+r*cols)+2]=(unsigned char) (blue/num);



}

int main(int argc, char **argv) 
{
	char* input_file;
	char* output_file;
    int bsize=4;
	//Check for the input file and output file names
	switch(argc) {
		case 4:
			input_file = argv[1];
			output_file = argv[2];
			bsize = atoi(argv[3]);
            break;
		case 3:
			input_file = argv[1];
			output_file = argv[2];
			bsize = 4;
            break;
		default:
			std::cerr << "Usage: <executable> input_file output_file bsize";
			exit(1);
	}
	
	unsigned char *h_rgb_image; //store image's rbg data
	unsigned char *d_rgb_image; //array for storing rgb data on device
	unsigned char *h_blur_image, *d_blur_image; //host and device's blur image data array pointers
	int rows; //number of rows of pixels
	int cols; //number of columns of pixels
	
	//load image into an array and retrieve number of pixels
	h_rgb_image = loadPPM(input_file, &rows, &cols); 

	int total_pixels=rows*cols;
	//allocate memory of host's blur image data array
	h_blur_image = (unsigned char *)malloc(sizeof(unsigned char*) * total_pixels * CHANNELS);

    

	/* ADD YOUR CODE HERE */
	//memcpy(h_blur_image,h_rgb_image,total_pixels*CHANNELS);
	cudaMalloc(&d_rgb_image,total_pixels*CHANNELS);
	cudaMalloc(&d_blur_image,total_pixels*CHANNELS);
	
	cudaMemcpy(d_rgb_image,h_rgb_image,total_pixels*CHANNELS,cudaMemcpyHostToDevice);
    dim3 BlockSize(16,16,1);
	dim3 GridSize((cols+15)/16,(15+rows)/16,1);
    	
	kernel_blur<<<GridSize,BlockSize>>>(d_rgb_image,d_blur_image,rows,cols,bsize);
    cudaMemcpy(h_blur_image,d_blur_image,total_pixels*CHANNELS,cudaMemcpyDeviceToHost);
	
	/* END OF YOUR CODE */

	//output the blurred image
    writePPM(output_file, h_blur_image, rows, cols);
	cudaFree(d_rgb_image);
	cudaFree(d_blur_image);
	return 0;
}

//function for loading an image into rgb format unsigned char array

unsigned char* loadPPM(const char* path, int* width, int* height) {
	FILE* file = fopen(path, "rb");

	if (!file) {
		fprintf(stderr, "Failed to open file\n");
		return NULL;
	}

	char header[3];
	fscanf(file, "%2s", header);
	if (header[0] != 'P' || header[1] != '6') {
		fprintf(stderr, "Invalid PPM file\n");
		return NULL;
	}

	fscanf(file, "%d %d", width, height);

	int maxColor;
	fscanf(file, "%d", &maxColor);

	fgetc(file);  // Skip single whitespace character

	unsigned char* img = (unsigned char*) malloc((*width) * (*height) * CHANNELS);
	if (!img) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	fread(img, CHANNELS, *width * *height, file);

	fclose(file);

	return img;
}

//function for writing blurred image data array to the image file
void writePPM(const char* path, unsigned char* img, int width, int height) {
    FILE* file = fopen(path, "wb");

    if (!file) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    fprintf(file, "P6\n%d %d\n255\n", width, height);

    fwrite(img, CHANNELS, width * height, file);

    fclose(file);
}
