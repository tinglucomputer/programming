#include <stdio.h>
#include <stdlib.h>
#include "defination1.h"

/*
采用Bloom Filter方案,n=1271000的模式串,k=10个相互独立的哈希函数
那么,需要的位数m = k * n / 0.693 = 18350000b = 2300000B,
*/ 
long long cmpCount = 0;  
int sCount = 0;         //字符串匹配成功的个数
long long mCount = 0;  //内存分配的字节数 

void* bupt_malloc(size_t size)
{
	mCount += size;
    if(size <= 0)
    {
    	return NULL;
    }
    return malloc(size);
}

int cmp(unsigned char a,unsigned char b)
{
	cmpCount++;
	return  a & b ? 1 : 0;
}

void BF(const char* str,char* p)
{
	unsigned int t = RSHash(str,len);
	unsigned char a = 128;
	//printf("%d ",t); 
	p[t/8] |= a >> t % 8;           //t/8表示在几个字节上  t%8表示第几位  把某一位置1需要知道第几位  
	t = JSHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = PJWHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = ELFHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = BKDRHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = SDBMHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = DJBHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = DEKHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = APHash(str,len);
	//printf("%d ",t);
	p[t/8] |= a >> t % 8;
	t = THash(str,len);
	//printf("%d\n",t);
	p[t/8] |= a >> t % 8;
}

void Check(const char* str,char* p,FILE* f)
{
	unsigned int t = RSHash(str,len);
	unsigned char a = 128;
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}            
	t = JSHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = PJWHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = ELFHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = BKDRHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = SDBMHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = DJBHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = DEKHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = APHash(str,len);
	//printf("%d ",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}    
	t = THash(str,len);
	//printf("%d\n",t);
	if(cmp(p[t/8],a >> t % 8) == 0)
	{
		fprintf(f,"%s no\n",str);
		return;
	}  
	sCount++;  
	fprintf(f,"%s yes\n",str);
}


int main(int argc, char *argv[]) 
{
	unsigned char* p = bupt_malloc(mB);
	int i = 0;
	
	for(;i < mB;i++)                                      //初始化m位数组  采用高高低低的原则 
	{
		p[i] = 0;
	} 
	
	FILE* fp = fopen("patterns-127w.txt", "r");
	if(fp == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}
	int pCount = 0,wCount = 0;                                //用于记录两个文件的总记录数 
	char ch;
	int j = 0;
	char pat[70];
	
	
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
			BF(pat,p);
			pCount++;	
			j=0;
		}
	}
	
	pat[j] = '\0';
	BF(pat,p);
	pCount++;
	FILE* fp2 = fopen("result.txt", "w");
	if(fp2 == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}	
	FILE* fp1 = fopen("words-98w.txt", "r");
	if(fp1 == NULL)
	{
		printf("The file open failure...\n");
		return(0);
	}
	j=0;
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
			Check(pat,p,fp2);
			wCount++;
			j=0;
		}
	}
	pat[j] = '\0';
	Check(pat,p,fp2);
	wCount++;
	fprintf(fp2,"%lld ",mCount / 1024);       //全部的头结点和插入的子节点  总共需要分配内存（单位KB） 
	fprintf(fp2,"%lld ",cmpCount / 1000);     //位比较的次数（单位千次）
	fprintf(fp2,"%d ",sCount);                //匹配成功的字符串数量 
	fprintf(fp2,"%d\n",wCount);               //words文件的总个数 
	free(p);p=NULL;
	fclose(fp);fclose(fp1);fclose(fp2);
	return 0;
}
