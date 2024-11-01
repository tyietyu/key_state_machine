# key_state_machine
按键驱动的状态机demo
# 对应硬件接口部分
struct my_key_time
{
    struct key_time_i *interface; // 按键时间接口
};

struct my_key
{
    struct key_i *interface; // 按键接口
};
在这两个结构体中添加对象接口

# C语言实现静态数组链表功能，通过更改下面的宏定义更改链表的大小以及数组大小
# 没有使用动态内存分配，解决了在嵌入式中内存碎片的问题

#define MAX_LIST_SIZE 50 // 定义链表的最大节点数
#define ARRAY_SIZE 20 // 定义数组的大小
