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
