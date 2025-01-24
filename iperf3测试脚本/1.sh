o "==== 内存占用高的进程 ===="
ps aux --sort=-%mem | head -n 10

# 检测系统温度
echo "==== 系统温度 ===="
sensors

# 监控网络流量下降
interface="eth0"
prev_rx_bytes=$(cat /sys/class/net/$interface/statistics/rx_bytes)
prev_tx_bytes=$(cat /sys/class/net/$interface/statistics/tx_bytes)

sleep 1

curr_rx_bytes=$(cat /sys/class/net/$interface/statistics/rx_bytes)
curr_tx_bytes=$(cat /sys/class/net/$interface/statistics/tx_bytes)

rx_diff=$((curr_rx_bytes - prev_rx_bytes))
tx_diff=$((curr_tx_bytes - prev_tx_bytes))

if [ $rx_diff -lt 1000 ] && [ $tx_diff -lt 1000 ]; then
	  echo "警告：网络流量下降，接收流量：$rx_diff 字节，发送流量：$tx_diff 字节"
  else
	    echo "网络流量正常，接收流量：$rx_diff 字节，发送流量：$tx_diff 字节"
fi

