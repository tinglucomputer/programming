#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long mCount = 0;
int sCount = 0;
long long cmpCount = 0;
int nCount = 0;

typedef struct TrieNode
{
	int flag;
	struct TrieNode* next[2];
}*Trie;

void* bupt_malloc(int size)
{
	mCount += size;
	if(size <= 0)
	{
		exit(0);
	}
	return malloc(size);
}

Trie newNode()
{
	nCount++;
	Trie x = bupt_malloc(sizeof(struct TrieNode));
	x->flag = 0;
	int j = 0;
	for(;j < 2;j++)
	{
		x->next[j] = NULL;
	}
	return x;	
} 

Trie InsertTrie(Trie T,unsigned char* p)
{
	unsigned char ele;
	int index,i,mol;
	Trie q = T;
	
	while(*p)
	{
		i = 7;mol = 128; 
		ele = *p;
		
		while(i >= 0)
		{
			index = ele >> i;
			
			if(!q->next[index])
			{
				q->next[index] = newNode();
			}
			q = q->next[index];
			ele = ele % mol;
			mol = mol / 2;
			i--;
		}
		
		p++;
	}
	q->flag = 1;
	return T;
}

void SearchTrie(Trie T,char* p,FILE* f)
{
	unsigned char ele;
	int index,i,mol;
	Trie q = T;
	char* tmp = p;
	
	while(*p)
	{
		i = 7;mol = 128; 
		ele = *p;
		cmpCount++;					// 
		while(i >= 0)
		{
			index = ele >> i;
			
			if(!q->next[index])
			{
				fprintf(f,"%s no\n",tmp);
				return;
			}
			q = q->next[index];
			ele = ele % mol;
			mol = mol / 2;
			i--;
		}
		
		p++;
	}
	if(!q->flag)
	{
		fprintf(f,"%s no\n",tmp);
		return;
	}
	sCount++;
	fprintf(f,"%s yes\n",tmp);
	
	return;
} 

int main(int argc, char *argv[]) 
{
	Trie T = NULL;
	char buf[70],*find;
	FILE* f1 = fopen("../patterns-127w.txt","r");
	FILE* f2 = fopen("../words-98w.txt","r");
	FILE* f3 = fopen("result.txt","w");
	int wCount = 0;
	
	T = newNode();
	
	if(!f1 || !f2)
	{
		printf("The txt file open failure\n");
		exit(0);
	}
	find = NULL;
	while(!feof(f1))
	{
		fgets(buf,1024,f1);
		find = strchr(buf,'\n');
		if(find)                   //最后一行之前都会有\n换行符                  
			*find = '\0';
		else						//最后一行不存在换行符  
			buf[strlen(buf)] = '\0';
		T = InsertTrie(T,buf);	
	}
	find = NULL;
	while(!feof(f2))
	{
		fgets(buf,1024,f2);
		find = strchr(buf,'\n');
		if(find)                   //最后一行之前都会有\n换行符                  
			*find = '\0';
		else						//最后一行不存在换行符  
			buf[strlen(buf)] = '\0';
		SearchTrie(T,buf,f3);
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
