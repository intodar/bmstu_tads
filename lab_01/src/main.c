/*
 * Гузев Вячеслав ИУ7-32Б Типы и структуры данных
 * Смоделировать операцию деления действительного числа на действительное
 * число в форме +- m.n Е +- K, где суммарная длина
 * мантиссы (m+n) - до 30 значащих
 * цифр, а величина порядка K - целое число. Результат выдать в форме
 * +- 0.m1 Е +- K1, где m1 - до 30 значащих цифр, а K1 - до 5 цифр.
 *
 * Способ задания числа - структура, содержащая одномерный статический целочисленный массив
 * из 31 ячейки и целочисленную переменную, отведённую под порядок
 * 0я ячейка - знак числа (1 если положительное, -1 если отрицательное)
 * с 1й по 30ю ячейку - однозначные числа.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define CORRECT_NUMBER 0
#define WRONG_NUMBER -1

struct bigfloat
{
    int num[31];
    int exp;
};


void DIV(struct bigfloat a, struct bigfloat *b, struct bigfloat *res);

int MORE(struct bigfloat *a, struct bigfloat *b);

void SIMPLIFY(struct bigfloat *a);

void SUB(struct bigfloat *a, struct bigfloat *b);

void MOVE_FORWARD(struct bigfloat *a);

void MOVE_BACK(struct bigfloat *a);

void DelNull(struct bigfloat *a);

int CheckNull(struct bigfloat *a);

int CheckInput(char *string, int n);

int InputBigFloat(char *string, int n, struct bigfloat *input);

void OutputBigFloat(struct bigfloat *output);

void FileOutputBigFloat(struct bigfloat *output);

int LastNum(int *num);

void NullNum(int *num);


int main(void)
{
    printf("DIVISION OF TWO NUMBERS\n"
           "-------------------------------------------------------------------------------------\n"
           "INPUT REQUIREMENTS :\n"
           "Right input : number looks like (+/-)123.123(Е/е)(+/-)123 without leading zeros,\n"
           "with no more than 30 symbols in mantissa, excluding dividing point,\n"
           "and with integer power.\n"
           "Symbols which can be included : E/e, +/-, power of number.\n"
           "Different variants of input : 123, 123e1, -123e-1, .123, 123.123, 123.1e1\n"
           "Please input divided and divisor number with no more than 99999 result's power\n"
           "-------------------------------------------------------------------------------------\n");

    printf("\nInput divided number filling requirements : ");

    char a[40];
    strcpy(a, "");

    scanf("%s", a);
    struct bigfloat a_struct;
    int error = InputBigFloat(a, strlen(a), &a_struct);
    if (error)
    {
        printf("Divided number input error!\n"
               "It looks like requirements are not satisfied\n"
               "Repeat input filling requirements\n");
        return error;
    }


    printf("\nInput divisor : ");

    char b[40];
    strcpy(b, "");

    scanf("%s", b);
    struct bigfloat b_struct;
    error = InputBigFloat(b, strlen(b), &b_struct);
    if (error)
    {
        printf("Divisor input error!\n"
               "It looks like requirements are not satisfied\n"
               "Repeat input filling requirements\n");
        return error;
    }

    struct bigfloat res_struct;
    error = CheckNull(&b_struct);
    if (!error)
        DIV(a_struct, &b_struct, &res_struct);
    else
    {
        printf("\nError! Zero division");
        return error;
    }

    printf("\nDivision result : ");
    OutputBigFloat(&res_struct);

    return error;
}


void DIV(struct bigfloat a, struct bigfloat *b, struct bigfloat *res)
{
    res->exp = a.exp - b->exp + 1;
    NullNum(res->num);
    int curr = 0;
    int hit = 0;

    for (int i = 1; i < 32; i++)
    {
        curr = 0;
        while (MORE(&a, b))
        {
            hit++;
            curr++;
            SUB(&a, b);
        }
        if (i < 31)
        {
            if (hit == 0)
            {
                res->exp--;
            }
            if (b->num[30] > 0)
            {
                b->num[29]++;
            }
            MOVE_FORWARD(b);
            SIMPLIFY(b);
            res->num[i] = curr;
        }
        else
        {
            if (b->num[30] > 0)
            {
                res->num[30] +=1;
                SIMPLIFY(res);
            }
        }

        if (CheckNull(&a))
            break;
    }
    res->num[0] = a.num[0] * b->num[0];
    DelNull(res);
}

int MORE(struct bigfloat *a, struct bigfloat *b)
{
    for (int i = 1; i < 31; i++)
    {
        if (a->num[i] > b->num[i])
            return 1;
        else if (a->num[i] < b->num[i])
            return 0;
    }
    return 1;
}

void SIMPLIFY(struct bigfloat *a)
{
    for (int i = 30; i > 1; i--)
    {
        if (a->num[i] > 9)
        {
            a->num[i - 1]++;
            a->num[i] -= 10;
        }
    }
    if (a->num[1] > 9)
    {
        a->num[1] -= 10;
    }
}

void SUB(struct bigfloat *a, struct bigfloat *b)
{
    int carry = 0;
    for (int i = 30; i > 0 || carry; --i)
    {
        a->num[i] -= carry + ((i > 0) ? b->num[i] : 0);
        carry = a->num[i] < 0;
        if (carry) a->num[i] += 10;
    }
}

void MOVE_FORWARD(struct bigfloat *a)
{
    for (int i = 30; i > 1; i--)
    {
        a->num[i] = a->num[i - 1];
    }
    a->num[1] = 0;
}

void MOVE_BACK(struct bigfloat *a)
{
    for (int i = 1; i < 30; i++)
    {
        a->num[i] = a->num[i + 1];
    }
    a->num[30] = 0;
}

void DelNull(struct bigfloat *a)
{
    if (LastNum(a->num) > 1)
        while (a->num[1] == 0)
            MOVE_BACK(a);
}

int CheckNull(struct bigfloat *a)
{
    if (LastNum(a->num) == 0)
        return 1;
    else
        return 0;
}

int CheckInput(char *string, int n)
{
    int error;

    error = CORRECT_NUMBER;

    if (!error)
    {
        int count_dots = 0;
        int count_e = 0;
        int count_exp = 0;
        int count_nums = 0;
        int count_signs_b = 0;
        int count_signs_a = 0;
        for (int i = 0; i < n; i++)
        {
            if (string[i] == 0 && count_signs_b == 0 && count_dots == 0 && string[i + 1] != '.')
                error = WRONG_NUMBER;
            else if (isdigit(string[i]) && count_e == 0)
                count_nums++;
            else if (isdigit(string[i]))
                count_exp++;
            else if ((string[i] == '+' || string[i] == '-') && count_e == 0)
            {
                if (i == 0)
                    count_signs_b++;
                else
                    error = WRONG_NUMBER;
            }
            else if (string[i] == '+' || string[i] == '-')
            {
                if (string[i - 1] == 'e' || string[i - 1] == 'E')
                    count_signs_a++;
                else
                    error = WRONG_NUMBER;
            }
            else if (string[i] == 'e' || string[i] == 'E')
                count_e++;
            else if (string[i] == '.' && count_e == 0)
                count_dots++;
            else
                error = WRONG_NUMBER;
        }
        if (!(count_nums < 31
              && count_e < 2
              && count_dots < 2
              && count_signs_b < 2
              && count_signs_a < 2))
            error = WRONG_NUMBER;
    }
    return error;
}

int InputBigFloat(char *string, int n, struct bigfloat *input)
{
    NullNum(input->num);

    int error = CheckInput(string, n);

    if (error == WRONG_NUMBER)
        return error;

    int before_e = 1;

    input->num[0] = 1;

    input->exp = 0;

    int sign = 1;

    int current = 1;

    int dot_switch = 0;

    int dot_was = 0;

    for (int i = 0; i < n; i++)
    {
        if (string[i] == '-' && before_e)
            input->num[0] = -1;
        else if (string[i] == '+' && before_e)
        {
            input->num[0] = 1;
        }
        else if (isdigit(string[i]) && before_e)
        {
            if (!(LastNum(input->num) == 0 && string[i] == '0'))
            {
                input->num[current++] = (int) *(string + i) - 48;
            }
            else if (dot_was)
                dot_switch--;
        }
        else if (string[i] == '.')
        {
            dot_switch = current - 1;
            dot_was = 1;
        }
        else if (isdigit(string[i]))
        {
            input->exp += (int) *(string + i) - 48;
            if (i < n - 1)
                input->exp *= 10;
        }
        else if (string[i] == '-')
            sign = -1;
        else if (string[i] == '+')
            sign = 1;
        else if (string[i] == 'e' || string[i] == 'E')
        {
            before_e = 0;
        }
        if (!dot_was && before_e && string[i] != '+' && string[i] != '-')
            dot_switch++;
    }
    input->exp *= sign;
    input->exp += dot_switch;
    return error;
}


void OutputBigFloat(struct bigfloat *output)
{
    if (abs(output->exp) < 100000)
    {
        if (!CheckNull(output))
        {
            if (output->num[0] < 0)
                printf("-");
            printf("%s", LastNum(output->num) != 0 ? "0." : "0");
            int last_num = LastNum(output->num);
            for (int i = 1; i < last_num + 1; i++)
            {
                printf("%d", output->num[i]);
            }
            printf("E");
            printf("%d\n", output->exp);
        }
        else
        {
            printf("0\n");
        }
    }
    else
    {
        printf("Result output error!\n"
               "It looks like you have result power more than 99999.\n"
               "Result power must be filled in [-99999, 99999] range!\n");
    }
}

void FileOutputBigFloat(struct bigfloat *output)
{
    FILE *file = fopen("../tests/pout.txt", "w");
    if (abs(output->exp) < 100000)
    {
        if (!CheckNull(output))
        {
            if (output->num[0] < 0)
                fprintf(file, "-");
            fprintf(file, "%s", LastNum(output->num) != 0 ? "0." : "0");
            int last_num = LastNum(output->num);
            for (int i = 1; i < last_num + 1; i++)
            {
                fprintf(file, "%d", output->num[i]);
            }
            fprintf(file, "E");
            fprintf(file, "%d\n", output->exp);
        }
        else
        {
            fprintf(file, "0\n");
        }
    }
    else
    {
        fprintf(file, "Result output error!\n"
                      "It looks like you have result power more than 99999.\n"
                      "Result power must be filled in [-99999, 99999] range!\n");
    }
}

int LastNum(int *num)
{
    int result = 0;
    for (int i = 1; i < 31; i++)
        if (num[i] != 0)
            result = i;
    return result;
}

void NullNum(int *num)
{
    for (int i = 0; i < 31; i++)
    {
        num[i] = 0;
    }
}
