#ifndef FIREWALL_H
#define FIREWALL_H

#include <stdint.h>
#include <time.h>

/***
枚举类型
***/

// 防火墙运行模式
typedef enum{
  MODE_AUTO = 0,    // 自动选择
  MODE_NFQUEUE,   // NFQueue模式(主方案)
  MODE_IPTABLES,    // iptables命令模式(降级方案)
  MODE_DISABLED   // 禁用
} firewall_mode_t;

// 规则动作
typedef enum{
  ACTION_ACCEPT = 0,    // 接受
  ACTION_DROP = 1,   // 丢弃(无响应)
  ACTION_REJECT = 2   //拒绝(返回错误)
} rule_action_t;

// 协议类型
typedef enum{
  PROTOCOL_ANY = 0,    // 任意协议
  PROTOCOL_TCP = 6,   // TCP协议
  PROTOCOL_UDP = 17,    // UDP协议
  PROTOCOL_ICMP =1    //ICMP协议
} protocol_t;

// 流量方向
typedef enum{
  DIRECTION_IN = 0,   // 入站
  DIRECTION_OUT = 1,    // 出站
  DIRECTION_ANY = 2   // 双向
} direction_t;

/***
数据结构
***/

// 防火墙规则
typedef struct{
  int id;   // 数据库ID
  char name[64];    // 规则名称
  int protocol;   // 协议类型
  char src_ip[46];    // 源IP(支持IPv6)
  char dst_ip[46];    // 目的IP
  int src_port;   // 源端口
  int dst_port;   // 目的端口
  int direction;  // 流量方向
  int action;   // 处理动作
  int priority;   // 优先级
  int enabled;    // 是否启用
  char comment[256];    // 备注
  time_t created_at;    // 创建时间
  time_t updated_at;    // 更新时间
} firewall_rule_t;

// 统计信息
typedef struct{
  uint64_t packets_total;   // 总包数
  uint64_t packets_accepted;    // 接受的包
  uint64_t packets_dropped;   // 丢弃的包
  uint64_t packets_rejected;    // 拒绝的包
  uint64_t bytes_total;   // 总字节数
  time_t uptime_start;    // 启动的时间
  time_t last_updated;    // 最后更新时间
} firewall_stats_t;

// 日志条目
typedef struct{
  int id;
  int level;    // 日志级别
  int type;   // 日志类型
  char src_ip[46];
  char dst_ip[46];
  int src_port;
  int dst_port;
  int protocol;
  int packet_len;
  int matched_rule_id;
  int action;
  time_t timestamp;
  char message[512];
} firewall_log_entry_t;

typedef struct{
  int port;   // HTTP服务端口
  char bind_addr[46];   // 绑定地址
  firewall_mode_t mode;   // 运行模式
  int queue_number;   // NFQueue队列号
  int auto_load_modules;    // 自动加载内核模块
  char db_path[256];    // 数据库路径
  int log_retention_days;   // 日志保留天数
  char log_level[16];   // 日志级别
  char log_file[256];   // 日志文件
} firewall_config_t;

/***
API函数声明
***/

// 生命周期
int firewall_init(firewall_config_t *config);
int firewall_destroy(void);
int firewall_start(void);
int firewall_stop(void);
firewall_mode_t firewall_get_mode(void);
const char* firewall_get_mode_name(void);

// 规则管理
int firewall_add_rule(firewall_rule_t *rule);
int firewall_remove_rule(int rule_id);
int firewall_update_rule(int rule_id, firewall_rule_t *rule);
int firewall_list_rules(firewall_rule_t **rules, int *count);
int firewall_get_rule(int rule_id, firewall_rule_t *rule);

// 统计
int firewall_get_stats(firewall_stats_t *stats);
int firewall_reset_stats(void);

// 日志
int firewall_get_logs(firewall_log_entry_t **logs, int *count, int offset, int limit);
int firewall_clear_logs(void);

#endif /* FIREWALL_H */
