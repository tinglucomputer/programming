#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//结点最大数量 
#define MAX 340000
//COO型数组的长度 
#define COO_LENGTH 3100000
//最终选择的页面是多少 
#define TOPNUM 10

//使用COO型压缩方法存储稀疏矩阵	
void BuildCoo(int source, int dest, int* coo_row, int* coo_column);
void ValueCoo(int* coo_column, int* LinkOutNum, double* coo_values);	
void CalPageRank(int* coo_row, int* coo_column, double* coo_values, double* pagerank, double* lastrank);
void PageTop(double* pagerank, FILE* f1);

long long mCount = 0;
//稀疏矩阵真实的元素数量 
int URLNUM = 0; 	
int COONUM = 0;

int topID[TOPNUM];

void* bupt_malloc(int size)
{
	if(size <= 0)
	{
		return NULL;
	}
	mCount += size;
	return malloc(size);
}


int main()
{

    FILE* url = fopen("../url.txt", "r");
	FILE* top10 = fopen("top10.txt", "w");
	FILE* f1 = fopen("top10.txt","w");
	FILE* f2 = fopen("pagerank.txt","w");
	
    if(url == NULL)
    {
    	printf("file url.txt read failed!\n");
    	return -1;
	}
	if(top10 == NULL)
    {
    	printf("file top10.txt write failed!\n");
    	return -1;
	}
	
	//COO型数据结构 
	int* coo_row = bupt_malloc(COO_LENGTH * sizeof(int));
	int* coo_column = bupt_malloc(COO_LENGTH * sizeof(int));
	double* coo_values = bupt_malloc(COO_LENGTH * sizeof(double));
	
	//每个页面的pagerank值 
	double* pagerank = bupt_malloc(MAX * sizeof(double));

	//在算法上lastrank数组相当于Vk pagerank数组是Vk+1
	double* lastrank = bupt_malloc(MAX * sizeof(double));
	
	//节点的出度 
	int* LinkOutNum = bupt_malloc(MAX * sizeof(int));
	
	if(!coo_row || !coo_column || !coo_values ||
		!pagerank || !lastrank || !LinkOutNum)
	{
		printf("The memory is enough\n");
		exit(0);
	}
	char row[20], strSourceID[10], strDestID[10];
	int space = 0, end = 0, sourceID = 0, destID = 0;
	int i;

	//读取url文件获得节点之间地指向关系 
	
	/*for(i = 0;i < 13 ;i++)
	{
		scanf("%d%d",&sourceID, &destID);
		BuildCoo(sourceID, destID, coo_row, coo_column);
		LinkOutNum[sourceID]++;
		URLNUM++;
	}*/
	
	while(!feof(url))
    {
        fgets(row, 20, url);
        
        memset(strSourceID, 0, 10);
        memset(strDestID, 0, 10);
        for (space = 0; row[space] != ' ' ; ++space);
            strncpy(strSourceID, row, space);
        sourceID = atoi(strSourceID);
        for (end = space + 3; row[end] != '\n' ; ++end);
            strncpy(strDestID, row + space + 3, end - space - 2);
        destID = atoi(strDestID);
        
        LinkOutNum[sourceID]++;
       
        URLNUM++;
        
    }
	 
    ValueCoo(coo_column, LinkOutNum, coo_values);
    
    CalPageRank(coo_row, coo_column, coo_values, pagerank, lastrank);
    
    for(i = 0; i< MAX; i++)
    {
    	fprintf(f2,"%.16lf\n",pagerank[i]);
	}
    PageTop(pagerank,f1);
    
    fclose(url);fclose(top10);fclose(f1);fclose(f2);
	return 0;
}

void BuildCoo(int source, int dest, int* coo_row, int* coo_column)
{
    if(COONUM == COO_LENGTH)
    {
        printf("ERROR : the coo has been full \n");
        exit(0);
    }
    else
    {
        coo_column[COONUM] = source;	//Gm矩阵列表示源点 行标示终点 
        coo_row[COONUM] = dest;
        COONUM++;
    }
}

void ValueCoo(int* coo_column, int* LinkOutNum, double* coo_values)
{
	int i;
    for(i = 0; i < COONUM; i++)
    {
        int sourceID = coo_column[i];   
        int linkNum = LinkOutNum[ sourceID ];
        double value = 0.85 * 1.0 / linkNum;  	//other value = 0.15 / URLNUM
        coo_values[i] = value;
    }
}


/*void CalPageRank(int* coo_row, int* coo_column, double* coo_values, double* pagerank, double* lastrank)
{
   
	int i;
     
    double limitation = 0.00001;
    double distance = 1;

    double comf = 0.15 / URLNUM;
     
	
	for(i = 0; i < MAX; i++)
	{
		pagerank[i] = 1.0 / URLNUM;	
	}
    
    while(distance >= limitation * limitation)
    {
        
        for(i = 0; i < MAX; i++)		//每次循环都需要把把pagerank(Vk+1)更新为lastrank(Vk) 
        {
            lastrank[i] = pagerank[i]; 
        }
        for(i = 0; i< COONUM; i++)		//计算新的pagerank 使用修正模型 
        {
            pagerank[coo_row[i]] =  coo_values[i] * lastrank[coo_column[i]] + comf * lastrank[coo_column[i]];
        }
        distance = 0;					//计算|Vk+1 - Vk|的值 来判断是否迭代中止 
        for(i = 0; i < MAX; i++)
        {
            distance = (lastrank[i] - pagerank[i]) * (lastrank[i] - pagerank[i]) + distance;
        }
        printf("distance = %.16lf\n",distance);
    }
	
    if(distance < limitation * limitation)
    {
        printf("Deal Finished!\n");
    }
	
}*/

void CalPageRank(int* coo_row, int* coo_column, double* coo_values, double* pagerank, double* lastrank)
{
    printf("  **  ---  **   \n");
    
	int i;
    
    double limitation = 0.00000001;
    double distance = 1;

    double comf = 0.15 / URLNUM;
    double comp = 0, tmp = 0;
	
	for(i = 0; i < MAX; i++)
	{
		pagerank[i] = 1.0 / MAX;	
	}
	
    while(distance > limitation * limitation)
    {
        comp = 0;
        for(i =0; i < MAX; i++)
        {
            lastrank[i] = pagerank[i];
            pagerank[i] = 0;
            comp = comp + lastrank[i];
        }
        //comp 记录了lastrank数组的总和 
        for(i = 0; i < MAX; i++)
        {
        	pagerank[i] = comp * comf;
		}
        
        for(i = 0; i < COO_LENGTH; i++)
        {
            pagerank[ coo_row[i] ] = coo_values[i] * lastrank[ coo_column[i] ] + pagerank[ coo_row[i] ];
            pagerank[ coo_row[i] ] = pagerank[ coo_row[i] ] + comf * lastrank[ coo_column[i] ];
        }
        distance = 0;
        for(i = 0; i < MAX; i++)
        {
            distance = (lastrank[i] - pagerank[i]) * (lastrank[i] - pagerank[i]) + distance;
           
        }
        printf("distance = %.16lf\n",distance);
    }

}


void PageTop(double* pagerank, FILE* f1)
{
        
    double topPagerank[TOPNUM];
    int i = 0;
    int j = 0;
    int q;
    for(i = 0; i < MAX; i++)
    {
        for(j = 0;j < TOPNUM; j++)
        {
            if(pagerank[i] > topPagerank[j])	//腾出j位置给pagerank[i] 
            {
                for(q = TOPNUM - 1; q > j; q--)
                {
                    topPagerank[q] = topPagerank[q-1];
                    topID[q] = topID[q-1];
                }
                topPagerank[j] = pagerank[i];
                topID[j] = i;   	
                break;
            }
        }
    }
    fprintf(f1,"The topest URL is:\n");
    for(i = 0; i < TOPNUM; i++)
    {
        fprintf(f1,"**%d: %d | %.16lf\n",i,topID[i],topPagerank[i]);
    }
    fprintf(f1,"%lld\n",mCount / 1000);
}
