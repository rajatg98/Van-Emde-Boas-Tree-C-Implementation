#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define high(x,u) ((x)/(u))
#define low(x,u)  ((x)%((u)))
typedef struct node{
    int min,max;
	int u;
	int *bit;
	struct node **cluster;
}node;

int findmin(node* ptr)
{
    int i;
    for(i=ptr->min+1;i<ptr->u;i++)
	if(ptr->bit[i]==1)
	    return i;
    return -1;
}
int findmax(node* ptr)
{
    int i;
    for(i=ptr->max-1;i>=0;i--)
	if(ptr->bit[i]==1)
	    return i;
    return -1;
}
void freee(node* ptr)
{
    free(ptr->bit);
    free(ptr->cluster);
    free(ptr);
}

int stack[1000000],si=-1;
node * delete(int x,node*ptr,long long int u)
{
    if(ptr==NULL)
		return NULL;

    if(u==2)
    {
		ptr->bit[x]=0;
		//printf("entered\n");
		//printf("ptr->min=%d ptr->max=%d\n",ptr->min,ptr->max);
		if(x==ptr->min && ptr->max!=x)
	    	ptr->min=ptr->max;
		else if(x==ptr->max && ptr->min!=x)
			ptr->max=ptr->min;
		else if(x==ptr->max && x==ptr->min)
		{
	    	ptr->min=-1;
	    	ptr->max=-1;
	    	//printf("entered\n");
	    	freee(ptr);
	    	return NULL;
		}
		return ptr;
    }
    else
    {
		int cluster_no=high(x,((int)(ptr->u)));
		int position=low(x,((int)(ptr->u)));
		//printf("cluster=%d position=%d\n",cluster_no,position);
		ptr->cluster[cluster_no]=delete(position,ptr->cluster[cluster_no],((int)sqrt(u)));
		if(ptr->cluster[cluster_no]==NULL)
		{
			//printf("teri\n");
	    	ptr->bit[cluster_no]=0;
	    	//printf("min=%d max=%d clusterno=%d u=%lld\n",ptr->min,ptr->max,cluster_no,u);
	    	if(cluster_no==ptr->min && cluster_no!=ptr->max)
	    	{
		    	ptr->min=-1;
				// printf("teri1\n");
		    	ptr->min=findmin(ptr);
	    	}
	    	else if(cluster_no==ptr->max && cluster_no!=ptr->min)
	    	{
		    	ptr->max=-1;
				// printf("teri2\n");
		    	ptr->max=findmax(ptr);
	    	}
	    	else if(cluster_no==ptr->max && cluster_no==ptr->min)
	    	{
		    	ptr->max=-1;
		    	ptr->min=-1;	
				//printf("teri3\n");
		    	freee(ptr);
		    	return NULL;
	    	}
		}
	}
    return ptr;
}

struct node* insert(int k,node*ptr,long long int u)
{
    if(ptr==NULL &&  u!=2)
    {
		ptr=(node*)malloc(sizeof(node)*((int)sqrt(u)));
		ptr->bit=(int*)malloc(sizeof(int)*((int)sqrt(u)));
		int i;
		ptr->cluster=(node**)malloc(sizeof(node*)*((int)sqrt(u)));
		ptr->u=((int)sqrt(u));
		for(i=0;i<ptr->u;i++)
	    	ptr->cluster[i]=NULL;
		for(i=0;i<ptr->u;i++)
	    	ptr->bit[i]=0;
		ptr->min=ptr->max=-1;
    }
    else if(u==2&&ptr==NULL)
    {
		ptr=(node*)malloc(sizeof(node));
		ptr->bit=(int*)malloc(sizeof(int)*2);
		int i;
		ptr->cluster=NULL;
		ptr->u=2;
		for(i=0;i<ptr->u;i++)
	    	ptr->bit[i]=0;
		ptr->min=ptr->max=-1;
    }
    
    int cluster_no=high(k,((int)(ptr->u)));
    int position=low(k,((int)(ptr->u)));
    
    if(u==2)
    {
		ptr->bit[k]=1;
		if(ptr->min==ptr->max && ptr->min==-1)
	    	ptr->min=ptr->max=k;
		else if(k<ptr->min)
	    	ptr->min=k;
		else if(k>ptr->max)
	    	ptr->max=k;
		return ptr;
    }
    if(ptr->min==ptr->max && ptr->min==-1)
		ptr->min=ptr->max=cluster_no;
    else if(cluster_no<ptr->min)
		ptr->min=cluster_no;
    else if(cluster_no>ptr->max)
		ptr->max=cluster_no;
    ptr->bit[cluster_no]=1;
    ptr->cluster[cluster_no]=insert(position,ptr->cluster[cluster_no],sqrt(u));
	//printf("pt-mi=%d pt-ma=%d u=%lld\n",ptr->min,ptr->max,u);
    return ptr;
}


int findnext(node*ptr,int u)
{
    if(u==2)
		return ptr->min;

    int val=findnext(ptr->cluster[ptr->min],((int)sqrt(u)));
    return (ptr->min*ptr->u + val);
}

int member(node *ptr,int x,long long int u)
{
    if(ptr==NULL)
		return 0;
    if(u==2)
    {
		if(ptr->bit[x]==0)
	    	return 0;
		else
	    	return 1;
    }
    int cluster_no=high(x,((int)(ptr->u)));
    int position=low(x,((int)(ptr->u))),i;
    
    return member(ptr->cluster[cluster_no],position,(int)sqrt(u));
}

int succ(node*ptr,int x,int u)
{
    if(ptr==NULL)
		return -1;
    if(u==2)
    {
		//printf("ptr->max=%d ptr->min=%d x=%d\n",ptr->max,ptr->min,x);
		if(x<ptr->max)
	    	return ptr->max;
		return -1;
    }
    
    int cluster_no=high(x,((int)(ptr->u)));
    int position=low(x,((int)(ptr->u))),i;
    //printf("cluster_no=%d position=%d x=%d u=%d ptr->u=%d\n",cluster_no,position,x,u,ptr->u);
    //printf("ptr->max=%d ptr->min=%d x=%d\n",ptr->max,ptr->min,x);
    
    if(ptr->min==-1)
		return -1;
	if(cluster_no>ptr->max&&ptr->max!=-1)
		return -1;
    if(cluster_no<ptr->min)
    {
		int val=findnext(ptr->cluster[ptr->min],ptr->u);
		return ptr->min*ptr->u+val;
    }
    //printf("again\n");
    int r=succ(ptr->cluster[cluster_no],position,(int)sqrt(u));
    if(r==-1)
    {
		for(i=cluster_no+1;i<ptr->u;i++)
	    	if(ptr->bit[i]==1)
				break;
		if(i==ptr->u)
	    	return -1;
		else
		{
	    	int val=findnext(ptr->cluster[i],ptr->u);
	    	return i*ptr->u+val;
		}
    }
    else
		return cluster_no*ptr->u + r;
}

void print(node* ptr,long long int u,long long int no)
{
    if(ptr==NULL)
		return;
    if(ptr!=NULL && u==2)
    {
		//printf("ptr->bit[0] : %d\n",ptr->bit[0]);
		fflush(stdout);
		if(ptr->bit[0]==1)
		{
	    	stack[++si]=no;
			//printf("%lld ",no);
		}
		if(ptr->bit[1]==1)
		{	
	    	stack[++si]=no+1;
			//printf("%lld ",no+1);
		}
		return;
    }
    int i;
    for(i=0;i<ptr->u;i++)
    {
		long long int tem=ptr->u*i;
		long long int to=tem+no ;
		print(ptr->cluster[i],(int)sqrt(u),to);
	}
}

int main()
{
    int n,i,j,k,m,l,c,x;
    node *head=NULL;
    long long int temp=65536;
    long long int univ=temp*temp;
    printf("**WELCOME**\n\nEnter as:\n'I'->Insert\n'S'->Search\n'D'->Delete\n'P'->Print\n'Q'->Quit\n\n");
    while((c=getchar())!='Q')
	{
		if(c=='I')
		{
	    	scanf("%d",&x);
	    	head=insert(x,head,univ);
		}
		else if(c=='S')
		{
	    	scanf("%d",&x);
	    	if(member(head,x,univ)==1)
		    	printf("Found\n");
	    	else
		    	printf("Not Found\n");
		}
		else if(c=='P')
		{
	    	si=-1;
	    	print(head,univ,0);
			//if(member(head,37,univ))
			//printf("Found");
			//else printf("Not Found");
	    	for(i=0;i<=si;i++)
	    	{
		    	if(i!=si)
					printf("%d ",stack[i]);
		    	else
					printf("%d",stack[si]);
	    	}
            printf("\n");
		}
		else if(c=='D')
		{
	    	scanf("%d",&x);
	    	head=delete(x,head,univ);
			//printf("\n");
		}
		else if(c=='N')
		{
			printf("Enter number to find its successor\n");
			scanf("%d",&x);
			int next=succ(head,x,univ);
			printf("%d",next);
		}
    }
    return 0;
}
