import socket
import time
import numpy as np
import matplotlib.pyplot as plt
from drawnow import drawnow

## 创建一个udp接收函数

# def udp_recv(udp_socket):
#     recv_data = udp_socket.recvfrom(1024)
#     return recv_data


# udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# udp_socket.bind(("",4001))

# ## 将接受的udp数据以波形图的方式绘制出来
# def draw_wave():
# #   recv_data = udp_recv(udp_socket)
# #    print(recv_data[1][0]+": "+str(recv_data[0])[2:-3])

#     while True:
#         recv_data = udp_recv(udp_socket)
#         print(recv_data[1][0]+": "+str(recv_data[0])[2:-3])
  
  # 创建UDP套接字
UDP_IP = ""  # 服务器IP地址
UDP_PORT = 4001        # 服务器端口号

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 4321))

# 创建一个空列表存储数据
data = []

# 创建一个实时绘图函数
def plot_data():
    plt.plot(data)
    plt.xlabel('Time')
    plt.ylabel('Data')
    plt.title('Real-time Data Plot')

# 实时绘图循环
while True:
    # 从UDP套接字接收数据
    data_bytes, addr = sock.recvfrom(1)
    ## 将data_byte 转换为整数
    print(data_bytes)

    # 解码收到的数据并转换为整数
    # try:
    #     val = int(data_bytes.decode().strip())
    #     print(data_bytes)
    # except ValueError:
    #     continue
    
    # 将数据添加到列表中
    data.append(data_bytes)
    
    # 只保留最新的100个数据点
    if len(data) > 100:
        data = data[-100:]
    
    # 实时绘制数据
    plt.ion()
    drawnow(plot_data)
    plt.pause(0.0005)