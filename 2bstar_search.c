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
	//�������ַ��任��ANSI�������������ʽ�� Ӧ��ע�⵽������ʽ��ת�����޷������ı�ʾ ���Ӧ��ʹ���޷�������ָ��ȥ���� 
} 

/* ���ɽڵ㲢��ʼ�� */
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

/* ��ʼ�� */
BPlusTree Initialize()
{        
	BPlusTree T;        
	if (M < (3))
	{        
		printf("M��С����3��");        
		exit(EXIT_FAILURE);            
	}   
	//�����     
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

/* Ѱ��һ���ֵܽڵ㣬��洢�Ĺؼ���δ�������򷵻�NULL 
parent->Children[i]ָ��ľ������ڵ� �����õ������ָ�����ָ���������ֵܽڵ��ж��Ƿ��� 
*/
static Position FindSibling(Position Parent,int i)
{    
	Position Sibling;    
	int Limit;        
	Limit = M;        
	Sibling = NULL;    
	if (i == 0)                                        //λ��0���� 
	{        
		if (Parent->Children[1]->KeyNum < Limit)            
			Sibling = Parent->Children[1];             //λ��1δ��    
	}    
	else if (Parent->Children[i - 1]->KeyNum < Limit)  //���ֵ�δ��        
		Sibling = Parent->Children[i - 1];    
	else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit)   //���ڵ㲻�����һ�������ֵ� 
	    Sibling = Parent->Children[i + 1];            
	return Sibling;
}

/* 
��Ҫ��X����Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�����λ��   
��Ҫ��Parent����X�ڵ��ʱ��i��Ҫ�����λ�ã�Key��j��ֵû���� 
isKey��������������״̬������ 
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j)
{        
	int k;    
	if (isKey)
	{    
		//��Key����X�ڵ�    
		//����key
		k = X->KeyNum - 1;   //Ϊ���������Key�ڵط�        
		while (k >= j)
		{            
			X->Key[k + 1] = X->Key[k];
			k--;        
		}
		X->Key[j] = malloc(strlen(Key) + 1);              
		strcpy(X->Key[j],Key);    		//����jλ��
		
		//�����ӽڵ�һ���µ�key����Ҫ���¸��������Ӧ��key                
		if (Parent != NULL)              
			Parent->Key[i] = X->Key[0];                
			
		X->KeyNum++;            
	}
	else
	{        
		//�Ѻ���X�ڵ���뵽Parent���ڵ���
		                 
		//����Ҷ�ڵ��������      
		if (X->Children[0] == NULL)
		{            
			if (i > 0)                
				Parent->Children[i - 1]->Next = X;            
			X->Next = Parent->Children[i];        
		}

		         
		k = Parent->KeyNum - 1;        
		while (k >= i)                        //�ڳ�����λ�� 
		{            
			Parent->Children[k + 1] = Parent->Children[k];            
			Parent->Key[k + 1] = Parent->Key[k];            
			k--;        
		}
		Parent->Key[i] = malloc(strlen(X->Key[0]) + 1);        
		strcpy(Parent->Key[i],X->Key[0]);          //����
		       
		Parent->Children[i] = X;			//�޸ĺ���ָ��                
		Parent->KeyNum++;
		       
	}    
	return X;
}


/* 
��Ҫ��Xɾ��Key��ʱ��i��X��Parent��λ�ã�j��Key��λ��   
��Ҫ��Parentɾ��X�ڵ��ʱ��i��X��λ�ã�Key��j��ֵû���� 
isKey��������������״̬������ 
*/
static Position RemoveElement(int isKey, Position Parent,Position X,int i,int j)
{        
	int k,Limit;        
	if (isKey)
	{        
		Limit = X->KeyNum;        
		//ɾ��key        
		k = j + 1;        
		while (k < Limit)   //j���Ԫ����ǰ���Ǿ���ζ��ɾ����j 
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
		//ɾ���ڵ� 
		             
		//�޸���Ҷ�ڵ������      
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

/* Src��Dst���������ڵĽڵ� i��Src��Parent�е�λ�� ��Src��Ԫ���ƶ���Dst�� n���ƶ�Ԫ�صĸ���*/
static Position MoveElement(Position Src,Position Dst,Position Parent,int i,int n)
{    
	KeyType TmpKey;    
	Position Child;    
	int j,SrcInFront;        
	SrcInFront = 0;       //Dst�����ֵ�      
	if (cmp(Src->Key[0],Dst->Key[0]) < 0)        
		SrcInFront = 1;   //Dst�����ֵ�      
	j = 0;    
	    
	if (SrcInFront)
	{   //ÿ���ƶ����ֵ     
		if (Src->Children[0] != NULL)
		{   //Src�Ƿ�Ҷ�ӽڵ�         
			while (j < n) 
			{                
				Child = Src->Children[Src->KeyNum - 1];                
				RemoveElement(0, Src, Child, Src->KeyNum - 1, Unavailable);                
				InsertElement(0, Dst, Child, Unavailable, 0, Unavailable);     //����λ�õ�Key���ɺ��ӽڵ����                
				j++;            
			}        
		}
		else
		{   //Src��Ҷ�ӽڵ�         
			while (j < n) 
			{                
				TmpKey = Src->Key[Src->KeyNum -1];                
				RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);                
				InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);                
				j++;            
			}                    
		}                
		Parent->Key[i + 1] = Dst->Key[0];        
		//����Ҷ�ڵ���������         
		if (Src->KeyNum > 0)            
			FindMostRight(Src)->Next = FindMostLeft(Dst);           
	}
	else
	{   
		//ÿ���ƶ���Сֵ     
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
	while (j < Limit)                  //�ѹؼ��ֽϴ�Ĳ�������NewNode�ڵ��� �����һ��Ԫ�� 
	{        
		if (X->Children[0] != NULL)     //X������Ҷ�ڵ㻹��Ҫ�Ѻ��ӵ�ָ�������½���� 
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
	
	 
	if (Parent != NULL)  //X�ǷǸ���� �½����Parent�ڵ���i+1λ�ô�       
		InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable);    
	else
	{        
		//�����X�Ǹ� ��ô�����µĸ������� �����Children����0��1λ�÷ֱ�ָ��X���½��         
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
	while (j < T->KeyNum && cmp(Key,T->Key[j]) >= 0)
	{        
		//�ظ�ֵ������         
		if (cmp(Key,T->Key[j]) == 0)             
			return T;        
		j++;    
	}    
	if (j != 0 && T->Children[0] != NULL) j--; 
	       
	//��Ҷ    
	if (T->Children[0] == NULL)
	{
	
		T = InsertElement(1, Parent, T, Key, i, j);
	}        
		    
	//�ڲ��ڵ�     
	else        
		T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);  
		      
	//�Ȳ���ڵ��ٽ��е����ڵ�      
	Limit = M;        
	if (T->KeyNum > Limit)
	{        
		//��       
		if (Parent == NULL)
		{            
			//���ѽڵ�
			           
			T = SplitNode(Parent, T, i);
			
			        
		}        
		else    //�Ǹ��ڵ�ķ���  ����ķ�����B*��ʵ�ַ�ʽ �ŵ��Ǽ����½ڵ������(�����˲������ֵܽڵ�) 
		{
		            
			Sibling = FindSibling(Parent, i);
			           
			if (Sibling != NULL)
			{                
				//��T��һ��Ԫ�أ�Key����Child���ƶ���Sibing��               
				MoveElement(T, Sibling, Parent, i, 1);            
			}
			else
			{                
				//���ѽڵ�                
				T = SplitNode(Parent, T, i);            
			}        
		}            
	}
	if (Parent != NULL)    //���ѽڵ����֮�� ��Ҫ���¸ýڵ��ڸ��ڵ�λ���ϵ�ֵ i�Ǳ����        
		Parent->Key[i]=T->Key[0];            
	return T;
}


//���� 
BPlusTree Insert(BPlusTree T,KeyType Key)
{

    return RecursiveInsert(T, Key, 0, NULL);

}

//���� 
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

//���� 
void Travel(BPlusTree T,FILE* f)
{    
	RecursiveTravel(T, 0,f);    
	
} 

//������Ҷ�ڵ������ 
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
	//��һƬ��Ҷ    
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
