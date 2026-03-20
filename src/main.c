#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <firewall.h>

/* ================ 全局变量 ================ */
static volatile int g_runninng = 1;

/* ================ 信号处理 ================ */
static void signal_handler(int sig){
  if (sig == SIGINT || sig == SIGTERM) {
    printf("\nReceived signal %d, shutting down...\n", sig);
    g_running = 0;
  }
}

/* ================ 帮助信息 ================ */
static void print_usage(const char *prog) {
  printf("Usage: %s [OPTIONS]\n", prog);
  printf("\nOptions:\n");
  printf("  -c, --config <file> Configuration file (default: config/firewall.json)\n");
  printf("  -p, --port <port>     HTTP server port (default: 12345)\n");
  printf("  -m, --mode <mode>     Firewall mode: auto, nfqueue, iptables (default: auto)\n");
  printf("  -h, --help            Show this help message\n");
  printf("\n");
}

/* ================ 主程序  ================ */
int main(int argc, char *argv[]) {
  firewall_config_t config = {0};

  /* 默认配置 */
  config.port = 12345;
  strcpy(config.bind_addr, "0.0.0.0");
  config.mode = MODE_AUTO;
  config.queue.number = 0;
  config.auto_load_modules = 1;
  strcpy(config.db_path, "./firewall.db");
  config.log_retention_days = 30;
  strcpy(config.log_level, "info");
  strcpy(config.log_file, "./firewall.log");

  /* 解析命令行参数 */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
      if (i + 1 < argc) {
        /* 加载配置文件 */
        strncpy(config.db_path, argv[++i], sizeof(config.db_path) - 1);
      }
    }
    else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
      if (i + 1 < argc) {
        config.port = atoi(argv[++i]);
      }
    }
    else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mode") == 0) {
      if (i + 1 < argc) {
        i++;
        if (strcmp(argv[i], "nfqueue") == 0) {
          config.mode = MODE_NFQUEUE;
        }
        else if (strcmp(argv[i], "iptables") == 0) {
          config.mode = MODE_IPTABLES;
        }
        else {
          config.mode = MODE_AUTO;
        }
      }
    }
    else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    }
  }

  /* 设置信号处理 */
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  printf("===========================================\n");
  printf("       Firewall Manager v1.0\n");
  printf("===========================================\n");
  printf("Server Port: %d\n", config.port);
  printf("Bind Address: %s\n", config.bind_addr);
  printf("Mode: %s\n", config.mode == MODE_AUTO ? "auto" : config.mode == MODE_NFQUEUE ? "nfqueue" : "iptables");
  printf("Database: %s\n", config.db_path);
  printf("===========================================\n\n");

  /* 初始化防火墙 */
  if (firewall_init(&config) != 0) {
    fprintf(stderr, "Failed to initialize firewall\n");
    return 1;
  }

  /* 启动防火墙 */
  if (firewall_start() != 0){
    fprintf(stderr, "Failed to start firewall\n");
    firewall_destory();
    return 1;
  }

  printf("Firewall started successfully!\n")
  printf("Web interface: http://localhost:%d\n", config.port);
  printf("Press Ctrl+C to stop\n\n");

  /* 主循环 */
  while(g_running) {
    sleep(1);
  }

  /* 清理退出 */
  printf("Stopping firewall...\n");
  firewall_stop();
  firewall_destroy();
  
  printf("Firewall has sccessfully stopped.\n");
  return 0;
}
