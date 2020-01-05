#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define num 100000

long long cmpCount = 0;  //��������hash��ıȽϴ��� 
int sCount = 0;        //�ַ���ƥ��ɹ��ĸ���
long long mCount = 0; // �ڴ������ֽ���
 
struct SNode   //���ӽڵ� 
{
	char* pat;
	struct SNode* next;
};

struct PNode   //���ڵ� 
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
			return 1;               //��ƥ�� 
		}
		i++;
	}
	if(!a[i] && !b[i])              //ͬʱ����ƥ��   ƥ��ɹ� 
	{
		return 0;               	
	} 
	return 1;                  //��ƥ�� 
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

unsigned int RSHash(const char* str)    //��ϣ���� Ϊ�˱��ڲ����޶�
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

void Build_HashTable(struct PNode* HashP)   //��ʼ���ù�ϣ���� 
{
	if(HashP == NULL)
	{
		printf("The memory distributes failure...\n");
		exit(0);
	}
    int i = 0;
    for(;i<num;i++)
    {
		HashP[i].first = NULL;         //Ϊÿ�����ڵ��ͷָ���ÿ� 
    }
    return;
}


void Insert_HashTable(unsigned int index, const char* ch,struct PNode *P)
{
	struct SNode* s = bupt_malloc(sizeof(struct SNode));    //����ǰҪ������һ���ӽڵ� 
	if(s == NULL)
	{
		printf("The SNode request failure...\n");
		exit(0);
	}
	//β�巨������ϣ�� ������г�����ͬ���ַ�������Ҫ�ٴβ���
	struct SNode* q = P[index].first;
	struct SNode* qp = NULL; 
	if(q ==  NULL)                          //û��ͷ���ʱ 
	{
		s->pat = bupt_malloc(strlen(ch)+1); 
		strcpy(s->pat,ch);                  //���ø��ӽڵ���ַ�������
		 
		P[index].first = s;
		s->next = NULL;
	}
	else
	{
		while(q)                          //���α����ø��ڵ��µ��ӽڵ� 
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
		strcpy(s->pat,ch);                  //���ø��ӽڵ���ַ������� 
		qp->next = s;
		s->next = NULL;	
	} 
	
}

void Find_HashTable(unsigned int index, const char* ch,struct PNode *P,FILE* fp)
{
	int flag = 0;
	struct SNode* q = P[index].first;
	while(q)                          //���α����ø��ڵ��µ��ӽڵ� 
	{
		if(!cmp(q->pat,ch))
		{
			
			fprintf(fp,"%s yes\n",ch);
			flag = 1;             //�ҵ��ڵ�
			sCount++; 
			break;
		}
		q = q->next;
	}
	if(!flag)                    //δ�ҵ��ýڵ�  ʧ�� 
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
	int pCount = 0;                                //���ڼ�¼ģʽ���ļ����ܼ�¼�� 
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
			Insert_HashTable(RSHash(pat),pat,HashP);   //һ���ַ�����ȡ��Ͽ��Խ��в����ϣ�� 
			pCount++; 
			j=0;
		}
	}
	pat[j] = '\0';
	Insert_HashTable(RSHash(pat),pat,HashP);         //���һ���ַ����Ĳ��� 
	pCount++;
	
	
	FILE* fp = fopen("words-98w.txt", "r");  //�ļ�����ΪANSI
	if(fp == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}
	//��ʼ��ȡ�ڶ����ļ�����ƥ�� 
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
	pat[j] = '\0';                                                                      //  ���뵽���ʱ ��Ҫ��ӽ�����־
	Find_HashTable(RSHash(pat),pat,HashP,fp2);
	wCount++;
	
	fprintf(fp2,"%lld ",mCount / 1024);    //ȫ���Ľڵ��������ַ����� �ܹ���Ҫ������ڴ棨��λKB��
	fprintf(fp2,"%lld ",cmpCount / 1000);  //�ַ��ȽϵĴ�������λǧ�Σ� 
	fprintf(fp2,"%d ",sCount);             //ƥ��ɹ����ַ�������  
	fprintf(fp2,"%d\n",wCount);            //words�ļ����ܸ���  

	free(HashP);HashP=NULL;
	fclose(fp1);fclose(fp);fclose(fp2);
	return 0;
}

