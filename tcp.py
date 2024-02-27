import socket
import numpy as np
import matplotlib.pyplot as plt
from drawnow import drawnow


# 1 创建客户端套接字对象tcp_client_1
# 参数介绍：AF_INET 代表IPV4类型, SOCK_STREAM代表tcp传输协议类型 ,注：AF_INET6代表IPV6

tcp_client_1 = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# 2 通过客户端套接字的connect方法与服务器套接字建立连接  
# 参数介绍：前面的ip地址代表服务器的ip地址，后面的61234代表服务端的端口号 。

tcp_client_1.connect(("192.168.0.111",4321))

# 将编号好的数据存到变量send_data中，注：encode(encoding='utf-8)是将数据转换成utf-8的格式发送给服务器
##send_data = "你好，服务器，我是客户端1号".encode(encoding='utf-8')
    
# 3 通过客户端套接字的send方法将数据发送给服务器
##tcp_client_1.send(send_data)
data = []

# 创建一个实时绘图函数
def plot_data():
    plt.plot(data)
    plt.xlabel('Time')
    plt.ylabel('Data')
    plt.title('Real-time Data Plot')
    # 4 通过客户端套接字的recv方法来接受服务器返回的数据存到变量recv_data中，1024是可接收的最大字节数。
    # while(1):
    #     recv_data = tcp_client_1.recv(1024)
    
    # # 将接收到的服务器数据recv_data通过decode方法解码为utf-8
    #     print(recv_data.decode(encoding = 'utf-8'))

    # # 5 最后关闭客户端套接字连接
   

# 实时绘图循环
while True:
    # 从UDP套接字接收数据
    recv_data = tcp_client_1.recv(1)
    ## 分割数据
    ##recv_data = recv_data.split(b",")
    ## 将data_byte 转换为整数
    print(recv_data)

    # 解码收到的数据并转换为整数
    # try:
    #     val = int(data_bytes.decode().strip())
    #     print(data_bytes)
    # except ValueError:
    #     continue
    
    # 将数据添加到列表中
    data.append(recv_data)
    
    # 只保留最新的100个数据点
    if len(data) > 50:
        data = data[-50:]
    
    # 实时绘制数据
    plt.ion()
    drawnow(plot_data)
    plt.pause(0.00001)
tcp_client_1.close()