#!/bin/bash

# 定义程序路径和启动命令
APP_NAME="QTracker_booster"
APP_PATH="/home/orangepi/hdd/main/CAMERACONTROL"
START_COMMAND="sudo ./$APP_NAME"

# 检查是否有正在运行的 QTracker_booster 进程
PID=$(pgrep -f $APP_NAME)

if [ -n "$PID" ]; then
    echo "$APP_NAME is running with PID $PID. Restarting..."
    # 终止运行中的程序
    sudo kill -9 $PID
    sleep 1
else
    echo "$APP_NAME is not running. Starting..."
fi

# 进入程序目录并启动程序
cd $APP_PATH || { echo "Failed to navigate to $APP_PATH. Exiting."; exit 1; }
echo 'orangepi' | $START_COMMAND
if [ $? -eq 0 ]; then
    echo "$APP_NAME started successfully."
else
    echo "Failed to start $APP_NAME."
    exit 1
fi
