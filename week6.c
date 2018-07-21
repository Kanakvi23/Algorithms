#include<stdio.h>
#include<stdlib.h>

int preIndex = 0;	//Variable to traverse through the preorder array 

struct node 	//Defining structure that represents each node of the tree 
{
	int num;
	struct node* left;
	struct node* right;
};

struct node* create(int num) 	//Function to create a new node and initialize it
{
  struct node* node = (struct node*)malloc(sizeof(struct node));
  node->num = num;
  node->left = NULL;
  node->right = NULL;
  return(node);
}

int findInIndex(int arr[], int num, int l, int r)	//Function to find the position of the element taken from preorder array in inorder array
{												    //This indentifies the left and right subtrees of that element 
	int i;
	for(i=l;i<=r;i++)
	{
		if(arr[i] == num)
			return i;
	}
}

void enumSumPaths(struct node* root, int arr[],int sum)		//Function to find all possible paths which result in the entered sum 
{															/*Using recursion, we traverse up from each node until we find the entered sum 
															  or the sum exceeds the entered sum*/  
	int static k = 0;												
	if(root == NULL)
		return;
	else
	{
		arr[k] = root->num;
		k++;
		enumSumPaths(root->left,arr,sum);
		enumSumPaths(root->right,arr,sum);
		
		int temp=0;
		int start;
		printf("Checking if any path is possible which terminates at %d..\n",arr[k-1]);
		for(start=k-1;start>=0;start--)
		{
			temp += arr[start];
			if(temp>sum)
				break;
			if(temp == sum)
				{	printf("Path found!\n");
					printf("Path is: ");
					for(;start<=k-1;start++)
						printf("%d ",arr[start]);
					printf("\n");
					break;
				}
		}
		k--;
	}		
}


struct node* reconBinTree(int in[],int pre[], int l, int r)		//Function that constructs the binary tree using preorder and inorder arrays
{
	int i;
	
	if(l>r)
		return NULL;
	
	int curr = pre[preIndex];
	preIndex++;
	struct node* currNode = create(curr);
	
	if(l==r)
		return currNode;
	else
		{
			int inIndex = findInIndex(in,curr,l,r);
			printf("The current node is: %d\n",curr);
			printf("Left subtree nodes are: ");
			for(i=l;i<inIndex;i++)
				printf("%d ",in[i]);
			printf("\nRight subtree nodes are: ");
			for(i=inIndex+1;i<=r;i++)
				printf("%d ",in[i]);
			printf("\n");
			currNode->left = reconBinTree(in,pre,l,inIndex-1);
			currNode->right = reconBinTree(in,pre,inIndex+1,r);
		}
	return currNode;
}

void printInorder(struct node* node)		//Function to print the inorder array of the constructed binary tree 
{
	 if (node == NULL)
	     return;
	 printInorder(node->left);
	 printf("%d ", node->num);
	 printInorder(node->right);
}

int main(int argc, char **argv)
{
	//int pre[10] = {8,5,9,7,1,12,2,4,11,3};
	//int in[10] = {9,5,1,7,2,12,8,4,3,11};
	//int len = 10;
	//int arr[100];
	
	int pre[100], in[100], arr[100];
	int len = 0, i = 0;
	char temp;
	printf("Enter inorder traversal array with space between integers:\n");
	do
	{
		scanf("%d%c",&in[len],&temp);
		len++;
	}while(temp!='\n');
	printf("Enter preorder trasversal array with space between integers:\n");
	do
	{
		scanf("%d%c",&pre[i],&temp);
		i++;
	}while(temp!='\n');

	printf("Entered inorder traversal array: \n");
	for(i=0;i<len;i++)
	{
		printf("%d ",in[i]);
	}
	printf("\nEntered preorder traversal array: \n");
	for(i=0;i<len;i++)
	{
		printf("%d ",pre[i]);
	}

	printf("\n\nIntermediate steps:\n");
	struct node* root = reconBinTree(in, pre, 0, len-1);
	
	printf("Inorder traversal of the constructed tree is: \n");
  	printInorder(root);
  	int v;
  	v=atoi(argv[1]);
  	printf("All possible paths for sum = %d are:\n",v);
  	enumSumPaths(root,arr,v);
}
