/*Problem statement

You are given a sequence of Boolean constants T (for true) and F (for false).

You are required to determine the number of ways parenthesised Boolean expressions can be formed from the given sequence of Boolean constants using the xor connective so that the result evaluates to T. 

Give a dynamic programming algorithm to find the required count. */ 


/*In this program, we have used bottom up approach to solve the PS. 
  The base cases used are:
  1) When the string is of length 1: T can be parenthesised in 1 way to make it T and 0 ways to make it F. Vice-versa for F.
  2) When the string is of length 2: TT and FF can be parenthesised in 1 way to make it F and 0 ways to make it T, whereas TF and FT can be parenthesised in 1 way to make
     it T and 0 ways to make it F.
  We have  maintained two tables - first (called DP table) to store the number of ways the given string can be parenthesised to give T and the other for F.
  After computing the base values, on diagonal traversal through both the tables, we used subproblems to fill the tables. */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
  char string[100];
  int n,i,j,k,diff,numt,numf;
  printf("Enter the boolean string without any spaces\n");
  scanf("%s",string);
  n = strlen(string);
  int s = n-1;
  printf("%d\n",n);
  for(i=0;i<n;i++)
  {
    printf("%c",string[i]);
    printf("\n");
  }
  
  int **table,**tablef;                                       /*table[i][j] stores the number of ways the string starting from 
                                                                i and ending at j can be paranthesized to evaluate to True
                                                                whereas, tablef[i][j] stores the same for False cases*/ 
  table = (int **)malloc(n * sizeof(int *));
  for(i=0; i<n; i++)
    table[i] = (int *)malloc(n*sizeof(int));
  
  tablef = (int **)malloc(n * sizeof(int *));
  for(i=0; i<n; i++)
    tablef[i] = (int *)malloc(n*sizeof(int));
  
  for(i=0;i<n;i++)
    {
      if(string[i]=='T')
        {
          table[i][i]=1;
          tablef[i][i]=0;
        }
      else
        {
          table[i][i]=0;
          tablef[i][i]=1;
        }
    }
  
  for(i=0;i<n-1;i++)
  {
    j=i+1;
    if(string[i]==string[j])
      {
        table[i][j]=0;
        tablef[i][j]=1;
      }
    else
      {
        table[i][j]=1;
        tablef[i][j]=0;
       }
  }
  
  for(diff=2;diff<n;diff++)
    { 
      s--;
      for(i=0;i<s;i++)
      { 
        j=i+diff;
        numt=0,numf=0;
        for(k=i;k<j;k++)                                                                        /**/
          {                                                                                     /*With XOR connectivity FF and TT gives F, whereas FT and TF gives T*/
              numt += (table[i][k]*tablef[k+1][j]) + (tablef[i][k]*table[k+1][j]);
              numf += (table[i][k]*table[k+1][j]) + (tablef[i][k]*tablef[k+1][j]);
          }
        table[i][j] = numt;
        tablef[i][j] = numf;
      }
    }
  
  printf("DP table - \n");  
  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
        printf("%d  ",table[i][j]);
      printf("\n");
    }
   printf("Total number of ways - %d\n",table[0][n-1]);
}
