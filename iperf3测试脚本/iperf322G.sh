#!/bin/bash

# 设置目标服务器IP和测试持续时间
TARGET_IP="192.168.1.63"
TEST_DURATION=36000  # 10小时（36000秒）

# 定义输出文件
OUTPUT_FILE="iperf_output.txt"

# 运行iperf3测试并保存输出到文件
iperf3 -c $TARGET_IP -t $TEST_DURATION > $OUTPUT_FILE

# 过滤带宽低于2.2 Gbps的结果并输出
echo "带宽低于 2.2 Gbps 的测试结果:"
grep -E 'bitrate|Mbits|Gbits' $OUTPUT_FILE | awk '
{
    # 处理带宽低于2.2 Gbps（2200 Mbps）的行
    if ($0 ~ /Gbits/ && $7 < 2.2) {
        print $0
    } 
    else if ($0 ~ /Mbits/ && $7 < 2200) {
        print $0
    }
}'
