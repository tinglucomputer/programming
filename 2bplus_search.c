#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define M (5)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2) 
#define Unavailable NULL

int sCount = 0;
long long cmpCount = 0;
int nCount = 0;
long long mCount = 0;

typedef struct BPlusNode *BPlusTree,*Position;
typedef char* KeyType;
struct BPlusNode
{    
	int KeyNum;    
	KeyType Key[M + 1];    
	BPlusTree Children[M + 1];    
	BPlusTree Next;
};

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

void* bupt_malloc(int size)
{
	mCount += size;
	if(size <= 0)
	{
		return NULL;
	}
	return malloc(size);
}

//生成节点并初始化
static BPlusTree MallocNewNode()
{   
	nCount++; 
	BPlusTree NewNode;    
	int i;    
	NewNode = bupt_malloc(sizeof(struct BPlusNode));    
	if (NewNode == NULL)        
		exit(EXIT_FAILURE);            
	i = 0;    
	while (i < M + 1)
	{        
		NewNode->Key[i] = NULL;        
		NewNode->Children[i] = NULL;        
		i++;    
	}    
	NewNode->Next = NULL;    
	NewNode->KeyNum = 0;        
	return NewNode;
} 

//初始化
BPlusTree Initialize()
{        
	BPlusTree T;        
	if (M < (3))
	{        
		printf("M最小等于3！");        
		exit(EXIT_FAILURE);            
	}   
	/* 根结点 */    
	T = MallocNewNode();        
	return T;
}

/* 当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置   
当要对Parent插入X节点的时候，i是要插入的位置，Key和j的值没有用 
isKey用来对以上两种状态的区分 
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j)
{        
	int k;    
	if (isKey)
	{        
		//插入key         
		k = X->KeyNum - 1;   //为即将插入的Key腾地方        
		while (k >= j)
		{            
			X->Key[k + 1] = X->Key[k];
			k--;        
		}
		X->Key[j] = bupt_malloc(strlen(Key)+1);               
		strcpy(X->Key[j],Key);    		//插入j位置
		//printf("插入X->Key[j]\n");
		//插入子节点一个新的key必须要更新父结点中相应的key                
		if (Parent != NULL)              
			Parent->Key[i] = X->Key[0];                
			
		X->KeyNum++;            
	}
	else
	{        
		//插入节点                
		//对树叶节点进行连接        
		if (X->Children[0] == NULL)
		{            
			if (i > 0)                
				Parent->Children[i - 1]->Next = X;            
			X->Next = Parent->Children[i];        
		}                
		k = Parent->KeyNum - 1;        
		while (k >= i)
		{            
			Parent->Children[k + 1] = Parent->Children[k];            
			Parent->Key[k + 1] = Parent->Key[k];            
			k--;        
		}        
		Parent->Key[i] = X->Key[0];        
		Parent->Children[i] = X;                
		Parent->KeyNum++;            
	}    
	return X;
}





static BPlusTree SplitNode(Position Parent,Position X,int i)
{    
	int j,k,Limit;    
	Position NewNode;        
	NewNode = MallocNewNode();        
	k = 0;    
	j = X->KeyNum / 2;    
	Limit = X->KeyNum;    
	while (j < Limit)                  //把关键字较大的部分移入NewNode节点中 至多多一个元素 
	{        
		if (X->Children[0] != NULL)     //X不是树叶节点 
		{            
			NewNode->Children[k] = X->Children[j];            
			X->Children[j] = NULL;        
		}  
		//元素移入操作      
		NewNode->Key[k] = X->Key[j];        
		X->Key[j] = Unavailable;        
		NewNode->KeyNum++;
		X->KeyNum--;        
		j++;
		k++;    
	}
	if (Parent != NULL)        
		InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable);    
	else
	{        
		//如果是X是根，那么创建新的根并返回        
		Parent = MallocNewNode();        
		InsertElement(0, Parent, X, Unavailable, 0, Unavailable);
		InsertElement(0, Parent, NewNode, Unavailable, 1, Unavailable);   
	    return Parent;    
	}        
	return X;
}

static BPlusTree RecursiveInsert(BPlusTree T,KeyType Key,int i,BPlusTree Parent)
{    
	int j,Limit;    
	Position Sibling; 
	       
	//查找分支     
	j = 0;    
	while (j < T->KeyNum && strcmp(Key,T->Key[j]) >= 0)
	{        
		//重复值不插入        
		if (strcmp(Key,T->Key[j]) == 0)            
			return T;        
		j++;    
	}    
	if (j != 0 && T->Children[0] != NULL) j--;       //T是一个分支节点 应该插入到j-1孩子中  这是根据这棵二叉树的结构来说的 
	       
	//树叶    
	if (T->Children[0] == NULL)
	{
		T = InsertElement(1, Parent, T, Key, i, j);
		//printf("叶子节点是%s\n",Key);
	}        
		    
	//内部节点     
	else        
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);  
		      
	//先插入节点再进行调整节点        
	Limit = M;        
	if (T->KeyNum > Limit)
	{        
		T = SplitNode(Parent, T, i);
	}
	if (Parent != NULL)        
		Parent->Key[i] = T->Key[0];            
	return T;
}


//插入 
BPlusTree Insert(BPlusTree T,KeyType Key)
{

    return RecursiveInsert(T, Key, 0, NULL);

}




void Search(BPlusTree T,char* p,FILE* f)
{
	
	int j = 0;    
	if(T)
	{
		while (j < T->KeyNum && cmp(p,T->Key[j]) >= 0)
		{                
			if (strcmp(p,T->Key[j]) == 0) 
			{
				fprintf(f,"%s yes\n",p);
				sCount++;
				return;
			}                   
			j++;    
		}    
		j--;
		if(j == -1)
		{
			fprintf(f,"%s no\n",p);
			return;
		}
		else
		{
			Search(T->Children[j],p,f);
		}
	}
	else
	{
		fprintf(f,"%s no\n",p);
		return;
	}
	
}

int main(int argc, char *argv[]) 
{
	int i;

    BPlusTree T;

    T = Initialize();
    
    int wCount = 0;
    char buf[70];
    FILE* f1 = fopen("../patterns-127w.txt", "r"); 
	FILE* f2 = fopen("../words-98w.txt","r");
	FILE* f3 = fopen("result.txt","w");
	if(!f1 || !f2)
	{
		printf("The txt file open failure...\n");
		return(0);
	}
	
	char* find = NULL; 
	while (!feof(f1))									
	{ 		
		fgets(buf,1024,f1);
		find = strchr(buf,'\n');                               
		if(find)                            
			*find = '\0';
		else
			buf[strlen(buf)] = '\0';        
		T = Insert(T,buf);
		
	}
	find = NULL;
	while(!feof(f2))
	{
		fgets(buf,1024,f2);
		find = strchr(buf,'\n');
		if(find)
			*find = '\0';
		else
			buf[strlen(buf)] = '\0';
		Search(T,buf,f3);
		wCount++;
	}
	
	fprintf(f3,"%d ",nCount);             //树节点个数 
	fprintf(f3,"%lld ",mCount / 1024);    //总共需要分配的内存（单位KB）
	fprintf(f3,"%lld ",cmpCount / 1000);  //字符比较的次数（单位千次）   
	fprintf(f3,"%d ",wCount);            //words文件的总个数
	fprintf(f3,"%d",sCount);             //匹配成功的字符串数量
    
        
    
    fclose(f1);fclose(f2);fclose(f3);
	return 0;
}
