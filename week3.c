/* Aim: To calculate the minimum distance between two points from a given set of n points using an efficient algorithm.
   The approach in this program has a time complexity of O(nlogn).
   The array containing the coordinates are sorted on the basis of x coordinate and y coordinate using a O(nlogn) sorting algorithm.
   The x-sorted array is divided into two sub arrays containing n/2 terms each and the function to find the minimum distance is called recursively. 
   After division into sub arrays, the centre strip is found in O(n) time. Divison of y-sorted array about the middle line also takes O(n) time 
   by processing every point and comparing its x coordinate with x coordinate of middle line. 
   Therefore,
   T(n) = 2T(n/2) + O(n) + O(n) + O(n)
   T(n) = 2T(n/2) + O(n)
   T(n) = T(nLogn)  */


#include<stdio.h>
#include<math.h>
#include<stdlib.h>

struct point
{
	float x;
	float y;
};

void swap(struct point* first, struct point* second)
{
    struct point temp;
    temp=*first;
    *first = *second;
    *second = temp;
}

int part(struct point p[],int low, int high, char s)
{
    struct point pivot;
    pivot = p[high];
    int j,i = (low - 1);
 
    for (j = low; j <= high- 1; j++)
    {
      if(s=='x')
	{ 
	  if(p[j].x <= pivot.x)
	    {
	      i++;  
	      swap(&p[i], &p[j]);
	    }
	}
      else
	{
	  if(p[j].y <= pivot.y)
	    {
	      i++;  
	      swap(&p[i], &p[j]);
	    }
	}
    }
    swap(&p[i + 1], &p[high]);
    return (i + 1);
}

float min(float first,float second)
{
  if(first>second)
    return second;
  else
    return first;
}

void quicksort(struct point p[],int low,int high,char s)
{
  if(low<high)
    {
      int mid = part(p,low,high,s);
      quicksort(p,low,mid-1,s);
      quicksort(p,mid+1,high,s);
    }
 }

float bf(struct point p[],int n)
{
  float min = INFINITY;
  int i,j;
    for (i = 0; i < n; i++)
        for (j = i+1; j < n; j++)
	  {
	    float calcdist = sqrt(pow(p[i].x-p[j].x,2)+pow(p[i].y-p[j].y,2));
            if (calcdist < min)
                min = calcdist;
	  }
    return min;
}


float centclosest(struct point cent[], int size, float dist)
{
    float min = dist;
    int i,j;

    for (i = 0; i < size; i++)
        for (j = i+1; j < size && (cent[j].y - cent[i].y) < min; j++)
	  {
	    float calcdist = sqrt(pow(cent[i].x-cent[j].x,2)+pow(cent[i].y-cent[j].y,2));
            if (calcdist< min)
                min = calcdist;
	  }
 
    return min;
}

float rec(struct point px[],struct point py[],int n)
{
  if(n<=3)
    return(bf(px,n));

  int mid=n/2;
  float dist;
  struct point midp,*pyl,*pyr,*cent;
  midp=px[mid];
  pyl=(struct point *)malloc(sizeof(struct point)*(mid+1));
  pyr=(struct point *)malloc(sizeof(struct point)*(n-mid-1));
  cent=(struct point *)malloc(sizeof(struct point)*n);
  int i,j=0,l=0,r=0;

  for(i=0;i<n;i++)
    {
      if (py[i].x <= midp.x)
         pyl[l++] = py[i];
      else
         pyr[r++] = py[i];
    }
  float distl = rec(px, pyl, mid);
  float distr = rec(px + mid, pyr, n-mid);
  
  dist=min(distr,distl);
  
  for(i=0;i<n;i++)
    {
      if(abs(py[i].x - midp.x) < dist)
	{
	  cent[j] = py[i]; 
	  j++;
	}
    }
  
  float centmin = centclosest(cent,j,dist);
  return(min(dist,centmin));
}


float sorting(struct point p[],int n)
{
        int i;
        struct point *px, *py;
	px=(struct point *)malloc(sizeof(struct point)*n);
	py=(struct point *)malloc(sizeof(struct point)*n);
	for(i=0;i<n;i++)
	  {
	    px[i]=p[i];
	    py[i]=p[i];
	  }
	quicksort(px,0,n-1,'x');
	quicksort(py,0,n-1,'y');
	
	printf("Sorted by x:\n");
	for(i=0;i<n;i++)
	  {
	    printf("(%f,%f)\n",px[i].x,px[i].y);
	  }

	printf("Sorted by y:\n");
	for(i=0;i<n;i++)
	  {
	    printf("(%f,%f)\n",py[i].x,py[i].y);
	  }
	return (rec(px,py,n));
}


void main()
{
        int n,i;
        struct point *p;
        printf("Enter the number of 2D points\n");
	scanf("%d",&n);
	p=(struct point *)malloc(sizeof(struct point)*n);
	printf("Enter the points as x (space) y:\n");
	for(i=0;i<n;i++)
	  {
	    printf("Enter point %d: \n",i+1);
	    scanf("%f %f",&p[i].x,&p[i].y);
	  }
	printf("The points entered are:\n");
	for(i=0;i<n;i++)
	  printf("(%f,%f)\n",p[i].x,p[i].y);	
	
	float res = sorting(p,n);
	printf("Closest distance = %f\n",res);
}
