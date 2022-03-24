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
        //��ӡ�˵�
        menu();
        //ѡ��
        printf("��ѡ��-> ");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            game();
            break;
        case 0:
            break;
        default:
            printf("ѡ�����������ѡ��\n");
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
    printf("ɨ����Ϸ�����ɨ����Ϸ���ѶȻ������ѡ����Ѷ�������\n");
    printf("�������Ѷȣ����м���,�ո����,����2 * 2��ͼ���������9 * 9)\n-> ");
    while (1)
    {
        scanf("%d %d", &COL, &ROW);
        if ((COL <= 1) || (ROW <= 1))
        {
            printf("�Ѷ�ѡ�����������ѡ��\n-> ");
        }
        else
        {
            break;
        }
    }

    //1.����ɨ��(��Ҫ����һ���Ŀռ�ֱ�Ų��ú��׵���Ϣ���Ų��׵���Ϣ)
    char** checker_mine = (char**)malloc((COL + 2) * sizeof(char*));//������
    char** checker_show = (char**)malloc((COL + 2) * sizeof(char*));//�Ų���
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

    //2.��ʼ��һ�����̣�
    checker_board(COL, ROW, checker_mine, '0');
    checker_board(COL, ROW, checker_show, '*');

    //3.��ӡ����
    print_checker_board(COL, ROW, checker_show);
    //print_checker_board(COL, ROW, checker_mine);

    //4.������
    int mine_num = ste_mine(COL, ROW, checker_mine);
    //print_checker_board(COL, ROW, checker_mine);

    //5.�Ų���
    check_mine(COL, ROW, checker_mine, checker_show, mine_num);

    //�ͷ��ڴ�
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
    printf("----------ɨ����Ϸ----------\n");
    //��ӡ�к�
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
    printf("----------ɨ����Ϸ----------\n");
}

int ste_mine(int COL, int ROW, char** checker)
{
    int mine_num = COL + 1;

    while (mine_num)
    {
        //��������׵�����
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
    //��������
    //�Ų�����λ���Ƿ�����
    //1. �������ը����
    //2. ��������ף�չʾ������ʣ�µ��׵�������

    int x = 0, y = 0;
    int winner_mine = 0;

    while (winner_mine < (COL * ROW - mine_num))
    {
        printf("��������Ҫ�Ų�����꣺\n-> ");
        scanf("%d %d", &x, &y);
        if ((x >= 1) && (x <= ROW) && (y >= 1) && (y <= COL))
        {
            if (*(*(checker_mine + y) + x) == '1')
            {
                printf("�����ˣ� ��Ϸ����\n");
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
            printf("�Ų���������������������룺\n-> ");
        }
    }
    if (winner_mine == (COL * ROW - mine_num))
    {
        printf("��ϲ�㣬ɨ�׳ɹ�������\n");
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