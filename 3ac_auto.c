#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#define M 800000


unsigned long long mCount = 0;
unsigned long long cmpCount = 0;

typedef struct ACNode
{
	int count;
	struct ACNode* fail;
	struct ACNode* next[256]; 
}AC;

struct StrNode
{
	int num;
	char* str;
};

AC* q[M];


void* bupt_malloc(unsigned int size)
{
	if(size <= 0)
	{
		return NULL;
	}
	mCount += size;
	return malloc(size);
}

AC* newNode()
{
	int i;
	AC* newN = bupt_malloc(sizeof(AC));
	newN->fail = NULL;
	newN->count = 0;
	for(i=0;i<256;i++)
	{
		newN->next[i] = NULL; 
	}
	return newN;	
}

void Insert(AC* root,unsigned char *s,int index)
{
    AC *p = root;
    int i;
    for(i = 0; s[i]; i++)
    {
        int x = s[i];
        if(p->next[x] == NULL)
        {
            AC* newN = newNode();
            p->next[x] = newN;
        }
        p = p->next[x];
    }
    p->count = index;     //表示该节点是第几个字符串 
}

void BuildFail(AC* root)
{
    int head = 0,tail = 1,i;	//头和尾指针初始化 
    q[head] = root;
    AC *p, *temp;
    while(head != tail)			//循环队列为空的情况 
    {
        temp = q[head];   		//出队
		head = (head + 1) % M; 
        //printf("%d %d\n",head,tail);
        for(i = 0; i < 256; i++)	//为每一个孩子增加fail指针 
        {
            if(temp->next[i])
            {
                if(temp == root)	//根节点 
                {
                    temp->next[i]->fail = root; 	//根节点的孩子fail指针都指向根节点 
                }
                else
                {
                    p = temp->fail;		//非根节点的孩子结点 
                    while(p)
                    {
                        if(p->next[i])	//如果不为空 则说明已经找到了一个相同的结点 
                        {
                            temp->next[i]->fail = p->next[i];
                            break;
                        }
                        p = p->fail;
                    }
                    if(p == NULL) temp->next[i]->fail = root; 	//并没有找到相同节点 指向根节点 
                }
                q[tail] = temp->next[i];		//把该节点加入队尾中
				tail = (tail + 1) % M; 
            }
        }
    }
}

void Automation(AC* root,unsigned char *ch,struct StrNode* strN)
{
    AC *p = root;
    int len = strlen(ch),i = 0;
   	
    for(i = 0; i < len; i++)
    {
        int x = ch[i];
        while(!p->next[x] && p != root) 
		{
			p = p->fail;	//回退
			cmpCount++;
		}
        p = p->next[x];
        if(!p) p = root;				//出现了当前节点不存在的孩子 则表明当前节点的匹配失败 需要回退到根节点 
        AC *temp = p;
        while(temp != root)
        {
           if(temp->count > 0)
           {
           		strN[temp->count - 1].num++; 
           }
           temp = temp->fail; 	//如果当前的匹配成功 那么fail指针处的也可能会是成功的
		   cmpCount++; 
        }
    }
    
}


void DirectInsertSort(struct StrNode* a,int end)
{
	int i,j;
	for(i = 1;i <= end;i++)
	{
		if(a[i].num > a[i-1].num)
		{
			struct StrNode tmp = a[i];			//需要存储一个拷贝 因为移位时把值冲掉了 
			j = i-1;
			while(a[j].num < tmp.num && j >= 0)  
			{
				a[j+1] = a[j];
				j--;
			}	//该种状态下是稳定的，但是<=时是不稳定的
			a[j+1] = tmp;
			
		}	
	}	
}



/*void QuickSort(struct StrNode a[],int l,int r)
{
	
	if(l < r)
	{
		int i = l,j = r;
		struct StrNode p = a[l];                  //p是轴值
		while(i < j)
		{
			while(i < j && p.num >= a[j].num) j--;
			if(i < j)
			{
				struct StrNode temp = a[i]; a[i] = a[j]; a[j] = temp;
				i++;
			}
			while(i < j && a[i].num >= p.num) i++;
			if(i < j)
			{
				struct StrNode temp = a[i]; a[i] = a[j]; a[j] = temp;
				j--;
			}
			a[i] = p;
		}
		//printf("%d %d %d\n",l,i,r);
		QuickSort(a, l, i - 1);
		QuickSort(a, i + 1, r);
	}	
}*/

int PreProcess(struct StrNode* a,int n)
{
	int i=n-1,j=n-1;
	for(;j>=0;j--)
	{
		if(a[j].num == 0)
		{
			struct StrNode tmp = a[j];
			a[j] = a[i];
			a[i] = tmp;
			i--;
		}
	}
	return i;	
}

int main() 
{
	FILE* f1 = fopen("../string.txt","r");
	FILE* f2 = fopen("../pattern.txt","r");
	FILE* f3 = fopen("result.txt","w");
	
	
	if(!f1 || !f2)
	{
		printf("The file opens failure\n");
		exit(0);
	}
	int i = 0,j = 0;
	char pat[70];
	char* find = NULL; 
	unsigned char* str = bupt_malloc(1000000000);

	AC* root = newNode();
	
	printf("string字符串长度：\n"); 
	char ch = fgetc(f1);		//不能用unsigned char读入 否则读不完 
	while(ch != EOF)
	{
		str[i++] = ch;
		ch = fgetc(f1);
	} 
	str[i] = '\0';
	printf("%d\n",i);
	
	i = 0;		//统计pattern字符串数量 
	while(!feof(f2))
	{
		fgets(pat,70,f2);
		i++; 
	}
	struct StrNode* strN = bupt_malloc(i*sizeof(struct StrNode));		//申请pattern文件字符串空间 
	for(j=0;j<i;j++)
	{
		strN[j].num = 0; 
		strN[j].str = NULL;
	} 
	
	rewind(f2); 
	printf("模式串文件的字符串数量：\n");
	i = 0;
	while(!feof(f2))
	{
		fgets(pat,70,f2);
		find = strchr(pat,'\n');
		if(find)
		{
			*find = '\0';
		}
		else pat[strlen(pat)] = '\0';
		strN[i].str = bupt_malloc(strlen(pat)+1);
		strcpy(strN[i].str,pat);
		strN[i].str[strlen(pat)] = '\0';
		i++;
		Insert(root,pat,i);	
	}
	printf("%d\n",i);
	
	printf("BuildFailPointer\n");
	BuildFail(root);
	
	printf("Automation\n");
	Automation(root,str,strN);
	 
	printf("Sort %d\n",i);
	j = PreProcess(strN,i);		//1721361   535329
	//QuickSort(strN,0,i);
	DirectInsertSort(strN,j);	//1964s 	371s
	  
	printf("fprintf\n"); 
	for(j=0;j<i;j++)
	{
		fprintf(f3,"%s %d\n",strN[j].str,strN[j].num);  
	}
	
	fprintf(f3,"%lld ",cmpCount / 1000);
	fprintf(f3,"%lld\n",mCount / 1000);
	
	fclose(f1);fclose(f2);fclose(f3);
	return 0;
}
