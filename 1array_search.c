#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long cmpCount = 0;        //字符比较次数 
long long mCount = 0;         //内存占用的字节数 
int sCount = 0;              //字符搜索成功的个数 

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
    FILE* fp = fopen("words-98w.txt", "r");  //文件编码为ANSI
	if(fp == NULL)
	{
		printf("The words-98w.txt file open failure...\n");
		return(0);
	}
	int i=0,wCount,pCount,j;
	char pat[70];
	char* find = NULL; 
	while (!feof(fp))									//循环读取每一行，直到文件尾	
	{ 		
		fgets(pat,70,fp);                               //将fp所指向的文件一行内容读到pat缓冲区 
		find = strchr(pat, '\n');          //查找换行符
		if(find)                            //如果find不为空指针
			*find = '\0';
		w[i].pat = (char *)malloc(strlen(pat)+1);       //加1是指最后的结束符 其实可以不用加1 /n占据了一个字符的有效位置 
		strcpy(w[i].pat,pat);
		i++;			
	}
	w[i-1].pat[strlen(pat)] = '\0';        //为最后一个字符串加结束标志 
	wCount = i-1;
	//printf("%d\n",wCount);
	i = 0;find = NULL;
	FILE* fp1 = fopen("patterns-127w.txt", "r");
	if(fp1 == NULL)
	{
		printf("The patterns-127w.txt file open failure...\n");
		return(0);
	}
	while (!feof(fp1))									//循环读取每一行，直到文件尾	
	{ 		
		fgets(pat,70,fp1);                               //将fp所指向的文件一行内容读到pat缓冲区
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
	FILE* fp2 = fopen("result.txt","w");   //存放结果的文件 
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
	    		fprintf(fp2,"%s yes\n",w[i].pat);   //格式化的写入一个文件
	    		sCount++;
				break; 
	    	}
	  	}
	  	if(j == pCount+1)
	  	{
	  		fprintf(fp2,"%s no\n",w[i].pat);       //遍历所有的patterns文件字符串都没匹配 
		}
	}
	wCount++;pCount++;                          //内存数组下标从0开始 计数需要多1                           
	fprintf(fp2,"%lld ",mCount / 1024);         //内存使用的字节数 
	fprintf(fp2,"%lld ",cmpCount / 1000);            //字符比较次数
	fprintf(fp2,"%d ",sCount);                         //字符串匹配成功个数
	fprintf(fp2,"%d\n",wCount);                           //words文件的总记录数 
	//收尾工作 
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	free(p);p=NULL;
	free(w);w=NULL;
	return 0;
}

/*


*/

