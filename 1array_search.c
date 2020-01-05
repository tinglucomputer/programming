#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long cmpCount = 0;        //�ַ��Ƚϴ��� 
long long mCount = 0;         //�ڴ�ռ�õ��ֽ��� 
int sCount = 0;              //�ַ������ɹ��ĸ��� 

typedef struct Node1
{
    char* pat;
}Node;

void* bupt_malloc(size_t size)
{
	mCount += size;
    if(size <= 0)
    {
    	return NULL;
    }
    return malloc(size);
}

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


// Node = struct Node1

int main()
{
	Node* p = bupt_malloc(1271000*sizeof(Node));
	Node* w = bupt_malloc(987000*sizeof(Node));
	if(p == NULL || w == NULL)
	{
		printf("The memory is enough...\n");
		return(0);
    }
    FILE* fp = fopen("words-98w.txt", "r");  //�ļ�����ΪANSI
	if(fp == NULL)
	{
		printf("The words-98w.txt file open failure...\n");
		return(0);
	}
	int i=0,wCount,pCount,j;
	char pat[70];
	char* find = NULL; 
	while (!feof(fp))									//ѭ����ȡÿһ�У�ֱ���ļ�β	
	{ 		
		fgets(pat,70,fp);                               //��fp��ָ����ļ�һ�����ݶ���pat������ 
		find = strchr(pat, '\n');          //���һ��з�
		if(find)                            //���find��Ϊ��ָ��
			*find = '\0';
		w[i].pat = (char *)malloc(strlen(pat)+1);       //��1��ָ���Ľ����� ��ʵ���Բ��ü�1 /nռ����һ���ַ�����Чλ�� 
		strcpy(w[i].pat,pat);
		i++;			
	}
	w[i-1].pat[strlen(pat)] = '\0';        //Ϊ���һ���ַ����ӽ�����־ 
	wCount = i-1;
	//printf("%d\n",wCount);
	i = 0;find = NULL;
	FILE* fp1 = fopen("patterns-127w.txt", "r");
	if(fp1 == NULL)
	{
		printf("The patterns-127w.txt file open failure...\n");
		return(0);
	}
	while (!feof(fp1))									//ѭ����ȡÿһ�У�ֱ���ļ�β	
	{ 		
		fgets(pat,70,fp1);                               //��fp��ָ����ļ�һ�����ݶ���pat������
		find = strchr(pat,'\n');
		if(find)
			*find = '\0'; 
		p[i].pat = bupt_malloc(strlen(pat)+1);
		strcpy(p[i].pat,pat);
		i++;					   					
	}
	p[i-1].pat[strlen(pat)] = '\0';
	pCount = i-1;
	//printf("%d\n",pCount);
	FILE* fp2 = fopen("result.txt","w");   //��Ž�����ļ� 
	if(fp2 == NULL)
	{
	  	printf("The file failure...\n");
	  	return(0);
	}
	 
	for(i=0;i<=wCount;i++)
	{
	  	for(j=0;j<=pCount;j++)
	  	{
	    	if(!cmp(w[i].pat,p[j].pat))
	    	{
	    		fprintf(fp2,"%s yes\n",w[i].pat);   //��ʽ����д��һ���ļ�
	    		sCount++;
				break; 
	    	}
	  	}
	  	if(j == pCount+1)
	  	{
	  		fprintf(fp2,"%s no\n",w[i].pat);       //�������е�patterns�ļ��ַ�����ûƥ�� 
		}
	}
	wCount++;pCount++;                          //�ڴ������±��0��ʼ ������Ҫ��1                           
	fprintf(fp2,"%lld ",mCount / 1024);         //�ڴ�ʹ�õ��ֽ��� 
	fprintf(fp2,"%lld ",cmpCount / 1000);            //�ַ��Ƚϴ���
	fprintf(fp2,"%d ",sCount);                         //�ַ���ƥ��ɹ�����
	fprintf(fp2,"%d\n",wCount);                           //words�ļ����ܼ�¼�� 
	//��β���� 
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	free(p);p=NULL;
	free(w);w=NULL;
	return 0;
}

/*


*/

