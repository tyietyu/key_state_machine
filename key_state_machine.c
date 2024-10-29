#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

enum keyboard_state
{
    KEYBOARD_STATE_IDLE = 0,
    KEYBOARD_STATE_PRESSED,
};

struct key_time_i;
struct key_i;

typedef size_t (*key_time_get_time_t)(struct key_time_i **self);

struct key_time_i
{
    key_time_get_time_t get_time;
};

typedef int (*key_is_pressed_t)(struct key_i **self);

struct key_i
{
    key_is_pressed_t is_pressed; // 按键是否按下
};

struct my_key_time
{
    struct key_time_i *interface; // 按键时间接口
};

struct my_key
{
    struct key_i *interface; // 按键接口
};

size_t my_key_time_get_time(struct my_key_time *self)
{
    //外部接口时间函数，获取当前时间
    static size_t fake_time = 1024;
    return fake_time += 10;
}

static inline size_t key_time_get_time(struct key_time_i **self)
{
    return(*self)->get_time(self);
}

static inline int key_is_pressed(struct key_i **self)
{
    return(*self)->is_pressed(self); // 按键是否按下

}

struct  key_time_i my_key_time_interface={
    .get_time=(key_time_get_time_t)my_key_time_get_time, // 获取当前时间
};

int my_key_time_init(struct my_key_time *self)
{
    self->interface = &my_key_time_interface; // 初始化按键时间接口
    return 0;
}

int my_key_is_pressed(struct my_key *self)
{
    static int count = 0;
    if(count++ < 10)
    {
        return 0;
    }
    else if (count++ < 20)
    {
        return 1;
    }
    else
    {
        count = 0;
        return 0;
    }
}

struct key_i my_key_interface={
    .is_pressed=(key_is_pressed_t)my_key_is_pressed, // 按键是否按下
};

int my_key_init(struct my_key *self)
{
    self->interface = &my_key_interface; // 初始化按键接口
    return 0;
}


typedef int (*key_state_machine_on)(int *key, size_t pressed_time);

struct keyboard_state_machine
{
    enum keyboard_state state;
    struct key_time_i **time;
    struct key_i **key;
    size_t press_time;
    size_t release_time;
    key_state_machine_on on_event; // 按键状态机回调函数

};

int keyStatePressed(struct keyboard_state_machine *self)
{
    size_t current_time;
    // 按键防抖
    if (!key_is_pressed(self->key))
    {
        self->state = KEYBOARD_STATE_IDLE;
        current_time = key_time_get_time(self->time);
        int key_value=0; // 获取按键值
        self->on_event(&key_value, current_time - self->press_time);
    }
    return 0;
}

int keyStateIdle(struct keyboard_state_machine *self)
{
    // 按键防抖
    if (key_is_pressed(self->key))
    {
        self->state = KEYBOARD_STATE_PRESSED;
        self->press_time = key_time_get_time(self->time);
    }
    return 0;
}

int keyStateStep(struct keyboard_state_machine *self)
{
    printf("\t>>>>>>current state:%d (press_time:%lu)\n", self->state, self->press_time);
    switch (self->state)
    {
    case KEYBOARD_STATE_IDLE:
        return keyStateIdle(self);
    case KEYBOARD_STATE_PRESSED:
        return keyStatePressed(self);
    }
    return 0;
}

int keyboard_state_machine_init(struct keyboard_state_machine *self, key_state_machine_on on_event,
                                struct key_i **key, struct key_time_i **key_time)
{
    self->state = KEYBOARD_STATE_IDLE;
    self->press_time = 0;
    self->key = key;
    self->time = key_time;
    self->on_event = on_event;
    return 0;
}

int my_on_event(int *key, size_t pressed_time)
{
    printf("key:%d,pressed_time:%lu\n", *key, pressed_time);
    return 0;
}

int main()
{
    struct keyboard_state_machine keyboard;
    struct my_key_time key_time;
    struct my_key key;

    assert(! my_key_time_init(&key_time));
    assert(! my_key_init(&key));
    assert(! keyboard_state_machine_init(&keyboard,my_on_event, (struct key_i **)&key, (struct key_time_i **)&key_time));

    int i = 0;
    for (i = 0; i < 100; i++)
    {
       assert(! keyStateStep(&keyboard));
    }
    return 0;
}

