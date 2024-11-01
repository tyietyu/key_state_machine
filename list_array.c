#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIST_SIZE 50 // 定义链表的最大节点数
#define ARRAY_SIZE 20    // 定义数组的大小
#define WINDOW_SIZE 10   // 定义窗口大小

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
void init_List(List *list)
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
int alloc_Node(List *list)
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
void free_Node(List *list, int node)
{
    list->nodes[node].next = list->free;
    list->free = node;
    list->size--;
}

// 在链表末尾插入数据
bool insert_Array_And_Slide(List *list, float data_array[ARRAY_SIZE])
{
    int newNode = alloc_Node(list);
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

        // 如果链表长度超过窗口大小，则删除旧的节点
        if (list->size > WINDOW_SIZE)
        {
            int oldestNode = list->head;
            list->head = list->nodes[list->head].next;
            list->nodes[list->nodes[oldestNode].prev].next = list->head;
            list->nodes[list->head].prev = list->nodes[oldestNode].prev;
            free_Node(list, oldestNode);
        }
    }
}

// 删除指定值的节点
bool delete_Node(List *list, int nodeIndex)
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
            free_Node(list, current);
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
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("%.1f ", list->nodes[current].data[i]);
        }
        printf("\n");
        current = list->nodes[current].next;
    } while (current != list->head);
}

void print_windows(List *list)
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
        printf("帧 %d: ", nodeIndex++);
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("%.2f ", list->nodes[current].data[i]);
        }
        printf("\n");
        current = list->nodes[current].next;
    } while (current != list->head && nodeIndex < WINDOW_SIZE);
}

int main()
{
    List list;
    init_List(&list);

    // 模拟15帧数据
    for (int frame = 0; frame < 15; frame++)
    {
        float datafft[ARRAY_SIZE];
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            datafft[i] = frame + i * 0.1f; // 生成示例数据
        }

        insert_Array_And_Slide(&list, datafft);

        printf("插入帧 %d 后的滑动窗口:\n", frame);
        print_windows(&list);
        printf("\n");
    }

    return 0;
}
