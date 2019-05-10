/*
    ʵ��һ��֧�ֶ�̬���ݵ�����
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ��̬����ṹ��Ķ��� */
typedef struct array 
{
    int size;       /* ��̬�����С */
    int used;       /* �Ѿ�ʹ�õĴ�С */
    int *arr;
} DYNAMIC_ARRAY_T;

/* ��̬����ı��� */
void DyArray_dump(DYNAMIC_ARRAY_T *array)
{
    int index;

    for (index = 0; index < array->used; index++)
    {
        printf("[%02d]: %08d\n", index, array->arr[index]);
    }
}

/* ��̬���������� */
int DyArray_insert(DYNAMIC_ARRAY_T *array, int elem)
{
    int index = 0;
    if (array->used >= array->size)
    {
        return -1;
    }

    /* ����Ԫ�أ���С�����˳�������ظ� ������ */
    for (index = 0; index < array->used; index++) 
    {
        if (elem == array->arr[index])
        {
            return index;
        }


        /* �������������� */
        if (elem < array->arr[index])
        {
            memmove(&array->arr[index+1], &array->arr[index], (array->used - index) * sizeof(int));         
            break;
        }
    }

    /* ����β�� */     
    array->arr[index] = elem;
    array->used++;
    return index;
}

/* ��̬����ɾ������ */
int DyArray_delete(DYNAMIC_ARRAY_T *array, int index)
{
    if (index < 0 || index >= array->used)
    {
        return -1;
    }
    
    memmove(&array->arr[index], &array->arr[index+1], (array->used - index - 1) * sizeof(int));
    array->used--;
    return 0;
}

/* ��̬������������,�������� */
int DyArray_search(DYNAMIC_ARRAY_T *array, int elem)
{
    int index = 0;

    for (index = 0; index < array->used; index++) 
    {
        if (array->arr[index] == elem)
        {
            return index;
        }
        
        if (array->arr[index] > elem)
        {
            return -1;
        }
    }

    return -1;
}

/* ��̬�����ʼ������ */
int DyArray_init(DYNAMIC_ARRAY_T *DArray, int ArraySize)
{
    DArray->size = ArraySize;
    DArray->used = 0;
    DArray->arr  = (int *)malloc(ArraySize * sizeof(int));
    if (NULL == DArray->arr)
    {
        return -1;
    }
    return 0;
}

/* ��̬����    ȥ��ʼ������ */
int DyArray_finit(DYNAMIC_ARRAY_T *DArray)
{
    DArray->size = 0;
    DArray->used = 0;
    free(DArray->arr);
}

/* ������ */
int main()
{
    int Ret;
    int index;
    DYNAMIC_ARRAY_T DyArray;

    /* ��̬�����ʼ��:        ��С��10��int���� */
    Ret = DyArray_init(&DyArray, 10);
    if (0 != Ret)
    {
        return Ret;
    }

    /* �������� */
    DyArray_insert(&DyArray, 1);
    DyArray_insert(&DyArray, 3);
    DyArray_insert(&DyArray, 2);
    printf("=== insert 1, 3, 2\n");

    /* ��������ӡ��̬����, */
    DyArray_dump(&DyArray);

    /* ������̬�����е����� */
    index = DyArray_search(&DyArray, 2);
    printf("2 is at position %d\n", index);
    index = DyArray_search(&DyArray, 9);
    printf("9 is at position %d\n", index);
    
    /* ɾ����̬�����е����� */
    printf("=== delete [6] element \n");
    DyArray_delete(&DyArray, 6);
    DyArray_dump(&DyArray);
    printf("=== delete [0] element \n");
    DyArray_delete(&DyArray, 0);
    DyArray_dump(&DyArray);

    DyArray_finit(&DyArray);
    return 0;
}
