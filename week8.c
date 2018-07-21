/* Problem statement

Programming part
    Implement a priority queue program that reads a scheduling file (schedule.txt) supplied as a command line argument and performs the following operations:

        Insert job request ⟨A 10⟩ into the priority queue when A requests for 10 resources as per the following entry in the schedule file:

            A    10

        Print the status of the jobs in the priority queue when you encounter following entry in the schedule file:

            PRINT

        Delete job from the priority queue when its execution is over (Resources / CPU time meets the requirement for the job). Following entry in the schedule file indicates 40 resources are available.

            CPU    40

	    User can submit multiple jobs. Priority queue stores the job information (user id and corresponding resource requirement) as per the user's priority. No initial users list is available. You have to discover users through their interaction/job submission. */

/*
	We use heap data structure to implement priority queue, because the hgihest priority task stays on the heap array at all times and thus is easily executed.
	
	Let n be the total number of tasks
	Time complexity analysis:
	Finding the task with highest priority: O(1) 
	Inserting a new task to the heap: O(log(n)). We add a new task at the end of the tree. If the new task's user has greater priority or 
	the resources used are less is greater than  its  parent,  then  we  need to  traverse  up  to  fix  the  violated  heap property.
	Extracting the task with highest priority after its completion: O(log(n)). We replace it by the last element in the tree. Now we need to
	traverse down the tree to fix  the  violated  heap property. 
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int numusers=0;
char allusers[100];		//Array to store every user ID
int corrp[100];			//Array to store corresponsing priority of each user
int corrtasks[100];		//Array to store the current number of tasks submitted by each user
int hsize;
 
struct node
{
	char user;
	int res;
	int userpos;
};

struct node* swap(int child, int parent, struct node* harr)
{
	char tempuser = harr[child].user;
	int tempres = harr[child].res;
	int tempuserpos = harr[child].userpos;
	
	harr[child].user = harr[parent].user;
	harr[child].res = harr[parent].res;
	harr[child].userpos = harr[parent].userpos;
	
	harr[parent].user = tempuser;
	harr[parent].res = tempres;
	harr[parent].userpos = tempuserpos;
	
	return harr;		
}


struct node* insert(struct node* harr,char user,int res, int userpos)
{
	if(hsize == 100)
	{
		printf("Heap full, overflow!\n");
		return;
	}
	hsize++;
	int i = hsize-1;
	harr[i].user = user;
	harr[i].res = res;
	harr[i].userpos = userpos;
	int flag = 0;
	
	while(i!=0 && flag==0)
	{
		int parent = (i-1)/2;
		int parentp = corrp[harr[parent].userpos];	//For storing priority of parent
		//int j;  
		flag = -1;  
		/*for(j=0;j<numusers;j++)
		{
			if(allusers[j]==harr[(i-1)/2].user)
			{
				parentp = corrp[j];
				break;
			}
		}*/
		
		if(parentp < corrp[userpos])   //Checking if the priority of newly added node user is higher than its parent
		{
			harr = swap(i,parent,harr);
			i = (i-1)/2;
			flag = 0;
		}
		else if(parentp == corrp[userpos])	
		{
			if(harr[parent].res > harr[i].res)	//priority of new node and child user is same, therefore comparing resources by their respective tasks
			{
				swap(i,parent,harr);
				i = (i-1)/2;
				flag = 0;
			}
		}
	}
	return harr;
}

struct node* heapify(struct node* harr,int i)
{
	int l = 2*i + 1;
	int r = 2*1 + 2;
	int smallest = i;
	if( l < hsize)
	{
		if(corrp[harr[i].userpos] < corrp[harr[l].userpos] || (corrp[harr[i].userpos] == corrp[harr[l].userpos] && harr[i].res > harr[l].res))
			smallest = l;		  
	}
	
	if(r < hsize)
	{
		if(corrp[harr[smallest].userpos] < corrp[harr[r].userpos] || (corrp[harr[smallest].userpos] == corrp[harr[r].userpos] && harr[smallest].res > harr[r].res))
			smallest = r;	
	}
	
	if(smallest!=i)
	{
		swap(i,smallest,harr);
		heapify(harr,smallest);
	}
	return harr;
}


struct node* deletetop(struct node* harr)
{
	if(hsize <= 0)
		return NULL;
		
	int k;
	for(k=0;k<numusers;k++)   //Increasing the priority of users not in priority queue
	{
		if(corrtasks[k]<=0)	//User is not in the priority queue if all tasks submitted by it are over
		{
			corrp[k] = corrp[k] + harr[0].res;
			if(corrp[k]>10000 && allusers[k]!='@')
				corrp[k]=10000;
		} 
	}
	
	if(hsize == 1)
	{
		hsize--;
		return NULL;
	}
	
	harr = swap(0,hsize-1,harr); //Making the top element as the last element 
	hsize--;
	corrtasks[harr[0].userpos]--;
	
	harr = heapify(harr,0);
	
	return harr;
}

int main()
{
	FILE *ptr;
	ptr = fopen("schedule.txt","r");
	if(!ptr)
	{
		printf("Error!\n");
		return 0;
	}
	
	struct node *harr;	//Heap array
	harr = (struct node*)malloc(100*sizeof(struct node));  //Assuming 100 to be the capacity of the heap
	hsize=0;
	
	
	
	int res,k;
	char str[10];
	int cpu=0;
	
	for(k=0;k<100;k++)
		corrtasks[k]=0;


	while(!feof(ptr))
	{
		fscanf(ptr,"%s",str);
		//printf("The string read is %s\n",str);
		if(strcmp(str,"PRINT")!=0)
		{
			fscanf(ptr,"%d",&res);
			if(strcmp(str,"CPU")!=0)
			{
				printf("User is %s and resources required is %d\n",str,res);
				if(strcmp(str,"ADMIN")!=0)
				{	
					int i, flag=-1;
					for(i=0;i<numusers;i++)
					{
						if(allusers[i]==str[0])
							{
								flag=0;
								break;
							}
					}
					if(flag==-1)   //User not found, therefore it is first task by user
					{
						numusers++;
						allusers[numusers-1]=str[0];
						corrp[numusers-1]=10000;		//intial priority given to new users
					}
					else		//User has given a task earlier, priotiy of user will now decrease
					{
						corrp[i]=corrp[i]-res;
					}
					corrtasks[i]++;
					//Now we will add the task to the heap
					harr = insert(harr,str[0],res,i);
					
					
				}
				else
				{
					//Task is given by ADMIN, will have highest prioity, directly added to top of priotiy queue
					int i, flag=-1;
					for(i=0;i<numusers;i++)
					{
						if(allusers[i]=='@')	//Using the symbol @ for admin to maintain character data type for user ID and avoid using string
							{
								flag=0;
								break;
							}
					}
					if(flag==-1)   //User not found, therefore it is first task by user
					{
						numusers++;
						allusers[numusers-1]='@';
						corrp[numusers-1]=9999999;	//Priority of ADMIN
					}
					
					//Now we will add the task to the heap
					harr = insert(harr,'@',res,i);
					
				}
			}
			else
			{
				printf("CPU has made available %d resources\n",res);	//When CPU has made available resources, we use them till it is exhausted
				cpu=cpu+res;
				int resreqd;
				while(cpu>0 && harr!=NULL)
					{
						resreqd = harr[0].res;
						if(cpu - resreqd > 0)
							{
								harr = deletetop(harr);   //If highest priority task is completed, it is deleted afterwards	
							}
						else
							break;
						cpu = cpu - resreqd;
					}
			}
		}
		else
		{	
			printf("Operation is to print\n");	//When file says PRINT, printing the heap array which keeps the check on the highest priority task  
			printf("Heap array:\n");
			int i;
			for(i=0;i<hsize;i++)
			{
				printf("User: %c  Resources: %d\n",harr[i].user,harr[i].res);
			}
		}
	}
}
