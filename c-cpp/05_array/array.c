/*
    实现一个支持动态扩容的数组
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 动态数组结构体的定义 */
typedef struct array 
{
    int size;       /* 动态数组大小 */
    int used;       /* 已经使用的大小 */
    int *arr;
} DYNAMIC_ARRAY_T;

/* 动态数组的遍历 */
void DyArray_dump(DYNAMIC_ARRAY_T *array)
{
    int index;

    for (index = 0; index < array->used; index++)
    {
        printf("[%02d]: %08d\n", index, array->arr[index]);
    }
}

/* 动态数组插入操作 */
int DyArray_insert(DYNAMIC_ARRAY_T *array, int elem)
{
    int index = 0;
    if (array->used >= array->size)
    {
        return -1;
    }

    /* 插入元素：从小到大的顺序排序，重复 不插入 */
    for (index = 0; index < array->used; index++) 
    {
        if (elem == array->arr[index])
        {
            return index;
        }


        /* 满足条件，插入 */
        if (elem < array->arr[index])
        {
            memmove(&array->arr[index+1], &array->arr[index], (array->used - index) * sizeof(int));         
            break;
        }
    }

    /* 插入尾部 */     
    array->arr[index] = elem;
    array->used++;
    return index;
}

/* 动态数组删除操作 */
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

/* 动态数组搜索操作,返回索引 */
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

/* 动态数组初始化操作 */
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

/* 动态数组    去初始化操作 */
int DyArray_finit(DYNAMIC_ARRAY_T *DArray)
{
    DArray->size = 0;
    DArray->used = 0;
    free(DArray->arr);
}

/* 主函数 */
int main()
{
    int Ret;
    int index;
    DYNAMIC_ARRAY_T DyArray;

    /* 动态数组初始化:        大小是10个int数据 */
    Ret = DyArray_init(&DyArray, 10);
    if (0 != Ret)
    {
        return Ret;
    }

    /* 插入数据 */
    DyArray_insert(&DyArray, 1);
    DyArray_insert(&DyArray, 3);
    DyArray_insert(&DyArray, 2);
    printf("=== insert 1, 3, 2\n");

    /* 遍历并打印动态数组, */
    DyArray_dump(&DyArray);

    /* 搜索动态数组中的数据 */
    index = DyArray_search(&DyArray, 2);
    printf("2 is at position %d\n", index);
    index = DyArray_search(&DyArray, 9);
    printf("9 is at position %d\n", index);
    
    /* 删除动态数组中的数据 */
    printf("=== delete [6] element \n");
    DyArray_delete(&DyArray, 6);
    DyArray_dump(&DyArray);
    printf("=== delete [0] element \n");
    DyArray_delete(&DyArray, 0);
    DyArray_dump(&DyArray);

    DyArray_finit(&DyArray);
    return 0;
}
