#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long cmpCount = 0;
long long mCount = 0;
 
struct StrNode
{
	char* str;
	int num;
};

void* bupt_malloc(unsigned int size)
{
	if(size <= 0)
	{
		return NULL;
	}
	mCount += size;
	return malloc(size);
}


int SPCmp(char* str,char* pat)
{
	int i = 0,j = 0,count = 0,k = -1;
	int lenStr = strlen(str), lenPat = strlen(pat);
	 
	char* next = bupt_malloc(strlen(pat));
	if(!next)
	{
		printf("The memory is enough\n");
		exit(0);
	}
	next[0] = -1;

	while(j < lenPat-1)
	{
		if(k == -1 || pat[j] == pat[k])
		{
			j++;k++;
			if(pat[j] != pat[k])
			{
				next[j] = k;
			}
			else
			{
				next[j] = next[k];
			}
		}
		else
		{
			k = next[k];
		}
	}
	
	j = 0;
	while(i < lenStr )
	{
		cmpCount++;
		if(j == -1 || str[i] == pat[j])
		{
			i++;j++;
			if(j == lenPat)
			{
				j = 0;
				count++;
			}
		}
		else
		{
			j = next[j];	
		}
		  
	}
	return count;
}

int Partition(struct StrNode* a,int l,int r) 
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
	return i;
}

void QuickSort(struct StrNode a[],int l,int r)
{
	if(l==r) return;
	else if(l < r)
	{
		int p=Partition(a,l,r);
		QuickSort(a, l, p - 1);
		QuickSort(a, p + 1, r);
	}
}

int main() 
{
	FILE* f1 = fopen("../string.txt","r");
	FILE* f2 = fopen("../pattern_bf_kmp.txt","r");
	FILE* f3 = fopen("result.txt","w");
	//FILE* f4 = fopen("result1.txt","w");
	
	if(!f1 || !f2 )
	{
		printf("The file open failure\n");
		exit(0);
	}
	int i = 0,max = 0,count,j;
	
	char* str = bupt_malloc(2ll*1024*1024*1024);
	char pat[70]; 
	char* find = NULL;
	char ch = fgetc(f1);
	while(ch!=EOF)
	{
		str[i++] = ch;
		ch = fgetc(f1);
	} 
	str[i] = '\0';
	printf("%d\n",i);
	
	i = 0;          		//获得行数 
	while(!feof(f2))
	{
		fgets(pat,70,f2);
		i++;
  	}
  	rewind(f2);            	//f2文件指针回到原点 
	printf("%d\n",i);
	struct StrNode* strN = bupt_malloc(sizeof(struct StrNode)*i);
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
		count = SPCmp(str,pat);
		strN[i].num = count;
		//printf("%d\n",i);
		//fprintf(f4,"%s %d\n",strN[i].str,strN[i].num);
		i++;
  	}
  	printf("%d\n",i);
	QuickSort(strN,0,i-1);
	for(j=0;j<i;j++)
	{
		fprintf(f3,"%s %d\n",strN[j].str,strN[j].num);
	}
	fprintf(f3,"%lld ",cmpCount / 1000);
	fprintf(f3,"%lld\n",mCount / 1000);
	fclose(f1);fclose(f2);fclose(f3);//fclose(f4); 
	return 0;
}
