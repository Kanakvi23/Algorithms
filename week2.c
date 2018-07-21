#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<tgmath.h>
#define pi 3.14159265

void fft(int n, double complex A[], double complex F[])
{
	int j,k, sft=n/3;
	double complex w, w1,wk;

	if(n==1)
	{
		F[0]=A[0];
		return;
	}

	double complex *M1,*M2,*M3,*SM1,*SM2,*SM3;
	M1=(double complex *)malloc(sizeof(double complex)*(n/3));
    M2=(double complex *)malloc(sizeof(double complex)*(n/3));
    M3=(double complex *)malloc(sizeof(double complex)*(n/3));
    SM1=(double complex *)malloc(sizeof(double complex)*(n/3));
    SM2=(double complex *)malloc(sizeof(double complex)*(n/3));
    SM3=(double complex *)malloc(sizeof(double complex)*(n/3));

    for(j=k=0;k<n;j++,k+=3)
    {
    	M1[j]=A[k];
    	M2[j]=A[k+1];
    	M3[j]=A[k+2];
    }

    fft(n/3,M1,SM1);
    fft(n/3,M2,SM2);
    fft(n/3,M3,SM3);

    w= cos(2*pi/3)+sin(2*pi/3)*I;
    w1 = cos(2*pi/n)+sin(2*pi/n)*I;

	for(k=0;k<n/3;k++)
    {
    	wk=cpow(w1,k);
    	F[k] = SM1[k]+(wk*SM2[k])+(wk*wk*SM3[k]);
    	F[k+sft] = SM1[k]+(wk*w*SM2[k])+(wk*wk*w*w*SM3[k]);
    	F[k+2*sft] = SM1[k]+(wk*w*w*SM2[k])+(wk*wk*w*SM3[k]);
    }
}

void horner(int n, double complex A[])
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
}


int main()
{
	int n,k;
	double re,im;
	double complex *F,*A;
	printf("Enter the degree of polynomial (2^d-1): ");
	scanf("%d",&n);
	n=n+1;
	A=(double complex *)malloc(sizeof(double complex)*n);
    F=(double complex *)malloc(sizeof(double complex)*n);
	printf("Cofficients of the polynomial:\n");
	for(k=0;k<n;k++)
	{
		re=rand()%21-10;
		im=rand()%21-10;
		A[k]=re+im*I;
		printf("A[%d] = %f + i%f \n",k,creal(A[k]),cimag(A[k]));
	}

	fft(n,A,F);
	printf("By FFT method, F matrix-\n");
	for(k=0;k<n;k++)
	{
		  printf("F[%d] = %f + i%f\n", k,creal(F[k]), cimag(F[k]));
    }

    printf("By Horner Method, F matrix-\n");
    horner(n,A);
}
