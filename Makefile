# ================ 变量定义 ================
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -I./include
LDFLAGS = -lmicrohttpd -lnfq -lmnl -ljansson -lsqlite3 -lpthread

TARGET = linux-firewawll
SRC_DIR = src

# ================ 源文件 ================
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:.c=.o)

# ================ 伪目标 ================
.PHONY: all clean run install-deps dirs

# ================ 默认目标 ================
all: dirs $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# ================ 编译规则 ================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ================ 清理 ================
clean:
	rm -f $(OBJECTS) $(TARGET)
	find $(SRC_DIR) -name '*.o' -delete
	@echo "Clean complete"

# ================ 运行 ================
run: $(TARGET)
	sudo ./$(TARGET) --config config/firewall.json

# ================ 依赖安装 ================
install-deps:
	@echo "Installing dependences..."
	sudo apt-get update
	sudo apt-get install -y \
		gcc make \
		libmicrohttpd-dev \
		libnetfilter-queue-dev \
		libmnl-dev \
		libjansson-dev \
		libsqlite3-dev

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
