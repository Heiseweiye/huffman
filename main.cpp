#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#define Max 1000
#define M 2*Max - 1
int OK=0;                        //执行成功标志
typedef struct
{
    int Data;                   //字符值
    int w;                      //节点的权重
    int parent, lchild, rchild  //双亲结点、左孩子、右孩子
}HFMTNode;
                      //定义哈夫曼树节点类型
typedef struct
{
    char HFM[Max];              //编码数组
    int H;                      //指向哈夫曼编码最高位，即起始位置
}HFMCNode;                      //定义哈夫曼编码类型
                                // 生成节点及编码数组
HFMTNode HT[M];
HFMCNode HC[Max];

// 函数声明
void Reset();
void InFile();
int cmp(const void*, const void*);
int Num();
void CreateHT(int);
void CreateHC(int);
void ToFile(int);
void Encode(int);
void Decode(int);
void CodeTable(int);

int main()
{
    int select=0;
    int n=0;
    while(1)
    {
        do
        {
            system("cls");
            printf("                \t\t\t◇欢迎使用B17040914哈夫曼编解码系统◇\n");
            printf("\t\t\t*===================================================================*\n");
            printf("\t\t\t|  ○打开编码依据文件初始化请输 1     ○输出哈夫曼编码表请输入 2    |\n");
            printf("\t\t\t|  ○将文本文件编码并保存请输 3       ○将编码文件解码请输入 4      |\n");
            printf("\t\t\t|                        ○退出程序请输0                            |\n");
            printf("\t\t\t*===================================================================*\n");
            while(scanf("%d", &select)==NULL)
            {
                fflush(stdin);
                printf("◇输入错误(不要包含字母及其他字符)，请重新输入：");
            }
        }while(select!=0&&select!=1&&select!=2&&select!=3&&select!=4);
        switch(select)
        {
            case 1:
                Reset();
                InFile();
                if (OK==1)
                {
                    //排序使得有效字符在前面
                    qsort(HT, Max, sizeof(HFMTNode), cmp);
                    //记下有效字符的个数
                    n = Num();
                    CreateHT(n);
                    CreateHC(n);
                    ToFile(n);
                    system("pause");
                }
                break;
            case 2:
                system("cls");
                if (OK==1)
                {
                    CodeTable(n);
                }
                else
                {
                    printf("依据文件尚未读入!\n");
                }
                system("pause");
                break;
            case 3:
                system("cls");
                if (OK==1)
                {
                    Encode(n);
                }
                else
                {
                    printf("依据文件尚未读入!\n");
                }
                system("pause");
                break;
            case 4:
                system("cls");
                if (OK==1)
                {
                    Decode(n);
                }
                else
                {
                    printf("依据文件尚未读入!\n");
                }
                system("pause");
                break;
            case 0:
                return 0;
            default:
                fflush(stdin);
        }
    }
    return 0;
}
//初始化哈夫曼树
void Reset()
{
    int i;
    for (i = 0; i < M; i++)
    {
        HT[i].w = 0;
        HT[i].parent = HT[i].lchild = HT[i].rchild = -1;//初始化，各域的初值为-1
    }
}
//读入文件内容
void InFile()
{
    FILE *fp;
    char ch;
    char FILENAME[20] = {};
    system("cls");
    printf("请输入依据文件名:");
    fflush(stdin);
    scanf("%s", FILENAME);

    //打开原文件
    if ((fp = fopen(FILENAME, "rt")) == NULL)
    {
        printf("找不到依据文件 %s!\n", FILENAME);
        OK = 0;
        system("pause");
        return;
    }

    //读入字符并处理权重
    while (fscanf(fp, "%c", &ch) != EOF)
    {
        HT[ch].Data = ch;
        HT[ch].w++;
    }
    //关闭文件指针
    fclose(fp);
    printf("文件 %s 读入成功!\n", FILENAME);
    OK = 1;
}
//比较函数
int cmp(const void *a ,const void *b)
{
    return  (*(HFMTNode *)b).w - (*(HFMTNode *)a).w;
}
//统计有效的字符数量
int Num()
{
    int i,num = 0;
    for (i = 0; i < Max; i++)
    {
        if (HT[i].w> 0)
            num++;
    }
    return num;
}
//构造哈夫曼树
void CreateHT(int n)
{
    int i, k ,lmin, rmin;//lmin和rmin为最小权值的两个节点的位置
    int min1, min2;//min1,min2为最小取值的两个节点

    //一共有2n-1个节点
    for (i = n; i < 2 * n - 1; i++)
    {
        min1 = min2 = 0x7FFFFFFF;
        lmin = rmin = -1;
        for (k = 0; k <= i - 1; k++)
        {
            //只在尚未构造二叉树的节点中查找
            if (HT[k].parent == -1)
            {
                if (HT[k].w < min1)
                {
                    min2 = min1; rmin = lmin;
                    min1 = HT[k].w; lmin = k;
                }
                else if (HT[k].w < min2)
                {
                    min2 = HT[k].w;
                    rmin = k;
                }
            }
        }

        // 修改2个小权重节点的双亲
        HT[lmin].parent = i;
        HT[rmin].parent = i;
        //修改双亲的权重
        HT[i].w = HT[lmin].w + HT[rmin].w;
        //修改双亲的孩子
        HT[i].lchild = lmin;
        HT[i].rchild = rmin;
    }
}

//构造哈夫曼编码
void CreateHC(int n)
{
    int i;
    int index;
    int parent;
    //根据哈夫曼树求哈夫曼编码
    for (i = 0; i < n; i++)
    {
        HC[i].H = n;
        index = i;
        parent = HT[i].parent;
        //循序直到树根结点
        while (parent != -1)
        {
            //处理左孩子结点
            if (HT[parent].lchild == index)
            {
                HC[i].HFM[HC[i].H--] = '0';
            }
            //处理右孩子结点
            else
            {
                HC[i].HFM[HC[i].H--] = '1';
            }
            index = parent;
            parent = HT[parent].parent;
        }
        // St指向哈夫曼编码最开始字符
        HC[i].H++;
    }
}
// 输出哈弗曼编码到文件
void ToFile(int n)
{
    FILE *fp;
    int i, j;
    char HCodeFile[20]={};
    printf("你想将编码表保存到文件:");
    fflush(stdin);
    scanf("%s",HCodeFile);
    // 生成哈夫曼编码表文件
    if ((fp = fopen(HCodeFile, "wt+")) == NULL)
    {
        printf("编码表文件 %s 保存失败!\n",HCodeFile);
        return;
    }

    // 将内存里的东西写入文件 */
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%c ", HT[i].Data);
        for (j = HC[i].H; j <= n; j++)
        {
            fprintf(fp, "%c", HC[i].HFM[j]);
        }
        if (i < n - 1) fprintf(fp, "\n");
    }

    // 关闭文件指针
    fclose(fp);
    printf("编码表文件 %s 生成成功!\n",HCodeFile);
}

// 打印哈夫曼编码到屏幕
void CodeTable(int n)
{
    int i, j;
    printf("|字符|  ASCII |哈夫曼编码 \n");
    for (i = 0; i < n; i++)
    {
        printf("|\"%c\" |   %d   |    ", HT[i].Data, HT[i].Data);
        for (j = HC[i].H; j <= n; j++)
        {
            printf("%c", HC[i].HFM[j]);
        }
        printf("\n");
    }
}


//编码
void Encode(int n)
{
    int i, j;
    char ch;
    FILE *fp1, *fp2;
    char HCodeFile[20]={};
    //输入要编码的文件名
    char file[20]={};
    printf("请输入要编码的文件: ");
    fflush(stdin);
    scanf("%s",file);
    //打开要编码的原文件
    if ((fp1 = fopen(file, "rt")) == NULL)
    {
        printf("找不到文件 %s !\n", file);
        return;
    }
    //定义要创建的编码文件名
    char Encodefile[20]={};
    printf("请输入要创建的编码文件名:");
    fflush(stdin);
    scanf("%s",Encodefile);

    //生成编码文件
    if ((fp2 = fopen(Encodefile, "wt+")) == NULL)
    {
        printf("编码文件 %s 保存失败!\n",Encodefile);
        return;
    }

    //一个字符一个字符替换
    printf("文件内容如下:\n");
    while (fscanf(fp1, "%c", &ch) != EOF)
    {
        printf("%c",ch);
        for (i = 0; i < n; i++)              //在哈夫曼数组中找到与读入字符相同的，然后查找哈夫曼编码表进行编码
        {
            if (HT[i].Data == ch)
            {
                for (j = HC[i].H; j <= n; j++)
                {
                    fprintf(fp2, "%c", HC[i].HFM[j]);
                }
                break;
            }
        }
    }
    printf("\n");

    //关闭文件指针
    fclose(fp1);
    fclose(fp2);
    printf("编码成功，编码结果保存至文件 %s!\n",Encodefile);
    printf("编码结果为:\n");
    FILE *fp3;
    fp3 = fopen(Encodefile, "rt+");
    ch = fgetc(fp3);
    while( ch != EOF )
    {
        putchar(ch);
        ch = fgetc(fp3);
    }
    printf("\n");
    fclose(fp3);
}

//解码
void Decode(int n)
{
    int i;
    char ch;
    FILE *fp1, *fp2;

    //输入要解码的文件
    char Encodefile[20]={};
    printf("请输入要解码的文件: ");
    fflush(stdin);
    scanf("%s",Encodefile);

    //打开要解码的文件
    if ((fp1 = fopen(Encodefile, "rt")) == NULL)
    {
        printf("找不到要解码的文件 %s!\n",Encodefile);
        return;
    }
    //输入解码之后所要创建的文件名
    char Decodefile[20]={};
    printf("请输入解码后所要创建的文件名: ");
    fflush(stdin);
    scanf("%s",Decodefile);

    //生成解码文件
    if ((fp2 = fopen(Decodefile, "wt+")) == NULL)
    {
        printf("解码文件 %s 保存失败!\n",Decodefile);
        return;
    }

    //利用哈夫曼树解码
    i = 2 * n - 2;
    printf("文件内容如下:\n");
    while (fscanf(fp1, "%c", &ch) != EOF)
    {
        printf("%c",ch);
        if (ch == '0')
        {
            i = HT[i].lchild;
        }
        else
        {
            i = HT[i].rchild;
        }
        // 找到叶子节点为止
        if (HT[i].lchild == -1)
        {
            fprintf(fp2, "%c", HT[i].Data);
            //继续从根节点开始查找
            i = 2 * n - 2;
        }
    }
    printf("\n");
    //关闭文件指针
    fclose(fp1);
    fclose(fp2);
    printf("解码成功，解码结果保存至文件 %s !\n",Decodefile);
    printf("解码结果为:\n");
    FILE *fp3;
    fp3 = fopen(Decodefile, "rt+");
    ch = fgetc(fp3);
    while( ch != EOF )
    {
        putchar(ch);
        ch = fgetc(fp3);
    }
    printf("\n");
    fclose(fp3);
}




