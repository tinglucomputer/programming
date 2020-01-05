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
	//�������ַ��任��ANSI�������������ʽ�� Ӧ��ע�⵽������ʽ��ת�����޷������ı�ʾ ���Ӧ��ʹ���޷�������ָ��ȥ���� 
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

//���ɽڵ㲢��ʼ��
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

//��ʼ��
BPlusTree Initialize()
{        
	BPlusTree T;        
	if (M < (3))
	{        
		printf("M��С����3��");        
		exit(EXIT_FAILURE);            
	}   
	/* ����� */    
	T = MallocNewNode();        
	return T;
}

/* ��Ҫ��X����Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�����λ��   
��Ҫ��Parent����X�ڵ��ʱ��i��Ҫ�����λ�ã�Key��j��ֵû���� 
isKey��������������״̬������ 
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j)
{        
	int k;    
	if (isKey)
	{        
		//����key         
		k = X->KeyNum - 1;   //Ϊ���������Key�ڵط�        
		while (k >= j)
		{            
			X->Key[k + 1] = X->Key[k];
			k--;        
		}
		X->Key[j] = bupt_malloc(strlen(Key)+1);               
		strcpy(X->Key[j],Key);    		//����jλ��
		//printf("����X->Key[j]\n");
		//�����ӽڵ�һ���µ�key����Ҫ���¸��������Ӧ��key                
		if (Parent != NULL)              
			Parent->Key[i] = X->Key[0];                
			
		X->KeyNum++;            
	}
	else
	{        
		//����ڵ�                
		//����Ҷ�ڵ��������        
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
	while (j < Limit)                  //�ѹؼ��ֽϴ�Ĳ�������NewNode�ڵ��� �����һ��Ԫ�� 
	{        
		if (X->Children[0] != NULL)     //X������Ҷ�ڵ� 
		{            
			NewNode->Children[k] = X->Children[j];            
			X->Children[j] = NULL;        
		}  
		//Ԫ���������      
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
		//�����X�Ǹ�����ô�����µĸ�������        
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
	       
	//���ҷ�֧     
	j = 0;    
	while (j < T->KeyNum && strcmp(Key,T->Key[j]) >= 0)
	{        
		//�ظ�ֵ������        
		if (strcmp(Key,T->Key[j]) == 0)            
			return T;        
		j++;    
	}    
	if (j != 0 && T->Children[0] != NULL) j--;       //T��һ����֧�ڵ� Ӧ�ò��뵽j-1������  ���Ǹ�����ö������Ľṹ��˵�� 
	       
	//��Ҷ    
	if (T->Children[0] == NULL)
	{
		T = InsertElement(1, Parent, T, Key, i, j);
		//printf("Ҷ�ӽڵ���%s\n",Key);
	}        
		    
	//�ڲ��ڵ�     
	else        
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);  
		      
	//�Ȳ���ڵ��ٽ��е����ڵ�        
	Limit = M;        
	if (T->KeyNum > Limit)
	{        
		T = SplitNode(Parent, T, i);
	}
	if (Parent != NULL)        
		Parent->Key[i] = T->Key[0];            
	return T;
}


//���� 
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
	
	fprintf(f3,"%d ",nCount);             //���ڵ���� 
	fprintf(f3,"%lld ",mCount / 1024);    //�ܹ���Ҫ������ڴ棨��λKB��
	fprintf(f3,"%lld ",cmpCount / 1000);  //�ַ��ȽϵĴ�������λǧ�Σ�   
	fprintf(f3,"%d ",wCount);            //words�ļ����ܸ���
	fprintf(f3,"%d",sCount);             //ƥ��ɹ����ַ�������
    
        
    
    fclose(f1);fclose(f2);fclose(f3);
	return 0;
}
