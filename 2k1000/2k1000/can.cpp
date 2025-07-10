#include "can.h"
#include "ui_can.h"
#include <QDesktopWidget>
static int number = 30000;

CAN::CAN(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CAN)
{
    ui->setupUi(this);
    initial_ui();
    initial_customplot();
    initial_tableView();
}

CAN::~CAN()
{
    delete ui;
}

void CAN::initial_ui()
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
    if (QFile::exists(":/images/mainwindow_ui/open.svg")) {
        qDebug() << "SVG 存在！";
    } else {
        qDebug() << "SVG 路径错误！";
    }


   // ui->pushButton->setText("启动扫描");
    ui->pushButton->setProperty("transparent", false);
    ui->pushButton->setStyleSheet("QPushButton { border-image: url(:/images/mainwindow_ui/open.svg); }");
}

void CAN::setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame)
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

void CAN::on_toolButton_mainWindow_clicked()
{
    back_mainWindow();

}

void CAN::on_toolButton_Oscilloscope_clicked()
{
    back_OscilloscopeWindow();

}

void CAN::on_toolButton_Record_clicked()
{
    back_recordWindow();

}

void CAN::on_toolButton_deviceStatus_clicked()
{
    back_devstatusWindow();

}

void CAN::initial_customplot()
{
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &CAN::updatePlot);
//    dataTimer->setInterval(8);  // 每8毫秒刷新一次，大约120Hz
//    dataTimer->start(8);
    // 确保初始状态同步
    ui->pushButton->setProperty("transparent", false);
    customPlot1 = ui->customPlot1;
    customPlot1->plotLayout()->clear();
    axisRect1 = new QCPAxisRect(customPlot1);
    customPlot1->plotLayout()->addElement(0, 0, axisRect1);

    // 设置坐标轴范围
    axisRect1->axis(QCPAxis::atBottom)->setRange(0, 1000);
    axisRect1->axis(QCPAxis::atLeft)->setRange(0.0f, 6.0f);

    // 交互设置
    customPlot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables);
    axisRect1->setRangeZoom(Qt::Horizontal);
    axisRect1->setRangeZoomFactor(1.2);

    // 颜色定义
    QColor cyanColor(0, 255, 255);  // 青色
    QColor goldColor(255, 215, 0);  // 金色
    QPen cyanPen(cyanColor);
    cyanPen.setWidth(1);

    // 设置底部和左侧坐标轴（保留刻度值）
    QCPAxis* mainAxes[] = {
        axisRect1->axis(QCPAxis::atBottom),
        axisRect1->axis(QCPAxis::atLeft)
    };

    for (QCPAxis* axis : mainAxes) {
        axis->setBasePen(cyanPen);
        axis->setTickPen(cyanPen);
        axis->setSubTickPen(cyanPen);
        axis->setTickLabelColor(goldColor);
        axis->setLabelColor(goldColor);
        axis->setTickLabelFont(QFont("Arial", 10));
        axis->grid()->setVisible(true);
        axis->grid()->setPen(QPen(cyanColor, 0, Qt::DotLine));
    }

    // 设置顶部和右侧坐标轴（隐藏刻度值）
    QCPAxis* secondaryAxes[] = {
        axisRect1->axis(QCPAxis::atTop),
        axisRect1->axis(QCPAxis::atRight)
    };

    for (QCPAxis* axis : secondaryAxes) {
        axis->setBasePen(cyanPen);
        axis->setTickPen(cyanPen);
        axis->setSubTickPen(cyanPen);
        axis->setTickLabels(false);  // 关键：隐藏刻度值
        axis->grid()->setVisible(false);
    }

    // 设置坐标轴标签
    axisRect1->axis(QCPAxis::atBottom)->setLabel("时间 (ms)");
    axisRect1->axis(QCPAxis::atLeft)->setLabel("数值");
    axisRect1->axis(QCPAxis::atBottom)->setLabelFont(QFont("Arial", 15));
    axisRect1->axis(QCPAxis::atLeft)->setLabelFont(QFont("Arial", 15));

    // 确保四个坐标轴都可见（形成封闭框）
    axisRect1->axis(QCPAxis::atTop)->setVisible(true);
    axisRect1->axis(QCPAxis::atRight)->setVisible(true);

    // 背景设置
    customPlot1->setBackground(QBrush(QColor(59, 76, 91, 0)));
    axisRect1->setBackground(QBrush(Qt::transparent));
    QCPGraph *graph1 = customPlot1->addGraph(axisRect1->axis(QCPAxis::atBottom), axisRect1->axis(QCPAxis::atLeft));     // CAN_H
    QCPGraph *graph2 = customPlot1->addGraph(axisRect1->axis(QCPAxis::atBottom), axisRect1->axis(QCPAxis::atLeft));     // CAN_L
    QCPGraph *graph3 = customPlot1->addGraph(axisRect1->axis(QCPAxis::atBottom), axisRect1->axis(QCPAxis::atLeft));

    // 设置图形颜色
    graph1->setPen(QPen(QColor(144, 238, 144), 2));  // 设置颜色为淡绿
    graph2->setPen(QPen(QColor(255, 102, 0), 2));  //
    graph3->setPen(QPen(QColor(244, 177, 131), 2));  // 线条宽度为2
}

void CAN::updatePlot()
{
    static double time = 0; // 时间变量用于波形生成
    const int dataCount = 1000; // 数据点数量
    // 准备数据容器
    QVector<double> x(dataCount), y1(dataCount), y2(dataCount), y3(dataCount);
    // 生成三种不同的波形
    for (int i = 0; i < dataCount; ++i) {
        // X轴数据 (0-999)
        x[i] = i;
        // 1. 正弦波 (湿度数据)
        y1[i] = 3.0 * sin(2 * M_PI * 0.005 * x[i] + time) + 3.0;
        // 2. 方波 (可燃气体浓度)
        y2[i] = (fmod(0.02 * x[i] + time, 2.0) < 1.0 ? 1.0 : 5.0);
        // 3. 锯齿波 (温度数据)
        y3[i] = fmod(0.03 * x[i] + time, 6.0);
    }
    time += 0.5; // 递增时间使波形移动


    // 更新图表数据
    customPlot1->graph(0)->setData(x, y1); // 湿度数据 (淡绿)
    customPlot1->graph(1)->setData(x, y2); // 气体浓度 (棕红)
    customPlot1->graph(2)->setData(x, y3); // 温度数据 (浅橙)

    // 重绘图表
    customPlot1->replot();
}

void CAN::on_pushButton_clicked()
{
    bool isTransparent = ui->pushButton->property("transparent").toBool();

    // 切换状态（注意取反位置变化）
    isTransparent = !isTransparent;
    ui->pushButton->setProperty("transparent", isTransparent);

    // 根据新状态控制定时器
    if (!isTransparent) {  // 当显示"启动扫描"时停止，显示"停止扫描"时启动
        dataTimer->stop();
        ui->pushButton->setStyleSheet("QPushButton { border-image: url(:/images/mainwindow_ui/open.svg); border: none; }");
    } else {
        dataTimer->start(8);
        ui->pushButton->setStyleSheet("QPushButton { border-image: url(:/images/mainwindow_ui/close.svg); border: none; }");
    }

    // 强制样式刷新
    ui->pushButton->style()->unpolish(ui->pushButton);
    ui->pushButton->style()->polish(ui->pushButton);
}


void CAN::initial_tableView()
{
    // ================= 1. 表格样式设置 =================
    ui->tableView->setStyleSheet(
        "QTableView {"
        "   background-color: transparent;"
        "   border: 2px solid cyan;"
        "   color: rgba(0, 255, 255, 50);"
        "   font-size: 25px;"
        "   gridline-color: cyan;"
        "   selection-background-color: rgba(0, 255, 255, 50);"  // 选中项背景
        "}"
        "QTableView::item {"
        "   background-color: transparent;"
        "   color: rgba(0, 255, 255, 150);"
        "   padding: 8px;"
        "}"
        "QHeaderView::section {"
        "   background-color: rgba(1, 138, 145, 255);"
        "   color: cyan;"
        "   font-size: 25px;"  // 调小表头字体避免溢出
        "   padding: 5px;"      // 减少内边距
        "   border: 1px solid cyan;"
        "   margin: 0px;"       // 移除外边距
        "}"
    );

    // ================= 2. 初始化模型和表头 =================
    theModel = new QStandardItemModel(this);
    QStringList headers = {"节点号", "温度数据", "湿度数据", "可燃气体浓度数据"};
    theModel->setHorizontalHeaderLabels(headers);
    ui->tableView->setModel(theModel);

    // ================= 3. 列宽和布局设置 =================
    // 固定列宽（总宽度需略小于tableView宽度）
    ui->tableView->setColumnWidth(0, 456);  // 节点号
    ui->tableView->setColumnWidth(1, 460);  // 温度
    ui->tableView->setColumnWidth(2, 460);  // 湿度
    ui->tableView->setColumnWidth(3, 460);  // 气体浓度

    // 隐藏行号列（第一列）
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // ================= 4. 插入示例数据 =================
    // insertSampleData();
}

void CAN::insertSampleData()
{
    if (!theModel) return;
    theModel->removeRows(0, theModel->rowCount());

    // ================= 1. 模拟数据 =================
    QVector<QStringList> sampleData = {
        {"Node-001", "25.3°C", "45%", "0.02% (安全)"},
        {"Node-002", "28.1°C", "52%", "0.05% (安全)"},
        {"Node-003", "32.7°C", "38%", "0.11% (警告)"},  // 气体浓度高亮
        {"Node-004", "30.0°C", "60%", "0.08% (安全)"},
        {"Node-005", "19.5°C", "32%", "0.00% (安全)"}
    };

    // ================= 2. 颜色定义 =================
    const QColor nodeColor(0, 255, 255, 200);    // 青色-节点
    const QColor tempColor(255, 100, 100, 200);  // 红色-温度
    const QColor humiColor(100, 255, 100, 200);  // 绿色-湿度
    const QColor gasSafe(255, 215, 0, 200);      // 金色-安全
    const QColor gasWarn(255, 50, 50, 220);      // 亮红-警告

    // ================= 3. 插入数据 =================
    for (int row = 0; row < sampleData.size(); ++row) {
        for (int col = 0; col < sampleData[row].size(); ++col) {
            QStandardItem *item = new QStandardItem(sampleData[row][col]);

            // 按列设置颜色
            switch (col) {
                case 0: item->setForeground(nodeColor); break;
                case 1: item->setForeground(tempColor); break;
                case 2: item->setForeground(humiColor); break;
                case 3:
                    // 气体浓度特殊处理：超过0.1%显示警告色
                    if (sampleData[row][col].contains("警告")) {
                        item->setForeground(gasWarn);
                        item->setBackground(QColor(255, 0, 0, 30));  // 浅红背景
                    } else {
                        item->setForeground(gasSafe);
                    }
                    break;
            }

            // 统一设置对齐方式
            item->setTextAlignment(Qt::AlignCenter);
            theModel->setItem(row, col, item);
        }
        ui->tableView->setRowHeight(row, 50);  // 固定行高
    }
}
