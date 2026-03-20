#ifndef FIREWALL_BACKEND_H
#define FIREWALL_BACKEND_H

#include "firewall.h"

/* 后端能力标志 */
#define BACKEND_CAP_REALTIME    0x01 // 实时包处理
#define BACKEND_CAP_STATS       0x02 // 详细统计
#define BACKEND_CAP_PACKET_LOG  0x03 // 包日志

/* 后端接口结构(策略模式) */
typedef struct firewall_backend{
  const char *name;   // 后端名称
  int capabilities;   // 能力标志

  // 生命周期
  int (*init)(struct firewall_backend *self);
  int (*destroy)(struct firewall_backend *self);

  // 规则管理
  int (*add_rule)(struct firewall_backend *self, firewall_rule_t *rule);
  int (*remove_rule)(struct firewall_backend *self, int rule_id);
  int (*update_rule)(struct firewall_backend *self, int rule_id, firewall_rule_t *rule);
  int (*list_rules)(struct firewall_backend *self, firewall_rule_t **rules, int *count);

  // 运行控制
  int (*start)(struct firewall_backend *self);
  int (*stop)(struct firewall_backend *self);

  // 统计
  int (*get_stats)(struct firewall_backend *self, firewall_stats_t *stats);

  // 私有数据
  void *priv;
} firewall_backend_t;

/* 后端工厂函数 */
firewall_backend_t* backend_create_nfqueue(int queue_num);
firewall_backend_t* backend_create_iptables(void);
void backend_destroy(firewall_backend_t *backend);

#endif
