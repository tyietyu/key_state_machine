#ifndef _ESP8266_H_
#define _ESP8266_H_

#include  "main.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

// 创建返回枚举类型
typedef enum
{
    ESP8266_EOK=0,
    ESP8266_ERROR,
    ESP8266_TIMEOUT,
    ESP8266_BUSY,
    ESP8266_EINVAL,
} esp8266_state_t;

// Wi-Fi 连接信息结构体
typedef struct {
    const char *ssid;             // Wi-Fi 名称
    const char *password;         // Wi-Fi 密码
} wifi_config_t;

// ESP8266 设备信息结构体
typedef struct {
    const char *product_key;      // 产品 Key
    const char *device_name;      // 设备名称
    const char *device_secret;    // 设备密钥
} esp8266_device_info_t;

// ESP8266 MQTT 连接信息结构体
typedef struct {
    const char *username;         // MQTT 用户名
    const char *client_id;        // MQTT 客户端 ID
    const char *password;         // MQTT 密码
    const char *broker_address;   // MQTT 代理地址
} mqtt_config_t;

// 订阅和发布主题及消息格式结构体
typedef struct {
    const char *sub_topic;        // 订阅主题
    const char *pub_topic;        // 发布主题
    const char *json_format;      // JSON 消息格式
    const char *json_format_firmware; // JSON 固件版本格式
    const char *device_attributes; // 设备属性上报
} mqtt_topics_t;

// OTA 相关信息结构体
typedef struct {
    const char *upload_info_pub;               // 上传信息
    const char *download_info_sub;            // 下载信息
    const char *device_active_info_pub;       // 设备主动拉取信息
    const char *device_report_progress_pub;    // 设备上报进度
    const char *device_download_file;            // 下载文件
    const char *device_download_file_reply;   // 下载文件回复
} ota_info_t;

// 综合配置结构体
typedef struct {
    wifi_config_t wifi;                     // Wi-Fi 信息
    esp8266_device_info_t device_info;      // 设备信息
    mqtt_config_t mqtt;                     // MQTT 信息
    mqtt_topics_t mqtt_topics;              // MQTT 主题与消息格式
    ota_info_t ota;                         // OTA 信息
} esp8266_config_t;

// 初始化配置
esp8266_config_t esp8266_config = {
    .wifi = {
        .ssid = "XiaomiPro",
        .password = "123456789l"
    },
    .device_info = {
        .product_key = "k1644sbngGw",
        .device_name = "AT_MQTT",
        .device_secret = "1038f2eead281b6e90427a69d9cd532b"
    },
    .mqtt = {
        .username = "AT_MQTT&k1644sbngGw",
        .client_id = "k1644sbngGw.AT_MQTT|securemode=2\\,signmethod=hmacsha256\\,timestamp=1722604000001|",
        .password = "35d93f05a230fb364ac51438ed45f67c42d0c0fd4a2f792298297d106afdbad3",
        .broker_address = "k1644sbngGw.iot-as-mqtt.cn-shanghai.aliyuncs.com"
    },
    .mqtt_topics = {
        .sub_topic = "/k1644sbngGw/AT_MQTT/user/get",
        .pub_topic = "/sys/k1644sbngGw/AT_MQTT/thing/event/property/post",
        .json_format = "{\"params\":{\"esp8266_adc_data\":%d,\"LED\":%d},\"version\":\"1.0.0\"}",
        .json_format_firmware = "{\"id\":\"0011\",\"params\":{\"version\":\"1.0.0\"}}",
        .device_attributes = "/sys/k1644sbngGw/AT_MQTT/thing/service/property/set"
    },
    .ota = {
        .upload_info_pub = "/ota/device/inform/k1644sbngGw/AT_MQTT",
        .download_info_sub = "/ota/device/upgrade/k1644sbngGw/AT_MQTT",
        .device_active_info_pub = "/sys/k1644sbngGw/AT_MQTT/thing/ota/firmware/get",
        .device_report_progress_pub = "/ota/device/progress/k1644sbngGw/AT_MQTT",
        .device_download_file = "/sys/k1644sbngGw/AT_MQTT/thing/file/download_reply",
        .device_download_file_reply = "/sys/k1644sbngGw/AT_MQTT/thing/file/download"
    }
};

typedef struct
{
    unsigned char receive_buff[512];
    unsigned char send_buff[512];
    uint8_t receive_start;
    uint16_t receive_count;
    uint16_t receive_finish;
} esp8266_buffer_t;

typedef struct
{
    uint8_t (*init)(void);
    uint8_t (*esp8266_sw_reset)(void);
    uint8_t (*esp8266_restore)(void);
    uint8_t (*connect_wifi)(const char *wifi_ssid, const char *password);
    uint8_t (*login_to_cloud)(const char *mqtt_name, const char *mqtt_password, const char *mqtt_client_id, const char *broker_address);

    esp8266_config_t esp8266_config;
} esp8266_init_t;

typedef struct
{
    uint8_t *uart_port;
    void (*delay_ms)(uint32_t ms);
    void (*uart_send)(uint8_t *data, size_t length);
    void (*uart_receive)(uint8_t *data, size_t length);
    void (*uart_callback)(void);

    esp8266_buffer_t esp8266_buffer;
} uart_init_t;

uint8_t ESP8266_connect_wifi(const char *wifi_ssid, const char *password);
uint8_t ESP8266_connect_to_cloud(const char *mqtt_name, const char *mqtt_password, const char *mqtt_client_id, const char *broker_address);
uint8_t esp8266_init(uint8_t esp8266_mode,uint8_t esp8266_cfg);
uint8_t ESP8266_send_msg(const char *topic, const char *msg_format, ...);
uint8_t esp8266_receive_msg(const char *topic, uint8_t *msg_data, uint16_t msg_len);


#endif