#!/bin/bash

# === 配置部分 ===
SERVER_IP="192.168.1.62"       # 替换为服务器的 IP 地址
TEST_TIME=3600                # 测试时间，单位为秒
LOG_FILE="iperf3_test.log"    # 日志文件路径
PARALLEL_STREAMS=4            # 并行流数量
TCP_WINDOW="256K"             # TCP 窗口大小

# === 环境优化 ===
echo "优化网络参数..."
sudo sysctl -w net.core.rmem_max=26214400
sudo sysctl -w net.core.wmem_max=26214400
sudo sysctl -w net.ipv4.tcp_rmem="4096 87380 26214400"
sudo sysctl -w net.ipv4.tcp_wmem="4096 65536 26214400"
sudo sysctl -w net.ipv4.tcp_congestion_control=reno

# === 检查iperf3安装 ===
if ! command -v iperf3 &> /dev/null; then
    echo "iperf3 未安装，正在安装..."
    sudo apt-get update && sudo apt-get install -y iperf3
fi

# === 启动iperf3测试 ===
echo "启动 iperf3 测试，日志保存到 ${LOG_FILE}..."
nohup iperf3 -c "$SERVER_IP" -t "$TEST_TIME" -i 10 -P "$PARALLEL_STREAMS" -w "$TCP_WINDOW" > "$LOG_FILE" 2>&1 &

# 获取iperf3的PID
IPERF_PID=$!
echo "iperf3 测试已启动，进程ID: $IPERF_PID"
echo "测试时长: $TEST_TIME 秒，结果将在 $LOG_FILE 中查看。"

# === 显示当前网络接口的带宽使用情况 ===
echo "运行 iftop 查看实时网络使用情况（Ctrl+C退出）"
if command -v iftop &> /dev/null; then
    sudo iftop -i "$(ip route get $SERVER_IP | awk '/dev/ {print $5}')"
else
    echo "iftop 未安装。可以使用以下命令安装:"
    echo "  sudo apt-get install -y iftop"
fi

# === 等待测试完成 ===
wait $IPERF_PID
echo "iperf3 测试完成。日志已保存到 $LOG_FILE。"
