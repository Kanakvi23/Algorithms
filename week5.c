/* Problem statement

You are given a color picture (in PPM format) consisting of an m×n
array A of pixels, where each pixel specifies a triple of red, green, and blue (RGB) intensities. Suppose that we wish to compress this picture slightly by removing one pixel from each of the m rows, so that the whole picture become one pixel narrower. To avoid disturbing visual effects, however, we require that the pixels removed in two adjacent rows be in the same or adjacent columns; the pixels removed from a seam from the top row to the bottom row where successive pixels in the seam are adjacent vertically or diagonally.

Suppose now that along with each pixel A[i, j], we have calculated a real valued disruption measure d[i, j], indicating how disruptive it would be to remove pixel A[i, j]. Intuitively, the lower the disruption measure of a pixel, the more similar the pixel is to its neighbors. Suppose further that we define the disruption measure of a seam to be the sum of the disruption measures of its pixels.

Give a dynamic programming algorithm to find a seam with the lowest disruption measure. */


#include <stdlib.h>
#include <stdio.h>
#include<math.h> 
  
struct pixel 
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

void calcd(int **img_gray,int **d, int rows, int cols) //Function that calculates disruption value of (i,j)th pixel
{
    int k,l,i,j,sum;
    
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {    
        	sum=0;
             for(k=-1;k<=1;k++)
                {   
                    for(l=-1;l<=1;l++)
                    {
                        if( i-k<0 || j-l<0 || i-k>=rows || j-l>=cols )
                            continue;
                        sum += abs(img_gray[i][j]-img_gray[i-k][j-l]);
                    }
                }
             d[i][j]=sum;
         }
     }
}

int main(int argc, char **argv)
{
    int i, j, rows, cols, maxi,minimum;
    FILE *fp = fopen(argv[1], "rb"); //read the given ppm file in binary mode
    FILE *fpout = fopen("reduced.ppm", "wb"); /* write in binary mode */
    char type[3];
    
    fscanf(fp, "%3s", type);
    if(strcmp(type, "P6") != 0) {  //Make sure we have a P6 file
        printf("The file type doesn't match P6");
        return 0;
    }
    
    /*char c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }*/
    
    /*Read number of columns rows and the max value of intensity that can represent a colour*/
    fscanf(fp, "%d", &cols);
    fscanf(fp, "%d", &rows);
    fscanf(fp, "%d", &maxi);
    //printf("%d %d %d\n", cols, rows, maxi);
    
    /*Dynamically create a 2D pixel structure array that will store the RGB intensity values for each pizel*/
    struct pixel **img;
    int **img_gray,*s,**d,**minv,**path;
    
    img = (struct pixel **)malloc(rows * sizeof(struct pixel *));
    for (i=0; i<rows; i++)
         img[i] = (struct pixel *)malloc(cols * sizeof(struct pixel));
         
    img_gray = (int **)malloc(rows * sizeof(int *));
    for (i=0; i<rows; i++)
         img_gray[i] = (int *)malloc(cols * sizeof(int));
    
    s = (int*)malloc(rows*sizeof(int)); //To store the column index forming the seam of each row
    
    d = (int **)malloc(rows * sizeof(int *)); //To store disruption value of each pixel
    for (i=0; i<rows; i++)
         d[i] = (int *)malloc(cols * sizeof(int));

    minv = (int **)malloc(rows * sizeof(int *)); //To store min disruption value from lower row
    for (i=0; i<rows; i++)
         minv[i] = (int *)malloc(cols * sizeof(int));

    path = (int **)malloc(rows * sizeof(int *)); //To store path, i.e., column index
    for (i=0; i<rows; i++)
         path[i] = (int *)malloc(cols * sizeof(int)); 

    unsigned char temp[3];
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            fread(temp, sizeof(char), 3, fp);
            img[i][j].red=temp[0];
            img[i][j].green=temp[1];
            img[i][j].blue=temp[2];
            img_gray[i][j]=((temp[0]+temp[1]+temp[2])/3);
            //printf("%d %d %d   ", img[i][j].red, img[i][j].green, img[i][j].blue);
        }
        //printf("\n");
    }
    
    calcd(img_gray,d,rows,cols); 	//function to calculate disruption value of each pixel
    
    /*for(i=0;i<rows;i++)
        {
            for(j=0;j<cols;j++)
            {
                printf("%d ",d[i][j]);
            }
            printf("\n");
        }*/

    for(j=0;j<cols;j++)				//Using the bottom up approach to find least disruptive path
    {	
    	minv[rows-1][j]=d[rows-1][j];
    	path[rows-1][j]=j;
    }

    for(i=rows-2;i<=0;i--)
    {
    	for(j=0;j<cols;j++)
    	{
    		if(j==0)
    		{	
    			if(d[i+1][j]>d[i+1][j+1])
    				minimum = j+1;
    			else
    				minimum = j; 
    		}
    		if(j==cols-1)
    		{
    			if(d[i+1][j]>d[i+1][j-1])
    				minimum = j-1;
    			else
    				minimum = j;
    		}
    		else
    		{
    				if(d[i+1][j]<=d[i+1][j-1] && d[i+1][j]<=d[i+1][j+1])
						minimum = j;
					else if(d[i+1][j-1]<=d[i+1][j] && d[i+1][j-1]<=d[i+1][j+1])
						minimum = (j-1);
					else
						minimum = (j+1);
    		}
    		minv[i][j]= d[i+1][minimum]+ d[i][j];      //min[i][j] contains the minimum value found among d[i+1][j-1],d[i+1][j] and d[i+1][j+1]
    		path[i][j]=minimum;						   //path[i][j] contains the column index of the cell which gave that minimum value
    	}	
    }

	int minsum = minv[0][0],start=0;					//Finding the start point
	for(j=1;j<cols;j++)
	{
		if(minv[0][j]<minsum)
		{
			minsum=minv[0][1];
			start=j;
		}
	}

	int check = start;

    fprintf(fpout, "P6\n%d %d\n255\n", rows-1, cols-1); //writing header in the output file
    for (i=0;i<rows;i++)								//writing intensity values
    {
    	 for (j=0;j<cols;j++)
    	 {	
    	 	if(j!=check)
    	 	{
	      		temp[0]=img[i][j].red;  
	      		temp[1]=img[i][j].green;  
	      		temp[2]=img[i][j].blue;  
	      		fwrite(temp, 1, 3, fpout);
      		}
   	     }
   	     check = d[i][check];
	}
    fclose(fpout);
    fclose(fp);
    return 0;    	
}
