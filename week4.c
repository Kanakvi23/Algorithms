#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<tgmath.h>
#define pi 3.14159265

void fft(int n, double complex A[], double complex F[])
{
	int j,k, sft=n/2;
	double complex w, w1,wk;

	if(n==1)
	{
		F[0]=A[0];
		return;
	}

	double complex *SM1,*SM2, *M1,*M2;
	M1=(double complex *)malloc(sizeof(double complex)*(n/2));
    M2=(double complex *)malloc(sizeof(double complex)*(n/2));
    SM1=(double complex *)malloc(sizeof(double complex)*(n/2));
    SM2=(double complex *)malloc(sizeof(double complex)*(n/2));

    for(j=k=0;k<n;j++,k+=2)
    {
    	M1[j]=A[k];
    	M2[j]=A[k+1];
    }

    fft(n/2,M1,SM1);
    fft(n/2,M2,SM2);

    wk= 1+0*I;
    w = cos(2*pi/n)+sin(2*pi/n)*I;

	for(k=0;k<n/2;k++,wk*=w)
    {
    	//wk=cpow(w1,k);
    	F[k] = SM1[k]+(wk*SM2[k]);
    	F[k+sft] = SM1[k]-(wk*SM2[k]);
    }
}

void ifft(int n, double complex A[], double complex F[])
{
	int j,k, sft=n/2;
	double complex w, w1,wk;

	if(n==1)
	{
		F[0]=A[0];
		return;
	}

	double complex *SM1,*SM2, *M1,*M2;
	M1=(double complex *)malloc(sizeof(double complex)*(n/2));
    M2=(double complex *)malloc(sizeof(double complex)*(n/2));
    SM1=(double complex *)malloc(sizeof(double complex)*(n/2));
    SM2=(double complex *)malloc(sizeof(double complex)*(n/2));

    for(j=k=0;k<n;j++,k+=2)
    {
    	M1[j]=A[k];
    	M2[j]=A[k+1];
    }

    ifft(n/2,M1,SM1);
    ifft(n/2,M2,SM2);

    wk= 1+0*I;
    w = cos(2*pi/n)+sin(2*pi/n)*I;

	for(k=0;k<n/2;k++,wk/=w)
    {
    	//wk=cpow(w1,k);
    	F[k] = SM1[k]+(wk*SM2[k]);
    	F[k+sft] = SM1[k]-(wk*SM2[k]);
    }
}

/*void horner(int n, double complex A[])
{
	int i,k;
	double complex *R,sum;
	double complex w,wk;
	w = cos(2*pi/n)+sin(2*pi/n)*I;
	R=(double complex *)malloc(sizeof(double complex)*n);
	for(k=0;k<n;k++)
	{	
		wk=cpow(w,k);
		sum=A[n-1];
		i=n-2;
		while(i>=0)
		{
			sum = sum*wk+A[i];
			i--;
		}
		R[k] = sum;
		printf("F[%d]= %f + i%f\n",k,creal(R[k]), cimag(R[k]));
	}
}*/


int main()
{
	int m,n,k;
	double re,im;
	double complex *FA,*FB,*FC,*A,*B,*C;
	printf("Enter the degree of polynomials A and B (2^d-1): ");
	scanf("%d",&m);
	m=m+1;
	n=2*m;
	A=(double complex *)malloc(sizeof(double complex)*n);
    B=(double complex *)malloc(sizeof(double complex)*n);
    C=(double complex *)malloc(sizeof(double complex)*n);
    FA=(double complex *)malloc(sizeof(double complex)*n);
    FB=(double complex *)malloc(sizeof(double complex)*n);
    FC=(double complex *)malloc(sizeof(double complex)*n);
	printf("Enter the coeffcients of the polynomial A:\n");
	for(k=0;k<m;k++)
	{
		float re;
		printf("A[%d] = ",k);
		scanf("%f",&re);
		A[k]=re+0*I;
	}
	for(;k<n;k++)
	    A[k]=0+0*I;
	printf("Enter the coeffcients of the polynomial B:\n");   
	for(k=0;k<m;k++)
	{
		float re;
		printf("B[%d] = ",k);
		scanf("%f",&re);
		B[k]=re+0*I;
	}
	for(;k<n;k++)
	    B[k]=0+0*I;

	fft(n,A,FA);
	fft(n,B,FB);
	printf("By FFT method, FA matrix-\n");
	for(k=0;k<n;k++)
	{
		  printf("FA[%d] = %f + i%f\n", k,creal(FA[k]), cimag(FA[k]));
    }
    /*printf("By Horner Method, FA matrix-\n");
    horner(n,A);*/
    
    printf("By FFT method, FB matrix-\n");
    for(k=0;k<n;k++)
	{
		  printf("FB[%d] = %f + i%f\n", k,creal(FB[k]), cimag(FB[k]));
    }
    /*printf("By Horner Method, FB matrix-\n");
    horner(n,B);*/
    
    for(k=0;k<n;k++)
        FC[k]=FA[k]*FB[k];
        
    ifft(n,FC,C);
    printf("Result of matrix multiplication-\n");
    for(k=0;k<n;k++)
	{
		  printf("Coeffecient of x^%d = %f\n", k,(creal(C[k])/n));
    }
    
   
}
