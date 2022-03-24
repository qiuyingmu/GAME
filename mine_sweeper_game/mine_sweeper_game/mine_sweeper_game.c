#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void menu();

void game();

void checker_board(int COL, int ROW, char** checker, char set);

void print_checker_board(int COL, int ROW, char** checker);

int ste_mine(int COL, int ROW, char** checker);

void check_mine(int COL, int ROW, char** checker_mine, char** checker_show, int mine_num);

int statistics_mine(int COL, int ROW, char** checker);

void statistics_mine_show(int x, int y, char** checker, int statistic);

int main()
{
    int input = 0;
    srand((unsigned int)time(NULL));

    do
    {
        //打印菜单
        menu();
        //选择
        printf("请选择-> ");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            game();
            break;
        case 0:
            break;
        default:
            printf("选择错误，请重新选择\n");
        }
    } while (input);

    return 0;
}

void menu()
{
    printf("******************************\n");
    printf("*****       1.play       *****\n");
    printf("*****       0.exit       *****\n");
    printf("******************************\n");
}

void game()
{
    int COL, ROW, i;
    printf("扫雷游戏，这个扫雷游戏的难度会根据你选择的难度来调整\n");
    printf("请输入难度（几行几列,空格隔开,最少2 * 2的图，尽量最多9 * 9)\n-> ");
    while (1)
    {
        scanf("%d %d", &COL, &ROW);
        if ((COL <= 1) || (ROW <= 1))
        {
            printf("难度选择错误，请重新选择\n-> ");
        }
        else
        {
            break;
        }
    }

    //1.创建扫雷(需要两块一样的空间分别放布置好雷的信息和排查雷的信息)
    char** checker_mine = (char**)malloc((COL + 2) * sizeof(char*));//布置雷
    char** checker_show = (char**)malloc((COL + 2) * sizeof(char*));//排查雷
    if (checker_mine == NULL)
    {
        perror("game");
        return;
    }
    if (checker_show == NULL)
    {
        perror("game");
        return;
    }
    for (i = 0; i < COL + 2; i++)
    {
        *(checker_mine + i) = (char*)malloc((ROW + 2) * sizeof(char));
        if (*(checker_mine + i) == NULL)
        {
            perror("game");
            return;
        }
    }
    for (i = 0; i < COL + 2; i++)
    {
        *(checker_show + i) = (char*)malloc((ROW + 2) * sizeof(char));
        if (*(checker_show + i) == NULL)
        {
            perror("game");
            return;
        }
    }

    //2.初始化一个棋盘；
    checker_board(COL, ROW, checker_mine, '0');
    checker_board(COL, ROW, checker_show, '*');

    //3.打印棋盘
    print_checker_board(COL, ROW, checker_show);
    //print_checker_board(COL, ROW, checker_mine);

    //4.布置雷
    int mine_num = ste_mine(COL, ROW, checker_mine);
    //print_checker_board(COL, ROW, checker_mine);

    //5.排查雷
    check_mine(COL, ROW, checker_mine, checker_show, mine_num);

    //释放内存
    for (i = 0; i < COL + 2; i++)
    {
        free(*(checker_show + i));
        *(checker_show + i) = NULL;
    }
    for (i = 0; i < COL + 2; i++)
    {
        free(*(checker_mine + i));
        *(checker_mine + i) = NULL;
    }
    free(checker_mine);
    free(checker_show);
    checker_mine = NULL;
    checker_show = NULL;
}

void checker_board(int COL, int ROW, char** checker, char set)
{
    int i = 0, j = 0;
    for (i = 0; i < COL + 2; i++)
    {
        for (j = 0; j < ROW + 2; j++)
        {
            *(*(checker + i) + j) = set;
        }
    }
}

void print_checker_board(int COL, int ROW, char** checker)
{
    int i = 0, j = 0;
    printf("----------扫雷游戏----------\n");
    //打印列号
    for (i = 0; i <= COL; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 1; i <= COL; i++)
    {
        printf("%d ", i);
        for (j = 1; j <= ROW; j++)
        {
            printf("%c ", *(*(checker + i) + j));
        }
        printf("\n");
    }
    printf("----------扫雷游戏----------\n");
}

int ste_mine(int COL, int ROW, char** checker)
{
    int mine_num = COL + 1;

    while (mine_num)
    {
        //随机生成雷的坐标
        int x = rand() % ROW + 1;
        int y = rand() % COL + 1;
        if (*(*(checker + x) + y) == '0')
        {
            *(*(checker + x) + y) = '1';
            mine_num--;
        }
    }
    return (COL + 1);
}

void check_mine(int COL, int ROW, char** checker_mine, char** checker_show, int mine_num)
{
    //输入坐标
    //排查坐标位置是否是雷
    //1. 如果是雷炸死；
    //2. 如果不是雷，展示，计算剩下的雷的数量；

    int x = 0, y = 0;
    int winner_mine = 0;

    while (winner_mine < (COL * ROW - mine_num))
    {
        printf("请输入想要排查的坐标：\n-> ");
        scanf("%d %d", &x, &y);
        if ((x >= 1) && (x <= ROW) && (y >= 1) && (y <= COL))
        {
            if (*(*(checker_mine + y) + x) == '1')
            {
                printf("踩雷了， 游戏结束\n");
                print_checker_board(COL, ROW, checker_mine);
                print_checker_board(COL, ROW, checker_show);
                break;
            }
            else if (*(*(checker_mine + y) + x) == '0')
            {
                int statistic = statistics_mine(x, y, checker_mine);
                statistics_mine_show(x, y, checker_show, statistic);
                print_checker_board(COL, ROW, checker_show);
                winner_mine++;
            }
        }
        else
        {
            printf("排查坐标输入错误，请重新输入：\n-> ");
        }
    }
    if (winner_mine == (COL * ROW - mine_num))
    {
        printf("恭喜你，扫雷成功！！！\n");
        print_checker_board(COL, ROW, checker_mine);
        print_checker_board(COL, ROW, checker_show);
    }
}

int statistics_mine(int x, int y, char** checker)
{
    return
        +(*(*(checker + y) + (x - 1)))
        + (*(*(checker + y) + (x + 1)))
        + (*(*(checker + (y - 1)) + x))
        + (*(*(checker + (y + 1)) + x))
        + (*(*(checker + (y - 1)) + (x - 1)))
        + (*(*(checker + (y - 1)) + (x + 1)))
        + (*(*(checker + (y + 1)) + (x - 1)))
        + (*(*(checker + (y + 1)) + (x + 1)))
        - (8 * '0');
}

void statistics_mine_show(int x, int y, char** checker, int statistic)
{
    *(*(checker + y) + x) = (char)(statistic + '0');
}