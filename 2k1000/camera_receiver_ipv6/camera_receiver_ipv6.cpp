/*
 * 支持IPv6的PC端接收程序 - 龙芯派版本
 * 主要修改：添加IPv6支持，保持与发送端协议一致
 */

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 数据结构定义 (与发送端保持一致)
struct FrameInfo {
    uint32_t frame_id;
    uint32_t total_size;
    uint32_t packet_count;
    uint32_t width;
    uint32_t height;
    uint64_t timestamp;
};

struct PacketHeader {
    uint32_t frame_id;
    uint32_t packet_id;
    uint32_t data_size;
};

struct FrameBuffer {
    vector<uint8_t> data;
    vector<bool> received_packets;
    uint32_t total_packets;
    uint32_t received_count;
    chrono::steady_clock::time_point start_time;
    uint32_t width, height;
    bool is_complete;
    bool info_received;
};

static bool g_running = true;
static int g_socket_fd = -1;
static map<uint32_t, FrameBuffer> g_frame_buffers;
static mutex g_buffer_mutex;

void signal_handler(int sig) {
    printf("\n收到信号 %d，正在停止接收程序...\n", sig);
    g_running = false;
    
    if (g_socket_fd != -1) {
        close(g_socket_fd);
        g_socket_fd = -1;
    }
    
    destroyAllWindows();
}

class VideoReceiver {
private:
    int port;
    int frames_received;
    int frames_displayed;
    int packets_received;
    chrono::steady_clock::time_point start_time;
    bool use_ipv6;
    
public:
    VideoReceiver(int listen_port, bool ipv6 = true) : port(listen_port), frames_received(0), 
                                   frames_displayed(0), packets_received(0), use_ipv6(ipv6) {}
    
    bool start() {
        printf("===========================================\n");
        printf("      支持IPv6的龙芯派接收程序\n");
        printf("===========================================\n");
        printf("监听端口: %d\n", port);
        printf("协议版本: %s\n", use_ipv6 ? "IPv6" : "IPv4");
        printf("等待久久派连接...\n");
        printf("===========================================\n");
        
        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        
        // 创建socket - 支持IPv6
        if (use_ipv6) {
            g_socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
        } else {
            g_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        }
        
        if (g_socket_fd < 0) {
            printf("错误: 无法创建socket: %s\n", strerror(errno));
            return false;
        }
        
        // 设置socket选项
        int opt = 1;
        setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // IPv6专用选项：只监听IPv6
        if (use_ipv6) {
            int ipv6only = 1;
            if (setsockopt(g_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6only, sizeof(ipv6only)) < 0) {
                printf("警告: 无法设置IPv6专用模式: %s\n", strerror(errno));
            }
        }
        
        // 增大接收缓冲区
        int recv_buffer = 8 * 1024 * 1024; // 8MB
        setsockopt(g_socket_fd, SOL_SOCKET, SO_RCVBUF, &recv_buffer, sizeof(recv_buffer));
        
        // 绑定地址 - 根据IPv4/IPv6选择
        if (use_ipv6) {
            struct sockaddr_in6 server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin6_family = AF_INET6;
            server_addr.sin6_addr = in6addr_any;  // 监听所有IPv6地址
            server_addr.sin6_port = htons(port);
            
            if (bind(g_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                printf("错误: 无法绑定IPv6端口 %d: %s\n", port, strerror(errno));
                close(g_socket_fd);
                return false;
            }
            
            printf("✓ 成功绑定IPv6端口 %d (:::%d)\n", port, port);
        } else {
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(port);
            
            if (bind(g_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                printf("错误: 无法绑定IPv4端口 %d: %s\n", port, strerror(errno));
                close(g_socket_fd);
                return false;
            }
            
            printf("✓ 成功绑定IPv4端口 %d (0.0.0.0:%d)\n", port, port);
        }
        
        return true;
    }
    
    void receive_and_display() {
        char buffer[2048];  // 增大缓冲区
        struct sockaddr_storage client_addr;  // 支持IPv4和IPv6
        socklen_t client_len = sizeof(client_addr);
        
        namedWindow("5G摄像头实时视频", WINDOW_AUTOSIZE);
        
        start_time = chrono::steady_clock::now();
        auto last_stats = start_time;
        
        printf("\n等待视频数据包...\n");
        
        while (g_running) {
            ssize_t received = recvfrom(g_socket_fd, buffer, sizeof(buffer), 0,
                                      (struct sockaddr*)&client_addr, &client_len);
            
            if (received < 0) {
                if (g_running) {
                    printf("接收数据错误: %s\n", strerror(errno));
                }
                continue;
            }
            
            packets_received++;
            
            // 显示客户端信息 - 支持IPv4和IPv6
            if (packets_received == 1) {
                char client_ip[INET6_ADDRSTRLEN];
                int client_port = 0;
                
                if (client_addr.ss_family == AF_INET) {
                    struct sockaddr_in* addr_in = (struct sockaddr_in*)&client_addr;
                    inet_ntop(AF_INET, &addr_in->sin_addr, client_ip, INET_ADDRSTRLEN);
                    client_port = ntohs(addr_in->sin_port);
                    printf("✓ 首次连接来自IPv4: %s:%d\n", client_ip, client_port);
                } else if (client_addr.ss_family == AF_INET6) {
                    struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)&client_addr;
                    inet_ntop(AF_INET6, &addr_in6->sin6_addr, client_ip, INET6_ADDRSTRLEN);
                    client_port = ntohs(addr_in6->sin6_port);
                    printf("✓ 首次连接来自IPv6: [%s]:%d\n", client_ip, client_port);
                }
            }
            
            // 调试：显示接收到的数据包信息
            if (packets_received <= 10) {
                printf("[DEBUG] 收到 %zd 字节数据包\n", received);
            }
            
            auto current_time = chrono::steady_clock::now();
            
            // 处理帧信息头
            if (received == sizeof(FrameInfo)) {
                FrameInfo* info = (FrameInfo*)buffer;
                
                lock_guard<mutex> lock(g_buffer_mutex);
                
                // 创建新的帧缓存
                FrameBuffer& frame_buf = g_frame_buffers[info->frame_id];
                frame_buf.data.resize(info->total_size);
                frame_buf.received_packets.resize(info->packet_count, false);
                frame_buf.total_packets = info->packet_count;
                frame_buf.received_count = 0;
                frame_buf.start_time = current_time;
                frame_buf.width = info->width;
                frame_buf.height = info->height;
                frame_buf.is_complete = false;
                frame_buf.info_received = true;
                
                frames_received++;
                
                printf("✓ 接收帧信息: Frame %d, %dx%d, %d包, %dKB\n", 
                       info->frame_id, info->width, info->height, 
                       info->packet_count, info->total_size/1024);
            }
            // 处理数据包 - 修复关键逻辑
            else if (received >= sizeof(PacketHeader)) {
                PacketHeader* header = (PacketHeader*)buffer;
                
                // 数据在同一个UDP包中，跟在PacketHeader后面
                uint8_t* packet_data = (uint8_t*)buffer + sizeof(PacketHeader);
                size_t data_size = received - sizeof(PacketHeader);
                
                if (data_size != header->data_size) {
                    printf("[DEBUG] 数据大小不匹配: 期望%d, 实际%zu\n", 
                           header->data_size, data_size);
                    continue;
                }
                
                lock_guard<mutex> lock(g_buffer_mutex);
                
                auto it = g_frame_buffers.find(header->frame_id);
                if (it != g_frame_buffers.end() && it->second.info_received) {
                    FrameBuffer& frame_buf = it->second;
                    
                    // 检查包是否有效且未接收
                    if (header->packet_id < frame_buf.total_packets &&
                        !frame_buf.received_packets[header->packet_id]) {
                        
                        // 复制数据到缓存
                        size_t offset = header->packet_id * 1400; // 与发送端一致
                        if (offset + data_size <= frame_buf.data.size()) {
                            memcpy(frame_buf.data.data() + offset, packet_data, data_size);
                            frame_buf.received_packets[header->packet_id] = true;
                            frame_buf.received_count++;
                            
                            // 调试信息
                            if (frame_buf.received_count <= 3) {
                                printf("[DEBUG] Frame %d: 收到包 %d/%d (大小:%zu)\n",
                                       header->frame_id, header->packet_id + 1, 
                                       frame_buf.total_packets, data_size);
                            }
                            
                            // 检查帧是否完整
                            if (frame_buf.received_count == frame_buf.total_packets) {
                                frame_buf.is_complete = true;
                                
                                printf("✅ Frame %d 完整 (%d/%d包)\n", 
                                       header->frame_id, frame_buf.received_count, 
                                       frame_buf.total_packets);
                                
                                // 解码并显示图像
                                try {
                                    Mat img = imdecode(frame_buf.data, IMREAD_COLOR);
                                    if (!img.empty()) {
                                        string info_text = "Frame: " + to_string(header->frame_id) + 
                                                         " | Size: " + to_string(frame_buf.data.size()/1024) + "KB";
                                        putText(img, info_text, Point(10, 30), 
                                               FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
                                        
                                        imshow("5G摄像头实时视频", img);
                                        frames_displayed++;
                                        
                                        if (frames_displayed <= 5) {
                                            printf("✅ 成功显示第 %d 帧\n", frames_displayed);
                                        }
                                    }
                                } catch (const Exception& e) {
                                    printf("图像解码错误: %s\n", e.what());
                                }
                                
                                // 删除已完成的帧
                                g_frame_buffers.erase(it);
                            }
                        }
                    }
                } else {
                    if (packets_received <= 20) {
                        printf("[DEBUG] 找不到Frame %d的信息或未接收\n", header->frame_id);
                    }
                }
            }
            
            // 定期显示统计
            if (chrono::duration_cast<chrono::milliseconds>(current_time - last_stats).count() >= 2000) {
                printf("\r[统计] 包:%d | 帧:%d | 显示:%d | 缓存:%zu    ", 
                       packets_received, frames_received, frames_displayed, g_frame_buffers.size());
                fflush(stdout);
                last_stats = current_time;
            }
            
            // 检查窗口事件
            if (waitKey(1) == 27) {
                printf("\n按下ESC键，退出程序\n");
                g_running = false;
                break;
            }
        }
        
        destroyAllWindows();
    }
};

int main(int argc, char* argv[]) {
    printf("支持IPv6的龙芯派接收程序 v1.3\n");
    printf("编译时间: %s %s\n", __DATE__, __TIME__);
    printf("===========================================\n");
    
    int port = 8888;
    bool use_ipv6 = true;  // 默认使用IPv6
    
    if (argc >= 2) {
        port = atoi(argv[1]);
    }
    
    if (argc >= 3 && strcmp(argv[2], "ipv4") == 0) {
        use_ipv6 = false;
    }
    
    printf("使用方法: %s [端口] [ipv4|ipv6]\n", argv[0]);
    printf("当前配置: 端口=%d, 协议=%s\n", port, use_ipv6 ? "IPv6" : "IPv4");
    printf("===========================================\n");
    
    VideoReceiver receiver(port, use_ipv6);
    
    if (!receiver.start()) {
        printf("启动接收器失败\n");
        return 1;
    }
    
    receiver.receive_and_display();
    
    printf("程序正常退出\n");
    return 0;
}