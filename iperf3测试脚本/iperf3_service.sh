#!/bin/bash

# === 配置部分 ===
PORT=5201                      # 默认端口
LOG_FILE="iperf3_server.log"   # 日志文件

# === 检查iperf3安装 ===
if ! command -v iperf3 &> /dev/null; then
    echo "iperf3 未安装，正在安装..."
    sudo apt-get update && sudo apt-get install -y iperf3
fi

# === 启动iperf3服务器模式 ===
echo "启动 iperf3 服务器，监听端口: $PORT，日志保存到 $LOG_FILE"
nohup iperf3 -s -p "$PORT" > "$LOG_FILE" 2>&1 &

# 获取iperf3的PID
IPERF_PID=$!
echo "iperf3 服务器已启动，进程ID: $IPERF_PID"
echo "运行以下命令检查日志:"
echo "  tail -f $LOG_FILE"
