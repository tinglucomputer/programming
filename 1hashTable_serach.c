#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define num 100000

long long cmpCount = 0;  //包括建立hash表的比较次数 
int sCount = 0;        //字符串匹配成功的个数
long long mCount = 0; // 内存分配的字节数
 
struct SNode   //孩子节点 
{
	char* pat;
	struct SNode* next;
};

struct PNode   //父节点 
{
	struct SNode* first;
};

int cmpChar(char a,char b)
{
	cmpCount++;
	return a - b;
}

int cmp(char *a,char *b)
{
	int i = 0;
	while(a[i] && b[i])
	{
		if(cmpChar(a[i],b[i]) != 0)
		{
			return 1;               //不匹配 
		}
		i++;
	}
	if(!a[i] && !b[i])              //同时结束匹配   匹配成功 
	{
		return 0;               	
	} 
	return 1;                  //不匹配 
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

unsigned int RSHash(const char* str)    //哈希函数 为了便于测试限定
{
	unsigned int b    = 378551;
	unsigned int a    = 63689;
	unsigned int hash = 0;
	unsigned int i    = 0;
	while(*str)
	{
		hash = hash * a + (*(str++));
		a    = a * b;
	
	}
	return hash % num;                   //
}

void Build_HashTable(struct PNode* HashP)   //初始化该哈希链表 
{
	if(HashP == NULL)
	{
		printf("The memory distributes failure...\n");
		exit(0);
	}
    int i = 0;
    for(;i<num;i++)
    {
		HashP[i].first = NULL;         //为每个父节点的头指针置空 
    }
    return;
}


void Insert_HashTable(unsigned int index, const char* ch,struct PNode *P)
{
	struct SNode* s = bupt_malloc(sizeof(struct SNode));    //插入前要先申请一个子节点 
	if(s == NULL)
	{
		printf("The SNode request failure...\n");
		exit(0);
	}
	//尾插法建立哈希链 如果连中出现相同的字符串则不需要再次插入
	struct SNode* q = P[index].first;
	struct SNode* qp = NULL; 
	if(q ==  NULL)                          //没有头结点时 
	{
		s->pat = bupt_malloc(strlen(ch)+1); 
		strcpy(s->pat,ch);                  //设置该子节点的字符串数据
		 
		P[index].first = s;
		s->next = NULL;
	}
	else
	{
		while(q)                          //依次遍历该父节点下的子节点 
		{
			if(!strcmp(q->pat,ch))
			{
				free(s);s = NULL;
				return;
			}
			qp = q;
			q = q->next;
		}
		s->pat = bupt_malloc(strlen(ch)+1); 
		strcpy(s->pat,ch);                  //设置该子节点的字符串数据 
		qp->next = s;
		s->next = NULL;	
	} 
	
}

void Find_HashTable(unsigned int index, const char* ch,struct PNode *P,FILE* fp)
{
	int flag = 0;
	struct SNode* q = P[index].first;
	while(q)                          //依次遍历该父节点下的子节点 
	{
		if(!cmp(q->pat,ch))
		{
			
			fprintf(fp,"%s yes\n",ch);
			flag = 1;             //找到节点
			sCount++; 
			break;
		}
		q = q->next;
	}
	if(!flag)                    //未找到该节点  失败 
	{
		fprintf(fp,"%s no\n",ch);	
	}
		
}


int main()
{
	struct PNode* HashP = bupt_malloc(num*sizeof(struct PNode));
	
	Build_HashTable(HashP);
	
	char pat[70];
	FILE* fp1 = fopen("patterns-127w.txt", "r");
	if(fp1 == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}
	int j=0;char ch;
	int pCount = 0;                                //用于记录模式串文件的总记录数 
	while((ch = fgetc(fp1))!=EOF)
	{
		if(ch != '\n')
		{
			pat[j] = ch;
			j++;
		}
		else if(ch == '\n')
		{
			pat[j] = '\0';
			Insert_HashTable(RSHash(pat),pat,HashP);   //一个字符串读取完毕可以进行插入哈希链 
			pCount++; 
			j=0;
		}
	}
	pat[j] = '\0';
	Insert_HashTable(RSHash(pat),pat,HashP);         //最后一个字符串的插入 
	pCount++;
	
	
	FILE* fp = fopen("words-98w.txt", "r");  //文件编码为ANSI
	if(fp == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}
	//开始读取第二个文件进行匹配 
	j=0;
	int wCount = 0;
	FILE* fp2 = fopen("result.txt","w");
	if(fp2 == NULL)
	{
		printf("The file opens failure...\n");
		exit(0);
	}
	while((ch = fgetc(fp))!=EOF)
	{
		if(ch != '\n')
		{
			pat[j] = ch;
			j++;
		}
		else if(ch == '\n')
		{
			pat[j] = '\0'; 	
			Find_HashTable(RSHash(pat),pat,HashP,fp2);
			wCount++;
			j=0;
		}
	}
	pat[j] = '\0';                                                                      //  读入到最后时 需要添加结束标志
	Find_HashTable(RSHash(pat),pat,HashP,fp2);
	wCount++;
	
	fprintf(fp2,"%lld ",mCount / 1024);    //全部的节点和申请的字符数组 总共需要分配的内存（单位KB）
	fprintf(fp2,"%lld ",cmpCount / 1000);  //字符比较的次数（单位千次） 
	fprintf(fp2,"%d ",sCount);             //匹配成功的字符串数量  
	fprintf(fp2,"%d\n",wCount);            //words文件的总个数  

	free(HashP);HashP=NULL;
	fclose(fp1);fclose(fp);fclose(fp2);
	return 0;
}

