# ================ 变量定义 ================
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -I./include
LDFLAGS = -lmicrohttpd -lnetfilter_queue -lmnl -ljansson -lsqlite3 -lpthread

TARGET = linux-firewall
SRC_DIR = src

# 依赖检查命令
DEPS = libmicrohttpd-dev libnetfilter-queue-dev libmnl-dev libjansson-dev libsqlite3-dev

# ================ 源文件 ================
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:.c=.o)

# ================ 伪目标 ================
.PHONY: all clean run install-deps check-deps setup dirs

# ================ 目标说明 ================
# all       : 默认编译目标
# install-deps : 安装编译依赖
# check-deps   : 检查依赖是否满足
# setup     : 一键安装依赖并编译
# clean     : 清理构建文件
# run       : 运行程序（需root权限）

# ================ 默认目标 ================
all: dirs $(TARGET)
	@echo "Build complete: $(TARGET)"
	@echo "Run './$(TARGET) --help' for usage"

# ================ 编译规则 ================
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ================ 依赖管理 ================
# 检查依赖是否满足
check-deps:
	@echo "Checking build dependencies..."
	@for lib in $(DEPS); do \
		pkg_name=$$(echo $$lib | sed 's/-dev$$//' | sed 's/lib//'); \
		if dpkg -l $$lib >/dev/null 2>&1; then \
			echo "  [OK] $$lib"; \
		else \
			echo "  [MISSING] $$lib"; \
			MISSING=1; \
		fi; \
	done; \
	if [ "$$MISSING" = "1" ]; then \
		echo ""; \
		echo "Some dependencies are missing."; \
		echo "Run 'make install-deps' to install them."; \
		exit 1; \
	fi
	@echo "All dependencies satisfied."

# 安装编译依赖
install-deps:
	@echo "Installing build dependencies..."
	@sudo apt-get update
	@sudo apt-get install -y $(DEPS)
	@echo "Dependencies installed successfully."

# 一键安装+编译
setup: install-deps all

# ================ 清理 ================
clean:
	rm -f $(OBJECTS) $(TARGET)
	@find $(SRC_DIR) -name '*.o' -delete 2>/dev/null || true
	@echo "Clean complete"

# ================ 运行 ================
run: $(TARGET)
	@echo "Starting firewall (requires root)..."
	@sudo ./$(TARGET) --config config/firewall.json

# ================ 创建目录 ================
dirs:
	@mkdir -p $(SRC_DIR)/core
	@mkdir -p $(SRC_DIR)/backend
	@mkdir -p $(SRC_DIR)/database
	@mkdir -p $(SRC_DIR)/http/handlers
	@mkdir -p $(SRC_DIR)/utils
	@mkdir -p web/css
	@mkdir -p web/js/components
	@mkdir -p config

# ================  帮助 ================
help:
	@echo "Available targets:"
	@echo "  make all          - Build the firewall binary"
	@echo "  make setup        - Install dependencies and build"
	@echo "  make install-deps - Install build dependencies only"
	@echo "  make check-deps   - Check if dependencies are installed"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make run          - Run the firewall (requires root)"
	@echo "  make help         - Show this help message"
