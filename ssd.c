#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read(char *lineNumber);
void write(char *lineNumber, char *input);
void fullread();
void fullwrite();

void read(char *lineNumber)
{
    FILE *fp = fopen("nand.txt", "r");
    FILE *result = fopen("result.txt", "w");
    char buf[12];
    for (int i = 0; i <= 99; i++)
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, 12, fp);
        if (i == atoi(lineNumber))
        {
            fprintf(result, "%s", buf);
            break;
        }
    }
    fclose(fp);
    fclose(result);
}

void write(char *lineNumber, char *input)
{
    FILE *fp = fopen("nand.txt", "r");
    // 임시 파일 생성
    FILE *temp = fopen("temp.txt", "w");
    char buf[12];
    int cnt = 0;
    // nand.txt 파일에서 한 줄씩 읽어, 임시 파일에 기록
    while (1)
    {
        fgets(buf, 12, fp);
        if (feof(fp))
        {
            break;
        }
        if (cnt == atoi(lineNumber))
        {
            // 새로 작성
            fprintf(temp, "%s\n", input);
        }
        else
        {
            // 아니라면 원래 것 작성
            fprintf(temp, "%s", buf);
        }
        cnt++;
    }
    fclose(fp);
    fclose(temp);
    // nand 파일 지운 후, temp 파일 이름을 nand 로 바꿈
    remove("nand.txt");
    rename("temp.txt", "nand.txt");
}

void fullread()
{
    char buf[12];
    for (int i = 0; i <= 99; i++)
    {
        char lineNumber[3];
        snprintf(lineNumber, sizeof(lineNumber), "%d", i);
        read(lineNumber);
    }
}

void fullwrite()
{
    FILE *fp = fopen("nand.txt", "w");
    for (int i = 0; i < 100; i++)
    {
        fprintf(fp, "0xABCDFFFF\n");
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (!strcmp(argv[1], "R"))
    {
        read(argv[2]);
    }
    if (!strcmp(argv[1], "W"))
    {
        write(argv[2], argv[3]);
    }
    if (!strcmp(argv[1], "fullread"))
    {
        fullread();
    }
    if (!strcmp(argv[1], "fullwrite"))
    {
        fullwrite();
    }
    return 0;
}