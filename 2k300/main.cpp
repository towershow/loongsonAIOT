/*
 * 修复版5G摄像头实时传输系统 - 发送端
 * 编译环境: LoongArch64 + OpenCV
 * 主要修复: 协议一致性问题，将包头和数据合并发送
 * 编译命令: g++ -o camera_sender sender.cpp `pkg-config --cflags --libs opencv4`
 */

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ===========================================
// 全局变量和常量定义
// ===========================================

static bool g_streaming_active = true;     // 传输控制标志
static int g_udp_socket = -1;              // UDP套接字

// 网络配置
const int DEFAULT_PORT = 8888;             // 默认端口
const int MAX_UDP_PACKET = 1400;           // UDP最大安全包大小
const int JPEG_QUALITY = 75;               // JPEG压缩质量

// 摄像头配置
const int CAMERA_WIDTH = 640;              // 摄像头宽度
const int CAMERA_HEIGHT = 480;             // 摄像头高度
const int CAMERA_FPS = 20;                 // 摄像头帧率
const int FRAME_DELAY_MS = 50;             // 帧间延迟(ms)

// ===========================================
// 数据结构定义 (必须与接收端保持一致)
// ===========================================

// 帧信息头
struct FrameInfo {
    uint32_t frame_id;          // 帧编号
    uint32_t total_size;        // 帧总大小
    uint32_t packet_count;      // 分包数量
    uint32_t width;             // 图像宽度
    uint32_t height;            // 图像高度
    uint64_t timestamp;         // 时间戳
};

// 数据包头
struct PacketHeader {
    uint32_t frame_id;          // 帧编号
    uint32_t packet_id;         // 包编号
    uint32_t data_size;         // 数据大小
};

// ===========================================
// 信号处理函数
// ===========================================

void signal_handler(int signal) {
    printf("\n收到信号 %d，正在停止摄像头传输...\n", signal);
    g_streaming_active = false;
    
    if (g_udp_socket != -1) {
        close(g_udp_socket);
        g_udp_socket = -1;
    }
    
    printf("程序即将退出\n");
}

// ===========================================
// 网络工具函数
// ===========================================

bool test_network_connection(const string& target_ip, int target_port) {
    printf("测试网络连接: %s:%d\n", target_ip.c_str(), target_port);
    
    int test_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (test_socket < 0) {
        printf("错误: 无法创建测试socket: %s\n", strerror(errno));
        return false;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(target_port);
    
    if (inet_aton(target_ip.c_str(), &addr.sin_addr) == 0) {
        printf("错误: 无效的IP地址格式\n");
        close(test_socket);
        return false;
    }
    
    const char* test_msg = "CONNECTION_TEST";
    ssize_t sent = sendto(test_socket, test_msg, strlen(test_msg), 0,
                         (struct sockaddr*)&addr, sizeof(addr));
    
    close(test_socket);
    
    if (sent > 0) {
        printf("✓ 网络连接测试成功\n");
        return true;
    } else {
        printf("✗ 网络连接测试失败: %s\n", strerror(errno));
        return false;
    }
}

bool check_system_status() {
    printf("检查系统状态...\n");
    
    // 检查网络接口
    printf("网络接口信息:\n");
    system("ip addr show | grep 'inet ' | head -5");
    
    // 检查默认路由
    printf("默认路由:\n");
    system("ip route show default");
    
    // 检查网络连通性
    printf("检查互联网连接...\n");
    int ping_result = system("ping -c 1 -W 3 8.8.8.8 > /dev/null 2>&1");
    
    if (ping_result == 0) {
        printf("✓ 网络连接正常\n");
        return true;
    } else {
        printf("⚠ 网络连接可能有问题\n");
        return false;
    }
}

// ===========================================
// 5G摄像头传输核心函数
// ===========================================

int start_camera_streaming(const string& target_ip, int target_port) {
    printf("\n===========================================\n");
    printf("      修复版5G摄像头实时传输系统\n");
    printf("===========================================\n");
    printf("目标地址: %s:%d\n", target_ip.c_str(), target_port);
    printf("摄像头配置: %dx%d @ %d fps\n", CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_FPS);
    printf("图像质量: %d%%\n", JPEG_QUALITY);
    printf("最大包大小: %d 字节\n", MAX_UDP_PACKET);
    printf("按 Ctrl+C 停止传输\n");
    printf("===========================================\n");
    
    // 注册信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 1. 初始化摄像头
    printf("\n[步骤1] 初始化摄像头...\n");
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        printf("错误: 无法打开摄像头设备\n");
        return -1;
    }
    
    // 配置摄像头参数
    camera.set(CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    camera.set(CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
    camera.set(CAP_PROP_FPS, CAMERA_FPS);
    camera.set(CAP_PROP_BUFFERSIZE, 1); // 减少缓冲区延迟
    
    // 验证配置
    int actual_width = camera.get(CAP_PROP_FRAME_WIDTH);
    int actual_height = camera.get(CAP_PROP_FRAME_HEIGHT);
    double actual_fps = camera.get(CAP_PROP_FPS);
    
    printf("✓ 摄像头就绪: %dx%d @ %.1f fps\n", actual_width, actual_height, actual_fps);
    
    // 2. 创建网络连接
    printf("\n[步骤2] 建立网络连接...\n");
    g_udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_udp_socket < 0) {
        printf("错误: 无法创建UDP socket: %s\n", strerror(errno));
        camera.release();
        return -1;
    }
    
    // 配置目标地址
    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    
    if (inet_aton(target_ip.c_str(), &target_addr.sin_addr) == 0) {
        printf("错误: 无效的目标IP地址\n");
        close(g_udp_socket);
        camera.release();
        return -1;
    }
    
    // 设置socket选项
    int send_buffer = 8 * 1024 * 1024; // 8MB发送缓冲区
    if (setsockopt(g_udp_socket, SOL_SOCKET, SO_SNDBUF, &send_buffer, sizeof(send_buffer)) < 0) {
        printf("警告: 无法设置发送缓冲区大小\n");
    }
    
    // 设置socket超时
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(g_udp_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
    printf("✓ 网络连接已建立\n");
    
    // 3. 开始视频传输
    printf("\n[步骤3] 开始视频传输...\n");
    
    Mat frame;
    vector<uchar> jpeg_data;
    vector<int> encode_params = {IMWRITE_JPEG_QUALITY, JPEG_QUALITY};
    
    uint32_t frame_counter = 0;
    time_t start_time = time(NULL);
    time_t last_display_time = start_time;
    int frames_per_second = 0;
    int total_packets_sent = 0;
    int failed_packets = 0;
    
    printf("传输状态: [帧数] [FPS] [帧大小] [包数] [失败] [总时间]\n");
    
    while (g_streaming_active) {
        // 读取摄像头帧
        camera >> frame;
        if (frame.empty()) {
            printf("警告: 获取空帧，跳过\n");
            usleep(FRAME_DELAY_MS * 1000);
            continue;
        }
        
        // 压缩为JPEG
        if (!imencode(".jpg", frame, jpeg_data, encode_params)) {
            printf("错误: 图像压缩失败\n");
            continue;
        }
        
        // 计算分包数量（关键修复：确保数据大小计算正确）
        uint32_t packet_count = (jpeg_data.size() + MAX_UDP_PACKET - 1) / MAX_UDP_PACKET;
        
        // 准备帧信息
        FrameInfo frame_info;
        frame_info.frame_id = frame_counter;
        frame_info.total_size = jpeg_data.size();
        frame_info.packet_count = packet_count;
        frame_info.width = actual_width;
        frame_info.height = actual_height;
        frame_info.timestamp = time(NULL);
        
        // 发送帧信息头
        ssize_t sent = sendto(g_udp_socket, &frame_info, sizeof(frame_info), 0,
                             (struct sockaddr*)&target_addr, sizeof(target_addr));
        if (sent < 0) {
            printf("错误: 发送帧头失败: %s\n", strerror(errno));
            failed_packets++;
            continue;
        }
        
        // 调试信息：显示帧信息
        if (frame_counter < 5) {
            printf("[DEBUG] Frame %d: 大小=%dKB, 包数=%d\n", 
                   frame_counter, (int)(jpeg_data.size()/1024), packet_count);
        }
        
        // 分包发送图像数据 - 关键修复：包头和数据合并发送
        bool send_success = true;
        for (uint32_t packet_id = 0; packet_id < packet_count; packet_id++) {
            size_t offset = packet_id * MAX_UDP_PACKET;
            size_t remaining = jpeg_data.size() - offset;
            size_t packet_data_size = min((size_t)MAX_UDP_PACKET, remaining);
            
            // 创建数据包头
            PacketHeader packet_header;
            packet_header.frame_id = frame_counter;
            packet_header.packet_id = packet_id;
            packet_header.data_size = packet_data_size;
            
            // 创建完整的UDP包（包头 + 数据）- 关键修复
            vector<uint8_t> udp_packet(sizeof(PacketHeader) + packet_data_size);
            
            // 复制包头
            memcpy(udp_packet.data(), &packet_header, sizeof(PacketHeader));
            
            // 复制数据
            memcpy(udp_packet.data() + sizeof(PacketHeader), 
                   jpeg_data.data() + offset, packet_data_size);
            
            // 一次性发送包头+数据 - 关键修复
            sent = sendto(g_udp_socket, udp_packet.data(), udp_packet.size(), 0,
                         (struct sockaddr*)&target_addr, sizeof(target_addr));
            
            if (sent < 0) {
                printf("错误: 发送数据包失败 (包 %d/%d): %s\n", 
                       packet_id + 1, packet_count, strerror(errno));
                send_success = false;
                failed_packets++;
                break;
            }
            
            total_packets_sent++;
            
            // 调试信息：显示前几个包的详细信息
            if (frame_counter < 3 && packet_id < 3) {
                printf("[DEBUG] Frame %d, 包 %d: UDP包大小=%zu (头%zu+数据%zu)\n",
                       frame_counter, packet_id, udp_packet.size(), 
                       sizeof(PacketHeader), packet_data_size);
            }
            
            // 添加小延迟防止网络拥塞 - 5G网络优化
            usleep(500); // 0.5ms延迟
        }
        
        if (send_success) {
            frame_counter++;
            frames_per_second++;
        }
        
        // 显示传输统计 (每秒更新一次)
        time_t current_time = time(NULL);
        if (current_time > last_display_time) {
            time_t total_time = current_time - start_time;
            double avg_fps = frame_counter / (double)(total_time + 1);
            
            printf("[%05d] [%02d fps] [%3d KB] [%3d包] [%3d失败] [%02ld:%02ld]\n",
                   frame_counter, frames_per_second, (int)(jpeg_data.size() / 1024),
                   packet_count, failed_packets, total_time / 60, total_time % 60);
            
            frames_per_second = 0;
            last_display_time = current_time;
        }
        
        // 控制帧率
        usleep(FRAME_DELAY_MS * 1000);
    }
    
    // 清理资源
    camera.release();
    if (g_udp_socket != -1) {
        close(g_udp_socket);
        g_udp_socket = -1;
    }
    
    // 显示最终统计
    time_t total_time = time(NULL) - start_time;
    printf("\n===========================================\n");
    printf("传输完成统计:\n");
    printf("总传输时间: %ld 分 %ld 秒\n", total_time / 60, total_time % 60);
    printf("总传输帧数: %d 帧\n", frame_counter);
    printf("总发送包数: %d 包\n", total_packets_sent);
    printf("失败包数: %d 包\n", failed_packets);
    printf("成功率: %.2f%%\n", 100.0 * (total_packets_sent - failed_packets) / total_packets_sent);
    printf("平均帧率: %.2f fps\n", frame_counter / (double)(total_time + 1));
    printf("===========================================\n");
    
    return 0;
}

// ===========================================
// 主函数
// ===========================================

int main(int argc, char* argv[]) {
    printf("修复版5G摄像头实时传输系统 v1.1\n");
    printf("编译时间: %s %s\n", __DATE__, __TIME__);
    printf("主要修复: 协议一致性问题\n");
    printf("===========================================\n");
    
    // 解析命令行参数
    string target_ip = "192.168.1.100";  // 默认目标IP
    int target_port = DEFAULT_PORT;       // 默认端口
    
    if (argc >= 2) {
        target_ip = argv[1];
    }
    if (argc >= 3) {
        target_port = atoi(argv[2]);
    }
    
    printf("使用方法: %s [目标IP] [端口]\n", argv[0]);
    printf("当前配置: %s:%d\n", target_ip.c_str(), target_port);
    printf("===========================================\n");
    
    // 系统检查
    printf("执行系统检查...\n");
    if (!check_system_status()) {
        printf("警告: 系统检查发现问题，但继续执行\n");
    }
    
    // 网络测试
    if (!test_network_connection(target_ip, target_port)) {
        printf("警告: 网络测试失败，但继续尝试传输\n");
        printf("请确保:\n");
        printf("1. PC端接收程序已启动\n");
        printf("2. 防火墙允许端口 %d\n", target_port);
        printf("3. 网络连接正常\n");
    }
    
    printf("\n准备开始传输...\n");
    printf("请确保PC端接收程序已启动并监听端口 %d\n", target_port);
    printf("按回车键开始，或Ctrl+C取消\n");
    getchar();
    
    // 开始摄像头传输
    int result = start_camera_streaming(target_ip, target_port);
    
    if (result == 0) {
        printf("程序正常退出\n");
    } else {
        printf("程序异常退出，错误代码: %d\n", result);
    }
    
    return result;
}