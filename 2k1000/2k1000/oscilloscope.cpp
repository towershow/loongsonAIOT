#include "oscilloscope.h"
#include "ui_oscilloscope.h"
#include <QDesktopWidget>
#include "QPixmap"
#include"QPainter"
#include <QFile>
#include <QAudioOutput>
Oscilloscope::Oscilloscope(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Oscilloscope)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);  // 创建播放器

    // Qt5 不需要 QAudioOutput，直接设置音量
    player->setVolume(50);  // 音量范围 0~100

    // 设置视频输出（Qt5 使用 setVideoOutput）
    player->setVideoOutput(ui->videoWidget);
    // 初始隐藏 videoWidget
    ui->videoWidget->setVisible(false);  // 或 ui->videoWidget->hide();
    initial_ui();
}

Oscilloscope::~Oscilloscope()
{
    delete ui;
}

void Oscilloscope::initial_ui()
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

    ui->pushButton1->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(0, 255, 255, 0.7);"
        "    border: 2px solid #00FFFF;"
        "    padding: 5px;"
        "    border-radius: 5px;"
        "    image: url(:/images/mainwindow_ui/12.png);"
        "}"
        "QPushButton:checked {"
        "    background-color: transparent;"  // 选中时透明
        "}"
    );
    ui->pushButton2->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(0, 255, 255, 0.7);"
        "    border: 2px solid #00FFFF;"
        "    padding: 5px;"
        "    border-radius: 5px;"
        "    image: url(:/images/mainwindow_ui/12.png);"
        "}"
        "QPushButton:checked {"
        "    background-color: transparent;"  // 选中时透明
        "}"
    );
    ui->pushButton3->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(0, 255, 255, 0.7);"
        "    border: 2px solid #00FFFF;"
        "    padding: 5px;"
        "    border-radius: 5px;"
        "    image: url(:/images/mainwindow_ui/12.png);"
        "}"
        "QPushButton:checked {"
        "    background-color: transparent;"  // 选中时透明
        "}"
    );
    ui->pushButton4->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(0, 255, 255, 0.7);"
        "    border: 2px solid #00FFFF;"
        "    padding: 5px;"
        "    border-radius: 5px;"
        "    image: url(:/images/mainwindow_ui/12.png);"
        "}"
        "QPushButton:checked {"
        "    background-color: transparent;"  // 选中时透明
        "}"
    );


}

void Oscilloscope::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QSize labelSize = ui->label_line1->size();
    QPixmap pixmap(labelSize);
    pixmap.fill(Qt::transparent);  // 设置透明背景
    // 创建 QPainter 来绘制内容
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿
    QPen penYellow(QColor(0, 255, 255));
    penYellow.setWidth(2);  // 设置线条宽度
    painter.setPen(penYellow);
    painter.drawLine(8, labelSize.height() / 3, labelSize.width()-8, labelSize.height() / 3);
    ui->label_line1->setPixmap(pixmap);

}


void Oscilloscope::setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame)
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

void Oscilloscope::on_toolButton_mainWindow_clicked()
{
    back_mainWindow();
}


void Oscilloscope::on_toolButton_CAN_clicked()
{
    back_CANWindow();
}

void Oscilloscope::on_toolButton_Record_clicked()
{
    back_recordWindow();
}

void Oscilloscope::on_toolButton_deviceStatus_clicked()
{
    back_devstatusWindow();
}

void Oscilloscope::on_pushButton1_clicked()
{
    static bool isTransparent = false; // 静态变量记录当前状态

    if (!isTransparent) {
        // 切换到透明样式
        ui->pushButton1->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"  // 完全透明
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    } else {
        // 恢复半透明青色样式
        ui->pushButton1->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(0, 255, 255, 0.7);"
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    }
    qDebug() << "Supported formats:" << QMediaPlayer::supportedMimeTypes();
    // 控制视频播放和 videoWidget 显隐
    QString videoPath = "C:\\Users\\HP\\Desktop\\output.mp4";
    if (QFile::exists(videoPath)) {
        if (player->state() == QMediaPlayer::PlayingState) {
            player->pause();
            ui->videoWidget->hide();  // 暂停时隐藏
        } else {
            player->setMedia(QUrl::fromLocalFile(videoPath));
            player->play();
            ui->videoWidget->show();  // 播放时显示
        }
    } else {
        qDebug() << "视频文件不存在！";
    }


    isTransparent = !isTransparent; // 切换状态
}

void Oscilloscope::on_pushButton3_clicked()
{
    static bool isTransparent = false; // 静态变量记录当前状态

    if (!isTransparent) {
        // 切换到透明样式
        ui->pushButton3->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"  // 完全透明
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    } else {
        // 恢复半透明青色样式
        ui->pushButton3->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(0, 255, 255, 0.7);"
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    }

    isTransparent = !isTransparent; // 切换状态
}

void Oscilloscope::on_pushButton4_clicked()
{
    static bool isTransparent = false; // 静态变量记录当前状态

    if (!isTransparent) {
        // 切换到透明样式
        ui->pushButton4->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"  // 完全透明
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    } else {
        // 恢复半透明青色样式
        ui->pushButton4->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(0, 255, 255, 0.7);"
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    }

    isTransparent = !isTransparent; // 切换状态
}

void Oscilloscope::on_pushButton2_clicked()
{
    static bool isTransparent = false; // 静态变量记录当前状态

    if (!isTransparent) {
        // 切换到透明样式
        ui->pushButton2->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"  // 完全透明
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    } else {
        // 恢复半透明青色样式
        ui->pushButton2->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(0, 255, 255, 0.7);"
            "    border: 2px solid #00FFFF;"
            "    padding: 5px;"
            "    border-radius: 5px;"
            "    image: url(:/images/mainwindow_ui/12.png);"
            "}"
        );
    }

    isTransparent = !isTransparent; // 切换状态
}
