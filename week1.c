#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void multiply(int a[2][2], int b[2][2])
{
	int x = a[0][0]*b[0][0]+a[0][1]*b[1][0];
	int y = a[0][0]*b[0][1]+a[0][1]*b[1][1];
	int z = a[1][0]*b[0][0]+a[1][1]*b[1][0];
	int w = a[1][0]*b[0][1]+a[1][1]*b[1][1];
	a[0][0]=x;
	a[0][1]=y;
	a[1][0]=z;
	a[1][1]=w;
}

int mat(int n)
{
	printf("Using Matrix Power method- \n");
	int p[2][2]={{1,0},{0,1}};
	int t[2][2]={{1,1},{1,0}}; 

	while(n!=0)
	{
		int rem=n%2;
		if(rem==1)
			multiply(p,t);
		multiply(t,t);
		printf("Matrix P:- \n %d %d \n %d %d \n",p[0][0],p[0][1],p[1][0],p[1][1]);
		printf("Matrix T:- \n %d %d \n %d %d \n",t[0][0],t[0][1],t[1][0],t[1][1]);
		n=n/2;
	}
	
	printf("The required Fibonacci number is %d \n",p[0][0]);
}

void rev(int n)
{
	printf("Using Reversed Odd Even Reduction- \n");
	int num = n, k=0, cnt;
	int *fib = (int *)malloc(sizeof(int)*n);
	int bin[10];
	fib[0]=0,fib[1]=1,fib[2]=1;

	while(n!=0)
	{
		int rem = n%2;
		bin[k] = rem;
		n=n/2;
		k++;
	}

	for(k=k-2,cnt=1;k>=0;k--)
	{	
		fib[2*cnt] = fib[cnt]*(2*fib[cnt+1]-fib[cnt]);
		fib[2*cnt+1] = pow(fib[cnt+1],2) + pow(fib[cnt],2);
		printf("%d:- %d \n",2*cnt,fib[2*cnt]);
		printf("%d:- %d \n",2*cnt+1,fib[2*cnt+1]);
		if(bin[k]==1)
		{ 
			cnt=2*cnt+1;
			fib[cnt+1] = fib[cnt] + fib[cnt-1];
			printf("%d: %d\n",cnt+1, fib[cnt+1]);
		}
		else
		{
			cnt=2*cnt;
		}
	}

	printf("The required Fibonacci number is %d \n",fib[num]);
}

int main(int argc,char **argv)
{
	rev(atoi(argv[1]));
	mat(atoi(argv[1])-1);
}
