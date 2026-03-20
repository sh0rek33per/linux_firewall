#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firewall.h"

/* 实现防火墙核心逻辑 */

int firewall_init(firewall_config_t *config) {
  printf("[CORE] Initializing firewall...\n");
  printf("[CORE] Mode: %d, Port: %d\n",config->mode, config->port);
  return 0;
}

int firewall_destroy(void) {
  printf("[CORE] Destroying firewall...\n");
  return 0;
}

int firewall_start(void) {
  printf("[CORE] Starting firewall...\n");
  return 0;
}

int firewall_stop(void) {
  printf("[CORE] Stopping firewall...\n");
  return 0;
}

firewall_mode_t firewall_get_mode(void) {
  return MODE_NFQUEUE;
}

const char* firewall_get_mode_name(void) {
  return "nfqueue";
}

/* 规则管理 */
int firewall_add_rule(firewall_rule_t *rule) {
  (void)rule;
  return 0;
}

int firewall_remove_rule(int rule_id){
  (void)rule_id;
  return 0;
}

int firewall_update_rule(int rule_id, firewall_rule_t *rule) {
  (void)rule_id;
  (void)rule;
  return 0;
}

int firewall_list_rules(firewall_rule_t **rules, int *count) {
  *rules = NULL;
  *count = 0;
  return 0;
}

int firewall_get_rule(int rule_id, firewall_rule_t *rule) {
  (void)rule_id;
  (void)rule;
  return -1;
}

/* 统计 */
int firewall_get_stats(firewall_stats_t *stats) {
  memset(stats, 0, sizeof(firewall_stats_t));
  return 0;
}

int firewall_reset_stats(void) {
  return 0;
}

int firewall_get_logs(firewall_log_entry_t **logs, int *count, int offset, int limit) {
  *logs = NULL;
  *count = 0;
  (void)offset;
  (void)limit;
  return 0;
}

int firewall_clear_logs(void) {
  return 0;
}
