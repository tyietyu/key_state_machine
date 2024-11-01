#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIST_SIZE 50 // 定义链表的最大节点数
#define ARRAY_SIZE 20 // 定义数组的大小

// 定义链表节点结构
typedef struct
{
    float data[ARRAY_SIZE];
    int prev;
    int next;
} Node;

// 定义链表结构
typedef struct
{
    Node nodes[MAX_LIST_SIZE];
    int head;
    int free;
    int size;
} List;

// 初始化链表
void initList(List *list)
{
    list->head = -1;
    list->free = 0;
    list->size = 0;

    // 初始化空闲链表
    for (int i = 0; i < MAX_LIST_SIZE - 1; i++)
    {
        list->nodes[i].next = i + 1;
    }
    list->nodes[MAX_LIST_SIZE - 1].next = -1;
}

// 分配新节点
int allocNode(List *list)
{
    if (list->free == -1)
    {
        printf("链表已满\n");
        return -1;
    }
    int newNode = list->free;
    list->free = list->nodes[list->free].next;
    list->size++;
    return newNode;
}

// 释放节点
void freeNode(List *list, int node)
{
    list->nodes[node].next = list->free;
    list->free = node;
    list->size--;
}

// 在链表末尾插入数据
bool insertArrayAtEnd(List *list, float data_array[ARRAY_SIZE])
{
    int newNode = allocNode(list);
    if (newNode == -1)
        return false;

    memcpy(list->nodes[newNode].data, data_array, sizeof(float) * ARRAY_SIZE);

    if (list->head == -1)
    {
        // 链表为空
        list->head = newNode;
        list->nodes[newNode].prev = newNode;
        list->nodes[newNode].next = newNode;
    }
    else
    {
        // 链表不为空
        int last = list->nodes[list->head].prev;
        list->nodes[newNode].prev = last;
        list->nodes[newNode].next = list->head;
        list->nodes[last].next = newNode;
        list->nodes[list->head].prev = newNode;
    }

    return true;
}

// 删除指定值的节点
bool deleteNode(List *list, int nodeIndex)
{
    if (list->head == -1 || nodeIndex < 0 || nodeIndex >= MAX_LIST_SIZE)
        return false;

    int current = list->head;
    int count = 0;
    do
    {
        if (count == nodeIndex)
        {
            if (list->size == 1)
            {
                // 只有一个节点
                list->head = -1;
            }
            else
            {
                // 多于一个节点
                int prev = list->nodes[current].prev;
                int next = list->nodes[current].next;
                list->nodes[prev].next = next;
                list->nodes[next].prev = prev;
                if (current == list->head)
                {
                    list->head = next;
                }
            }
            freeNode(list, current);
            return true;
        }
        current = list->nodes[current].next;
        count++;
    } while (current != list->head);

    return false;
}

// 打印链表
void printList(List *list)
{
    if (list->head == -1)
    {
        printf("链表为空\n");
        return;
    }

    int current = list->head;
    int nodeIndex = 0;
    do
    {
        printf("节点 %d: \n", nodeIndex);
       for(int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("%.1f ", list->nodes[current].data[i]);
        }
        printf("\n");
        current = list->nodes[current].next;
    } while (current != list->head);
}

int main()
{
    List lists;
    initList(&lists);

     // 创建一些测试数据
    float datafft1[ARRAY_SIZE] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0,
                                  11.1, 12.2, 13.3, 14.4, 15.5, 16.6, 17.7, 18.8, 19.9, 20.0};
    float datafft2[ARRAY_SIZE] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
                                  1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};

    // 插入数组数据
    insertArrayAtEnd(&lists, datafft1);
    insertArrayAtEnd(&lists, datafft2);

    printf("原始链表: ");
    printList(&lists);

    // 删除一个节点
    deleteNode(&lists, 0);
    printf("删除第一个节点后: \n");
    printList(&lists);

 // 再次插入数据
    insertArrayAtEnd(&lists, datafft1);
    printf("\n再次插入数据后:\n");
    printList(&lists);

    return 0;
}
