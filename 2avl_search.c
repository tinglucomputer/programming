#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sCount = 0;            	//成功匹配个数 
int nCount = 0;			   	//树节点个数 
long long cmpCount = 0;		//字符比较次数 
long long mCount = 0;		//内存占用量 
 
typedef struct TNode
{
	char* p;
	struct TNode *lchild,*rchild;
	int height;
}*AVL;



int cmp (const char * s1, const char * s2)
{ 
	for(; *s1 == *s2; ++s1, ++s2) 
	{ 
		cmpCount++;
		if(*s1 == 0) 
		{ 
			return 0; 
		}
	} 
	return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1; 
	//把中文字符变换成ANSI或者其他的码格式是 应该注意到这种形式的转换是无符号数的表示 因此应该使用无符号类型指针去描述 
} 


void* bupt_malloc(size_t size)
{
	mCount += size; 
    if(size <= 0)
    {
    	return NULL;
    }
    return malloc(size);
}

AVL newTNode(char *p)
{
	nCount++; 
	AVL node = bupt_malloc(sizeof(struct TNode));
	node->lchild = node->rchild = NULL;
	node->height = 1;                        //初始层数
	node->p = bupt_malloc(strlen(p)+1); 
	strcpy(node->p,p);
	node->p[strlen(p)] = '\0';              //strcpy时需要在最后加上\0结束符 
	return node;
}

int height(AVL a)
{
	if(a == NULL) return 0;        //a没有兄弟节点 
	return a->height;
}

int max(int a,int b)
{
	return a > b ? a : b;
}

AVL LL(AVL node)
{
	AVL nodeX = node->lchild;
	node->lchild = node->lchild->rchild;
	nodeX->rchild = node;
	node->height = 1 + max(height(node->lchild),height(node->rchild));
	nodeX->height = 1 + max(height(nodeX->lchild),height(nodeX->rchild));
	return nodeX;
}

AVL RR(AVL node)
{
	AVL nodeX = node->rchild;
	node->rchild = node->rchild->lchild;
	nodeX->lchild = node;
	node->height = 1 + max(height(node->lchild),height(node->rchild));
	nodeX->height = 1 + max(height(nodeX->lchild),height(nodeX->rchild));
	return nodeX;	
} 

AVL LR(AVL node)
{
	node->lchild = RR(node->lchild);
	return LL(node);
}

AVL RL(AVL node)
{
	node->rchild = LL(node->rchild);
	return RR(node);
}

int getBalance(AVL node)
{
	if(node == NULL)
	{
		return 0;
	}
	return height(node->lchild) - height(node->rchild);
}

AVL Insert(AVL T,char *p)
{
	if(T == NULL)                 //向下插入节点   
	{
		return newTNode(p);
	}
	
	if(strcmp(T->p,p) < 0)
	{
		T->rchild = Insert(T->rchild,p);
	}
	else if(strcmp(T->p,p) > 0)
	{
		T->lchild = Insert(T->lchild,p);
	}
	else
	{
		return T;
	}
																
	T->height = 1 + max(height(T->lchild),height(T->rchild));    //height向上回溯依次得到 
	int balance = getBalance(T);                              //计算平衡因子
	
	if(balance > 1 && strcmp(T->lchild->p,p) > 0)
	{
		return LL(T);
	} 
	
	if(balance > 1 && strcmp(T->lchild->p,p) < 0)
	{
		return LR(T);
	} 
	
	if(balance < -1 && strcmp(T->rchild->p,p) < 0)
	{
		return RR(T);
	} 
	
	if(balance < -1 && strcmp(T->rchild->p,p) > 0)
	{
		return RL(T);
	} 
	
	return T;
	  
}

void Find(AVL T,char *p,FILE *f)
{
	if(T)
	{
		int t = cmp(T->p,p);
		if(t == 0)
		{
			sCount++;
			fprintf(f,"%s yes\n",p);
			return;
		}
		else if(t > 0)
		{
			Find(T->lchild,p,f);	
		} 
		else if(t < 0)
		{
			Find(T->rchild,p,f);
		}
		
	}
	else if(!T)
	{
		fprintf(f,"%s no\n",p);
		return;
	}
}

/*void preOrder(AVL T,FILE* f)
{
	if(T)
	{
		fprintf(f,"%s\n",T->p);
		preOrder(T->lchild,f);
		preOrder(T->rchild,f);
	}
}*/

int main()
{
	AVL T = NULL;
	
	FILE* fp = fopen("../patterns-127w.txt", "r");  
	if(fp == NULL)
	{
		printf("The patterns-127w.txt file open failure...\n");
		return(0);
	}
	int j=0,wCount=0,pCount=0;
	char pat[70]; 
	char ch;
	char* find = NULL;

	
	while(!feof(fp)) 
	{
		fgets(pat,70,fp);                                
		find = strchr(pat, '\n');          
		if(find)                            
			*find = '\0';
		else
			pat[strlen(pat)] = '\0';
		T = Insert(T,pat);			
	}
	
	
	FILE* fp1 = fopen("../words-98w.txt", "r");  
	if(fp1 == NULL)
	{
		printf("The words-98w.txt file open failure...\n");
		return(0);
	}
	FILE* fp2 = fopen("result.txt", "w");  
	if(fp2 == NULL)
	{
		printf("The result.txt file distribute failure...\n");
		return(0);
	}
	
	while(!feof(fp1)) 
	{
		fgets(pat,70,fp1);                                
		find = strchr(pat, '\n');          
		if(find)                            
			*find = '\0';
		else
			pat[strlen(pat)] = '\0';
		Find(T,pat,fp2);
		wCount++;			
	}
	
	
	fprintf(fp2,"%d ",nCount);             //树节点个数 
	fprintf(fp2,"%lld ",mCount / 1024);    //总共需要分配的内存（单位KB）
	fprintf(fp2,"%lld ",cmpCount / 1000);  //字符比较的次数（单位千次）   
	fprintf(fp2,"%d ",wCount);            //words文件的总个数
	fprintf(fp2,"%d",sCount);             //匹配成功的字符串数量
	
	/*FILE* fp3 = fopen("result1.txt", "w");  
	if(fp3 == NULL)
	{
		printf("The result.txt file distribute failure...\n");
		return(0);
	}
	preOrder(T,fp3);fclose(fp3);*/
	
	fclose(fp);fclose(fp1);fclose(fp2);
	return 0;
}
