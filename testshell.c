#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void help();
void read(char *lba_address);
void write(char *lba_address, char *data);
void fullread();
void fullwrite();
void testapp1();
void testapp2();
int lba_num_valid_check(int lba_num);
int data_valid_check(char *data);

void help()
{
    printf("\n\n");
    printf("테스트 셸 매뉴얼\n\n");
    printf("* 최초 실행 시 fullwrite 명령어를 입력해 nand.txt 파일을 생성하세요.\n\n");
    printf("W \n");
    printf("    W LBA주소 4바이트값\n");
    printf("    * ex) W 3 0xABCDABCD\n");
    printf("    * LBA 주소는 0 - 99 사이입니다.\n");
    printf("    * 반드시 0xAAAAAAAA 형식의 16진수 10글자로 입력합니다.\n");
    printf("    * 결과는 nand.txt 에 기록됩니다.\n");
    printf("\n");
    printf("R \n");
    printf("    R LBA주소\n");
    printf("    ex) R 4\n");
    printf("    * 결과는 result.txt 에 작성됩니다.\n");
    printf("\n");
    printf("fullwrite \n");
    printf("    fullwrite\n");
    printf("    * nand.txt 의 모든 값을 0xABCDFFFF 로 변경합니다.\n");
    printf("\n");
    printf("fullread \n");
    printf("    fullread\n");
    printf("    * nand.txt 의 모든 LBA 에 대해 read 수행 후, 셸에 모든 LBA 값을 출력합니다.\n");
    printf("\n");
    printf("testapp1 \n");
    printf("    testapp1 \n");
    printf("    * fullwrite 수행 후 fullread 수행합니다\n");
    printf("\n");
    printf("testapp2 \n");
    printf("    testapp2 \n");
    printf("    * 0 ~ 5 번 LBA 에 0xAAAABBBB 값으로 총 30번 Write를 수행합니다.\n");
    printf("    * 0 ~ 5 번 LBA 에 0x12345678 값으로 1 회 Over Write를 수행합니다.\n");
    printf("    * 0 ~ 5 번 LBA Read 했을 때 정상적으로 값이 읽히는지 확인합니다.\n");
    printf("\n");
    printf("exit 명령어 입력 시 이 프로그램은 종료됩니다.");
    printf("\n\n");
}

void read(char *lba_address)
{
    char command[50];
    sprintf(command, "./ssd R %s", lba_address);
    system(command);
}

void write(char *lba_address, char *data)
{
    char command[50];
    sprintf(command, "./ssd W %s %s", lba_address, data);
    system(command);
}

void fullread()
{
    char buf[12];
    FILE *fp = fopen("nand.txt", "r");

    system("./ssd fullread");
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 12, fp);
        printf("%s", buf);
        if (feof(fp))
        {
            break;
        }
    }
    fclose(fp);
}

void fullwrite()
{
    system("./ssd fullwrite");
}

void testapp1()
{
    fullwrite();
    fullread();
}

void testapp2()
{
    // 0 - 5 번 LBA 에 0xAAAABBBB 값으로 총 30번 write 수행
    char lineNumber[3];
    char *inputs[6] = {
        "0xAAAABBBB", "0xAAAABBBB",
        "0xAAAABBBB", "0xAAAABBBB",
        "0xAAAABBBB", "0xAAAABBBB"};
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            snprintf(lineNumber, sizeof(lineNumber), "%d", j);
            write(lineNumber, inputs[j]);
        }
    }

    // 0 - 5 번 LBA 에 0x12345678 값으로 1 회 Over Write 수행
    char *input = "0x12345678";
    for (int i = 0; i < 6; i++)
    {
        char lineNumber[2];
        snprintf(lineNumber, sizeof(lineNumber), "%d", i);
        write(lineNumber, input);
    }

    // 0 - 5 번 LBA READ
    for (int i = 0; i < 6; i++)
    {
        char lineNumber[2];
        snprintf(lineNumber, sizeof(lineNumber), "%d", i);
        read(lineNumber);
    }
}

int lba_num_valid_check(int lba_num)
{
    if (lba_num >= 0 && lba_num <= 99)
    {
        return 1;
    }
    return 0;
}

int data_valid_check(char *data)
{
    if (strlen(data) != 10)
    {
        return 0;
    }

    if (data[0] != '0' || data[1] != 'x')
    {
        return 0;
    }

    for (int i = 2; i < strlen(data); i++)
    {
        if (!isxdigit(data[i]))
        {
            return 0;
        }
    }

    unsigned int hexValue;
    sscanf(data, "%x", &hexValue);

    if (hexValue < 0 || hexValue > 0xFFFFFFFF)
    {
        return 0;
    }

    return 1;
}

int main()
{
    printf("최초 실행 시 fullwrite 명령어를 입력해 nand.txt 파일을 생성하세요.\n");
    char buf[10];
    while (1)
    {
        printf("shell > ");
        scanf("%s", buf);
        if (!strcmp(buf, "help"))
        {
            help();
        }
        else if (!strcmp(buf, "R"))
        {
            char lba_address[3];
            int lba_num;
            scanf("%s", lba_address);
            lba_num = atoi(lba_address);
            if (lba_num_valid_check(lba_num))
            {
                read(lba_address);
            }
            else
            {
                printf("INVALID COMMAND\n");
                continue;
            }
        }
        else if (!strcmp(buf, "W"))
        {
            char lba_address[3], data[11];
            int lba_num;
            scanf("%s", lba_address);
            scanf("%s", data);
            lba_num = atoi(lba_address);
            if (lba_num_valid_check(lba_num) && data_valid_check(data))
            {
                write(lba_address, data);
            }
            else
            {
                printf("INVALID COMMAND\n");
                continue;
            }
        }
        else if (!strcmp(buf, "fullwrite"))
        {
            fullwrite();
        }
        else if (!strcmp(buf, "fullread"))
        {
            fullread();
        }
        else if (!strcmp(buf, "testapp1"))
        {
            testapp1();
        }
        else if (!strcmp(buf, "testapp2"))
        {
            testapp2();
        }
        else if (!strcmp(buf, "exit"))
        {
            printf("bye\n");
            break;
        }
        else
        {
            printf("INVALID COMMAND\n");
            continue;
        }
    }
    return 0;
}
