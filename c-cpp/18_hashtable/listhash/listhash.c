/*************************************************************************
 > File Name: listhash.c
 > Author:  jinshaohui
 > Mail:    jinshaohui789@163.com
 > Time:    18-11-07
 > Desc:    
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"listhash.h"

#ifdef MEMORY_TEST
#include<mcheck.h>
#endif

/************************************************************
   desc: Hash Table 表创建函数 
  input: size               哈希表大小
         hash_value         哈希值计算函数
         keycmp             哈希值比较函数
         hash_node_free     哈希结点释放函数
 output:
 return: hashtab *          哈希表指针
************************************************************/
hashtab * hashtab_create(int size, hash_key_func hash_value, keycmp_func keycmp, hash_node_free_func hash_node_free)
{
    hashtab *h = NULL;
    int i = 0;

    if ((size < 0) || (hash_value == NULL) || (keycmp == NULL))
    {
        return NULL;
    }

    h = (hashtab *)malloc(sizeof(hashtab));
    if (h == NULL)
    {
        return NULL;
    }

    h->htables = (hashtab_node **)malloc(size * sizeof(hashtab_node*));
    if (h->htables == NULL)
    {
        return NULL;
    }

    h->size = size;
    h->nel = 0;
    h->hash_value = hash_value;
    h->keycmp = keycmp;
    h->hash_node_free = hash_node_free;

    for (i = 0; i < size; i++)
    {
        h->htables[i] = NULL;
    }

    return h;
}

/* 销毁哈希表 */
void hashtab_destory(hashtab *h)
{
    int i = 0;
    hashtab_node * cur = NULL;
    hashtab_node * tmp = NULL;

    if (h == NULL)
    {
        return;
    }

    /* 遍历哈希桶 */
    for (i = 0; i < h->size; i++)
    {
        cur = h->htables[i];
        /* 遍历哈希冲突链 */
        while (NULL != cur)
        {
            tmp = cur;
            cur = cur->next;
            h->hash_node_free(tmp);
        }
        h->htables[i] = NULL;
    }
    
    free(h->htables);
    free(h);
    return;
}

/* 哈希表插入key、value */
int hashtab_insert(hashtab *h, void *key, void *data)
{
    unsigned int hvalue = 0;
    int i = 0;
    hashtab_node *cur = NULL; 
    hashtab_node *prev = NULL; 
    hashtab_node *newnode = NULL;

    /* 入参合法性检查 */
    if ( (NULL == h ) || (NULL == key) || (NULL == data) )
    {
        return 1;
    }

    /*获取hash值，确定hash地址，找到hash冲突链的位置cur */
    hvalue = h->hash_value(h, key);
    cur = h->htables[hvalue];

    /* hash桶中元素是从小到大排列的，(遍历hash冲突链)找到要插入的位置  */
    while( (NULL != cur) && (h->keycmp(h, key, cur->key) > 0) )
    {
        prev = cur;
        cur = cur->next;
    }

    /*如果key和当前key比对一致，直接返回，数据已经存在*/
    if ( (NULL != cur) && (0 == h->keycmp(h, key, cur->key)))
    {
        return 2;
    }

    newnode = (hashtab_node *)malloc(sizeof(hashtab_node));
    if (newnode == NULL)
    {
        return 3;
    }

    newnode->key = key;
    newnode->data = data;

    
    if (NULL == prev)
    {
        /* 插入位置是hash冲突链第一个结点，将newnode结点插入cur前面 */
        newnode->next = h->htables[hvalue];
        h->htables[hvalue] = newnode;
    }
    else
    {
        /* 插入位置非hash冲突链第一个结点，将newnode结点插入prev后面 */
        newnode->next = prev->next;
        prev->next = newnode;
    }

    /* hash表中元素个数加1 */
    h->nel++;
    
    return 0;
}

/* 哈希表删除 */
hashtab_node *hashtab_delete(hashtab *h, void *key)
{
    int hvalue = 0;
    int i = 0;
    hashtab_node *cur = NULL; 
    hashtab_node *prev = NULL; 
    
    if ((h == NULL) || (key == NULL))
    {
        return NULL;
    }

    /* 获取hash值, 获取哈希冲突链 */
    hvalue = h->hash_value(h,key);
    cur = h->htables[hvalue];
    
    /*hash桶中元素是从小到大排列的，找到要删除的位置*/
    while((NULL != cur) && (h->keycmp(h, key, cur->key) >= 0))
    {
        if (h->keycmp(h, key, cur->key) == 0)
        {
            if (prev == NULL)
            {
                /* 删除哈希冲突链第一个结点 */
                h->htables[hvalue] = cur->next;
            }
            else
            {
                /* 删除哈希冲突链非第一个结点，直接摘掉 */
                prev->next = cur->next;
            }
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }

    return NULL;
}

/* hash表查找 */
void *hashtab_search(hashtab *h, void *key)
{
    int hvalue = 0;
    int i = 0;
    hashtab_node *cur = NULL; 
    
    if ((NULL == h) || (NULL == key))
    {
        return NULL;
    }

    /* 获取hash值, 获取哈希冲突链 */
    hvalue = h->hash_value(h,key);
    cur = h->htables[hvalue];
    
    /* hash桶中元素是从小到大排列的，找到要插入的位置 */
    while((NULL != cur) && (h->keycmp(h, key, cur->key) >= 0))
    {
        if (h->keycmp(h, key, cur->key) == 0)
        {   
            /* 找到了hash    key 对应的value */
            return cur->data;
        }
        cur = cur->next;
    }

    return NULL;
}

/* 哈希表遍历 */
void hashtab_dump(hashtab *h)
{
    int i = 0;
    hashtab_node * cur = NULL;

    if (NULL == h)
    {
        return ;
    }

    printf("\r\n----开始--size[%d],nel[%d]------------", h->size, h->nel);
    for( i = 0; i < h->size; i ++)
    {
        printf("\r\n htables[%d]:", i);
        cur = h->htables[i];
        while((cur != NULL))
        {
            printf("key[%s], data[%s] ", cur->key, cur->data);    
            cur = cur->next;
        }
    }
    printf("\r\n----结束--size[%d],nel[%d]------------", h->size, h->nel);
}

struct test_node
{
    char key[80];
    char data[80];
};

/* 哈希算法,计算哈希值 */
unsigned int siample_hash(const char *str)
{
    register unsigned int hash = 0;
    register unsigned int seed = 131;

    while(*str)
    {
        hash = hash * seed + *str++;
    }

    return hash & (0x7FFFFFFF);
}

/* 哈希函数:由Key计算得到地址 */
int hashtab_hvalue(hashtab *h, const void *key)
{
    return (siample_hash(key) % h->size);
}

/* 哈希Key比较函数 */
int hashtab_keycmp(hashtab *h, const void *key1, const void *key2)
{
    return strcmp(key1, key2);
}

void hashtab_node_free(hashtab_node *node)
{
    struct test_node * ptmp = NULL;

    ptmp = container(node->key, struct test_node, key);

    free(ptmp);
    free(node);
}

int main (int argc, char **argv)
{
    int i = 0;
    int res = 0;
    char *pres = NULL;
    hashtab *hashTable = NULL;
    hashtab_node * node = NULL;
    struct test_node *p = NULL;
    
    #ifdef MEMORY_TEST
        /* 设置环境变量 MALLOC_TRACE 的值为1.txt */
        setenv("MALLOC_TRACE", "1.txt", 1);
        /* mtrace为内存分配函数（malloc, realloc, memalign, free）安装hook函数 */
        mtrace();
    #endif

    /* 创建Hash Table */
    hashTable = hashtab_create(5, hashtab_hvalue, hashtab_keycmp, hashtab_node_free);
    assert(NULL != hashTable);
    while(1)
    {
        p = (struct test_node*)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 和value，当可以等于\"quit\"时退出");
        scanf("%s", p->key);
        scanf("%s", p->data);

        if(0 == strcmp(p->key, "quit"))
        {
            free(p);
            break;
        }

        res = hashtab_insert(hashTable, p->key, p->data);
        if (res != 0)
        {
            free(p);
            printf("\r\n key[%s],data[%s] insert failed %d", p->key, p->data, res);
        }
        else
        {
            printf("\r\n key[%s], data[%s] insert success %d", p->key, p->data, res);
        }
    }

    hashtab_dump(hashTable);

    while(1)
    {
        p = (struct test_node*)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 查询value的数值，当可以等于\"quit\"时退出");
        scanf("%s",p->key);

        if(strcmp(p->key, "quit") == 0)
        {
            free(p);
            break;
        }
        
        pres = hashtab_search(hashTable, p->key);
        if (NULL == pres)
        {
            printf("\r\n key[%s] search data failed",p->key);
        }
        else
        {
            printf("\r\n key[%s],search data[%s] success",p->key,pres);
        }
        free(p);
    }
    hashtab_dump(hashTable);
    
    while(1)
    {
        p = (struct test_node*)malloc(sizeof(struct test_node));
        assert(p != NULL);
        printf("\r\n 请输入key 删除节点的数值，当可以等于\"quit\"时退出");
        scanf("%s",p->key);

        if (strcmp(p->key, "quit") == 0)
        {
            free(p);
            break;
        }
        node = hashtab_delete(hashTable, p->key);
        if (node == NULL)
        {
            printf("\r\n key[%s] delete node failed ",p->key);
        }
        else
        {
            printf("\r\n key[%s],delete data[%s] success",node->key,node->data);
            hashTable->hash_node_free(node);
        }
        free(p);
        hashtab_dump(hashTable);
    }

    hashtab_destory(hashTable);
    #ifdef MEMORY_TEST
    muntrace();
    #endif
    return 0;

}
