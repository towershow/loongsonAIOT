#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTimer>
#include "oscilloscope.h"
#include "can.h"
#include "record.h"
#include "sys_status.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initial_ui();
    this->Oscilloscope_ui = new Oscilloscope;
    this->can_ui = new CAN;
    this->record_ui = new Record;
    this->sys_ui = new sys_status;
    // Oscilloscope界面 进入主界面
    connect(this->Oscilloscope_ui,&Oscilloscope::back_mainWindow,[=]()
            {
                this->Oscilloscope_ui->hide();
                this->show();
            });
    // Oscilloscope界面 进入CAN分析界面
    connect(this->Oscilloscope_ui,&Oscilloscope::back_CANWindow,[=]()
            {
                this->Oscilloscope_ui->hide();
                this->can_ui->show();
            });
    // Oscilloscope界面 进入历史记录界面
    connect(this->Oscilloscope_ui,&Oscilloscope::back_recordWindow,[=]()
            {
                this->Oscilloscope_ui->hide();
                this->record_ui->show();
            });
    // Oscilloscope界面 进入设备状态界面
    connect(this->Oscilloscope_ui,&Oscilloscope::back_devstatusWindow,[=]()
            {
                this->Oscilloscope_ui->hide();
                this->sys_ui->show();
            });
    // CAN界面 进入首页界面
    connect(this->can_ui,&CAN::back_mainWindow,[=]()
            {
                this->can_ui->hide();
                this->show();
            });
    // CAN界面 进入Oscilloscope界面
    connect(this->can_ui,&CAN::back_OscilloscopeWindow,[=]()
            {
                this->can_ui->hide();
                this->Oscilloscope_ui->show();
            });
    // CAN界面 进入历史记录界面
    connect(this->can_ui,&CAN::back_recordWindow,[=]()
            {
                this->can_ui->hide();
                this->record_ui->show();
            });
    // CAN界面 进入设备状态界面
    connect(this->can_ui,&CAN::back_devstatusWindow,[=]()
            {
                this->can_ui->hide();
                this->sys_ui->show();
            });

    // 历史记录界面 进入首页界面
    connect(this->record_ui,&Record::back_mainWindow,[=]()
            {
                this->record_ui->hide();
                this->show();
            });
    // 历史记录界面 进入示波器界面
    connect(this->record_ui,&Record::back_OscilloscopeWindow,[=]()
            {
                this->record_ui->hide();
                this->Oscilloscope_ui->show();
            });
    // 历史记录界面 进入CAN分析界面
    connect(this->record_ui,&Record::back_CANWindow,[=]()
            {
                this->record_ui->hide();
                this->can_ui->show();
            });
    // 历史记录界面 进入设备状态界面
    connect(this->record_ui,&Record::back_devstatusWindow,[=]()
            {
                this->record_ui->hide();
                this->sys_ui->show();
            });

    // 设备状态界面 进入首页界面
    connect(this->sys_ui,&sys_status::back_mainWindow,[=]()
            {
                this->sys_ui->hide();
                this->show();
            });
    // 设备状态界面 进入示波器界面
    connect(this->sys_ui,&sys_status::back_OscilloscopeWindow,[=]()
            {
                this->sys_ui->hide();
                this->Oscilloscope_ui->show();

            });
    // 设备状态界面 进入CAN分析界面
    connect(this->sys_ui,&sys_status::back_CANWindow,[=]()
            {

                this->sys_ui->hide();
                this->can_ui->show();
            });
    // 设备状态界面 进入历史记录界面
    connect(this->sys_ui,&sys_status::back_recordWindow,[=]()
            {
                this->sys_ui->hide();
                this->record_ui->show();
            });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initial_ui()
{
    setWindowTitle("一周十吨矿泉水");
    setWindowIcon(QIcon(":/images/mainwindow_ui/tree.ico"));
    // 使窗口居中
    this->setFixedSize(1920, 1017);
    QRect screenGeometry = QApplication::desktop()->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    QSize windowSize = this->size();
    int windowWidth = windowSize.width();
    int windowHeight = windowSize.height();

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    this->move(x, y - 10);

    // 创建按钮和线条
    QFrame *lineFrame1 = new QFrame(this);
    setupToolButton(ui->toolButton_mainWindow, ":images/mainwindow_ui/mainwindow.ico", "首页", lineFrame1);

    QFrame *lineFrame2 = new QFrame(this);
    setupToolButton(ui->toolButton_Oscilloscope, ":images/mainwindow_ui/1.ico", "AI监测", lineFrame2);
    // 获取当前图标大小
    QSize originalSize = ui->toolButton_Oscilloscope->iconSize();
    // 按比例缩放（例如放大 2 倍）
    QSize scaledSize = originalSize * 1.1;  // 或者缩小：originalSize * 0.5
    // 设置新的图标大小
    ui->toolButton_Oscilloscope->setIconSize(scaledSize);

    QFrame *lineFrame3 = new QFrame(this);
    setupToolButton(ui->toolButton_CAN, ":images/mainwindow_ui/2.ico", "环境感知", lineFrame3);
    // 获取当前图标大小
    QSize originalSize1 = ui->toolButton_CAN->iconSize();
    // 按比例缩放（例如放大 2 倍）
    QSize scaledSize1 = originalSize1 * 1.05;  // 或者缩小：originalSize * 0.5
    // 设置新的图标大小
    ui->toolButton_CAN->setIconSize(scaledSize1);

    QFrame *lineFrame4 = new QFrame(this);
    setupToolButton(ui->toolButton_Record, ":images/mainwindow_ui/Record.ico", "历史记录", lineFrame4);

    QFrame *lineFrame5 = new QFrame(this);
    setupToolButton(ui->toolButton_deviceStatus, ":images/mainwindow_ui/9.ico", "系统状态", lineFrame5);
    // 添加右边框
    QFrame *rightLineFrame = new QFrame(this);
    rightLineFrame->setStyleSheet("background-color: cyan;"); // 设置线条颜色为青色
    rightLineFrame->setFixedWidth(2); // 设置线条宽度
    rightLineFrame->setFixedHeight(ui->toolButton_deviceStatus->height() * 0.66); // 设置线条高度为按钮高度的三分之二
    rightLineFrame->setParent(ui->toolButton_deviceStatus);
    rightLineFrame->move(ui->toolButton_deviceStatus->width() - rightLineFrame->width(), (ui->toolButton_deviceStatus->height() - rightLineFrame->height()) / 2); // 垂直居中线条，水平靠右

    ui->toolButton_Oscilloscope->setStyleSheet(
        "QToolButton {"
        "    background-color: rgba(255, 255, 255, 0.1);"
        "    border: none;"
        "    border-radius: 10px;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 25px;"
        "    icon-size: 25px;"
        "    padding-left: 0px;"   // 减少左侧 padding，让图标更靠左
        "    padding-right: 0px;" // 增加右侧 padding，防止文字被截断
        "    text-align: right;"   // 让文字靠右，图标自然向左移动
        "}"
        "QToolButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.5);"
        "}"
        "QToolButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.7);"
        "}"
    );
    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(ui->label_currentTime);  // 时间在上面
    layout->addWidget(ui->label_date);  // 日期在下面

    QTimer *timer;
    timer = new QTimer(this);
    // 更新时间显示函数
    auto updateTimeDisplay = [&]() {
        QDateTime now = QDateTime::currentDateTime();
        QString timeFormat = now.toString("HH:mm:ss");
        QString dateFormat = now.toString("yyyy-MM-dd");
        ui->label_currentTime->setText(timeFormat);
        ui->label_date->setText(dateFormat);
    };

    // 首先立即更新一次时间
    updateTimeDisplay();

    // 连接计时器
    connect(timer, &QTimer::timeout, this, updateTimeDisplay);
    timer->start(1000);  // 每秒更新一次时间
}

void MainWindow::setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame)
{
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(64, 64)); // 根据需要调整图标大小
    button->setText(text);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置图标在上，文本在下
    // 设置左边框线条
    leftLineFrame->setStyleSheet("background-color: cyan;"); // 设置线条颜色为青色
    leftLineFrame->setFixedWidth(2); // 设置线条宽度
    leftLineFrame->setFixedHeight(button->height() * 0.66); // 设置线条高度为按钮高度的三分之二
    leftLineFrame->setParent(button);
    leftLineFrame->move(0, (button->height() - leftLineFrame->height()) / 2); // 垂直居中线条

    // 如果有右边框线条，设置它
    if (rightLineFrame) {
        rightLineFrame->setStyleSheet("background-color: cyan;"); // 设置线条颜色为青色
        rightLineFrame->setFixedWidth(2); // 设置线条宽度
        rightLineFrame->setFixedHeight(button->height() * 0.66); // 设置线条高度为按钮高度的三分之二
        rightLineFrame->setParent(button);
        rightLineFrame->move(button->width() - rightLineFrame->width(), (button->height() - rightLineFrame->height()) / 2); // 垂直居中线条
    }

    button->setStyleSheet("QToolButton {"
                          " background-color: rgba(255, 255, 255, 0.1);" // 背景白色，透明度为 0.3
                          " border: none;"                                // 去掉其他边框
                          " border-radius: 10px;"                         // 设置圆角
                          " padding-left: 10px;"                          // 左侧内边距
                          " color: white;"
                          " font-weight: bold;"                           // 设置字体加粗
                          " font-size: 25px;"                             // 设置字体大小（根据需要调整）
                          " }"
                          "QToolButton:hover {"
                          " background-color: rgba(255, 255, 255, 0.5);" // 悬停状态背景透明度增大
                          " }"
                          "QToolButton:pressed {"
                          " background-color: rgba(255, 255, 255, 0.7);"  // 按下状态背景透明度最大
                          " }");
}

void MainWindow::on_toolButton_Oscilloscope_clicked()
{
    this->Oscilloscope_ui->show();
    this->hide();
}


void MainWindow::on_toolButton_CAN_clicked()
{
    this->can_ui->show();
    this->hide();
}


void MainWindow::on_toolButton_Record_clicked()
{
    this->record_ui->show();
    this->hide();
}


void MainWindow::on_toolButton_deviceStatus_clicked()
{
    this->sys_ui->show();
    this->hide();
}
