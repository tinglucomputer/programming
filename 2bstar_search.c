#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define Unavailable NULL
#define M (5)
 

int nCount = 0;
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
		
		if(*s1 == 0) 
		{ 
			return 0; 
		}
	} 
	return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1; 
	//把中文字符变换成ANSI或者其他的码格式是 应该注意到这种形式的转换是无符号数的表示 因此应该使用无符号类型指针去描述 
} 

/* 生成节点并初始化 */
static BPlusTree MallocNewNode()
{  
	nCount++;  
	BPlusTree NewNode;    
	int i;    
	NewNode = malloc(sizeof(struct BPlusNode));    
	if (NewNode == NULL)        
		exit(EXIT_FAILURE);            
	i = 0;    
	while (i < M + 1)
	{        
		NewNode->Key[i] = Unavailable;        
		NewNode->Children[i] = NULL;        
		i++;    
	}    
	NewNode->Next = NULL;    
	NewNode->KeyNum = 0;        
	return NewNode;
} 

/* 初始化 */
BPlusTree Initialize()
{        
	BPlusTree T;        
	if (M < (3))
	{        
		printf("M最小等于3！");        
		exit(EXIT_FAILURE);            
	}   
	//根结点     
	T = MallocNewNode();        
	return T;
}



static Position FindMostLeft(Position P)
{    
	Position Tmp;        
	Tmp = P;        
	while (Tmp != NULL && Tmp->Children[0] != NULL) 
	{        
		Tmp = Tmp->Children[0];    
	}    
	return Tmp;
} 

static Position FindMostRight(Position P)
{    
	Position Tmp;        
	Tmp = P;        
	while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) 
	{        
		Tmp = Tmp->Children[Tmp->KeyNum - 1];   
	}    
	return Tmp;
}

/* 寻找一个兄弟节点，其存储的关键字未满，否则返回NULL 
parent->Children[i]指向的就是满节点 必须用到父结点指针才能指向其他的兄弟节点判断是否满 
*/
static Position FindSibling(Position Parent,int i)
{    
	Position Sibling;    
	int Limit;        
	Limit = M;        
	Sibling = NULL;    
	if (i == 0)                                        //位置0满了 
	{        
		if (Parent->Children[1]->KeyNum < Limit)            
			Sibling = Parent->Children[1];             //位置1未满    
	}    
	else if (Parent->Children[i - 1]->KeyNum < Limit)  //左兄弟未满        
		Sibling = Parent->Children[i - 1];    
	else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit)   //满节点不是最后一个并且兄弟 
	    Sibling = Parent->Children[i + 1];            
	return Sibling;
}

/* 
当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置   
当要对Parent插入X节点的时候，i是要插入的位置，Key和j的值没有用 
isKey用来对以上两种状态的区分 
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j)
{        
	int k;    
	if (isKey)
	{    
		//把Key插入X节点    
		//插入key
		k = X->KeyNum - 1;   //为即将插入的Key腾地方        
		while (k >= j)
		{            
			X->Key[k + 1] = X->Key[k];
			k--;        
		}
		X->Key[j] = malloc(strlen(Key) + 1);              
		strcpy(X->Key[j],Key);    		//插入j位置
		
		//插入子节点一个新的key必须要更新父结点中相应的key                
		if (Parent != NULL)              
			Parent->Key[i] = X->Key[0];                
			
		X->KeyNum++;            
	}
	else
	{        
		//把孩子X节点插入到Parent父节点中
		                 
		//对树叶节点进行连接      
		if (X->Children[0] == NULL)
		{            
			if (i > 0)                
				Parent->Children[i - 1]->Next = X;            
			X->Next = Parent->Children[i];        
		}

		         
		k = Parent->KeyNum - 1;        
		while (k >= i)                        //腾出插入位置 
		{            
			Parent->Children[k + 1] = Parent->Children[k];            
			Parent->Key[k + 1] = Parent->Key[k];            
			k--;        
		}
		Parent->Key[i] = malloc(strlen(X->Key[0]) + 1);        
		strcpy(Parent->Key[i],X->Key[0]);          //插入
		       
		Parent->Children[i] = X;			//修改孩子指针                
		Parent->KeyNum++;
		       
	}    
	return X;
}


/* 
当要对X删除Key的时候，i是X在Parent的位置，j是Key的位置   
当要对Parent删除X节点的时候，i是X的位置，Key和j的值没有用 
isKey用来对以上两种状态的区分 
*/
static Position RemoveElement(int isKey, Position Parent,Position X,int i,int j)
{        
	int k,Limit;        
	if (isKey)
	{        
		Limit = X->KeyNum;        
		//删除key        
		k = j + 1;        
		while (k < Limit)   //j后的元素向前覆盖就意味着删除了j 
		{            
			X->Key[k - 1] = X->Key[k];
			k++;        
		}                
		X->Key[X->KeyNum - 1] = Unavailable;                
		Parent->Key[i] = X->Key[0];                
		X->KeyNum--;    
	}
	else
	{        
		//删除节点 
		             
		//修改树叶节点的链接      
		if (X->Children[0] == NULL && i > 0)
		{

            Parent->Children[i - 1]->Next = Parent->Children[i + 1];

        }
		        
		Limit = Parent->KeyNum;        
		k = i + 1;        
		while (k < Limit)
		{            
			Parent->Children[k - 1] = Parent->Children[k];            
			Parent->Key[k - 1] = Parent->Key[k];            
			k++;        
		}                
		Parent->Children[Parent->KeyNum - 1] = NULL;        
		Parent->Key[Parent->KeyNum - 1] = Unavailable;                
		Parent->KeyNum--;            
	}    
	return X;
}

/* Src和Dst是两个相邻的节点 i是Src在Parent中的位置 将Src的元素移动到Dst中 n是移动元素的个数*/
static Position MoveElement(Position Src,Position Dst,Position Parent,int i,int n)
{    
	KeyType TmpKey;    
	Position Child;    
	int j,SrcInFront;        
	SrcInFront = 0;       //Dst是左兄弟      
	if (cmp(Src->Key[0],Dst->Key[0]) < 0)        
		SrcInFront = 1;   //Dst是右兄弟      
	j = 0;    
	    
	if (SrcInFront)
	{   //每次移动最大值     
		if (Src->Children[0] != NULL)
		{   //Src是非叶子节点         
			while (j < n) 
			{                
				Child = Src->Children[Src->KeyNum - 1];                
				RemoveElement(0, Src, Child, Src->KeyNum - 1, Unavailable);                
				InsertElement(0, Dst, Child, Unavailable, 0, Unavailable);     //插入位置的Key会由孩子节点更新                
				j++;            
			}        
		}
		else
		{   //Src是叶子节点         
			while (j < n) 
			{                
				TmpKey = Src->Key[Src->KeyNum -1];                
				RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);                
				InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);                
				j++;            
			}                    
		}                
		Parent->Key[i + 1] = Dst->Key[0];        
		//将树叶节点重新连接         
		if (Src->KeyNum > 0)            
			FindMostRight(Src)->Next = FindMostLeft(Dst);           
	}
	else
	{   
		//每次移动最小值     
		if (Src->Children[0] != NULL)
		{            
			while (j < n) 
			{                
				Child = Src->Children[0];                
				RemoveElement(0, Src, Child, 0, Unavailable);                
				InsertElement(0, Dst, Child, Unavailable, Dst->KeyNum, Unavailable);                
				j++;            
			}                    
		}
		else
		{            
			while (j < n) 
			{   
				TmpKey = Src->Key[0];                
				RemoveElement(1, Parent, Src, i, 0);                
				InsertElement(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum);                
				j++;            
			}                    
		}                
		Parent->Key[i] = Src->Key[0];
		        
		if (Src->KeyNum > 0)             
			FindMostRight(Dst)->Next = FindMostLeft(Src);            
	}        
	return Parent;
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
		if (X->Children[0] != NULL)     //X不是树叶节点还需要把孩子的指针移入新结点中 
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
	
	 
	if (Parent != NULL)  //X是非根情况 新结点在Parent节点中i+1位置处       
		InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable);    
	else
	{        
		//如果是X是根 那么创建新的根并返回 父结点Children数组0和1位置分别指向X和新结点         
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
	while (j < T->KeyNum && cmp(Key,T->Key[j]) >= 0)
	{        
		//重复值不插入         
		if (cmp(Key,T->Key[j]) == 0)             
			return T;        
		j++;    
	}    
	if (j != 0 && T->Children[0] != NULL) j--; 
	       
	//树叶    
	if (T->Children[0] == NULL)
	{
	
		T = InsertElement(1, Parent, T, Key, i, j);
	}        
		    
	//内部节点     
	else        
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);  
		      
	//先插入节点再进行调整节点      
	Limit = M;        
	if (T->KeyNum > Limit)
	{        
		//根       
		if (Parent == NULL)
		{            
			//分裂节点
			           
			T = SplitNode(Parent, T, i);
			
			        
		}        
		else    //非根节点的分裂  下面的分裂是B*树实现方式 优点是减少新节点的生成(利用了不满的兄弟节点) 
		{
		            
			Sibling = FindSibling(Parent, i);
			           
			if (Sibling != NULL)
			{                
				//将T的一个元素（Key或者Child）移动的Sibing中               
				MoveElement(T, Sibling, Parent, i, 1);            
			}
			else
			{                
				//分裂节点                
				T = SplitNode(Parent, T, i);            
			}        
		}            
	}
	if (Parent != NULL)    //分裂节点完成之后 需要更新该节点在父节点位置上的值 i是必须的        
		Parent->Key[i]=T->Key[0];            
	return T;
}


//插入 
BPlusTree Insert(BPlusTree T,KeyType Key)
{

    return RecursiveInsert(T, Key, 0, NULL);

}

//销毁 
BPlusTree Destroy(BPlusTree T)
{    
	int i,j;    
	if (T != NULL)
	{        
		i = 0;        
		while (i < T->KeyNum + 1)
		{            
			Destroy(T->Children[i]);
			i++;        
		}                
		printf("Destroy:(");        
		j = 0;        
		while (j < T->KeyNum)       //T->Key[i] != Unavailable           
			printf("%s:",T->Key[j++]);        
		printf(") ");        
		free(T);        
		T = NULL;    
	}        
	
	return T;
} 


static void RecursiveTravel(BPlusTree T,int Level,FILE* f)
{    
	int i;    
	if (T != NULL)
	{        
		fprintf(f,"  ");        
		fprintf(f,"[Level:%d]-->",Level);        
		fprintf(f,"(");        
		i = 0;        
		while (i < T->KeyNum)    //T->Key[i] != Unavailable
		   	fprintf(f,"%s:",T->Key[i++]);        
		fprintf(f,")\n");                
		Level++;                
		i = 0;        
		while (i <= T->KeyNum) 
		{            
			RecursiveTravel(T->Children[i], Level,f);       
			i++;        
		}                    
	}
} 

//遍历 
void Travel(BPlusTree T,FILE* f)
{    
	RecursiveTravel(T, 0,f);    
	
} 

//遍历树叶节点的数据 
void TravelData(BPlusTree T)
{    
	Position Tmp;    
	int i;    
	if (T == NULL)        
		return ;   
	printf("All Data:");    
	Tmp = T;    
	while (Tmp->Children[0] != NULL)        
		Tmp = Tmp->Children[0];    
	//第一片树叶    
	while (Tmp != NULL)
	{        
		i = 0;        
		while (i < Tmp->KeyNum)            
			printf("%s\n",Tmp->Key[i++]);        
		Tmp = Tmp->Next;    
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
	FILE* f = fopen("t.txt","w");
	//FILE* ff = fopen("tt.txt","w");
	
	if(!f1 || !f)
	{
		printf("The file\n");
		exit(0);
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
		//printf("%s\n",buf);        
		T = Insert(T,buf);
		wCount++;
	}
    printf("%d %d\n",wCount,nCount);   
    Travel(T,f);
    
    //TravelData(T); 
	//Destroy(T);
    fclose(f1);fclose(f);//fclose(ff);
	return 0;
}
