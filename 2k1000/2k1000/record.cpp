#include "record.h"
#include "ui_record.h"
#include <QDesktopWidget>
#include<QDateTime>
#include<QDebug>
Record::Record(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Record)
{
    ui->setupUi(this);
    initial_ui();
    initial_tableView();
}

Record::~Record()
{
    delete ui;
}

void Record::initial_ui()
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
}

void Record::setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame)
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

void Record::on_toolButton_mainWindow_clicked()
{
    back_mainWindow();
}

void Record::on_toolButton_Oscilloscope_clicked()
{
    back_OscilloscopeWindow();
}

void Record::on_toolButton_CAN_clicked()
{
    back_CANWindow();
}

void Record::on_toolButton_deviceStatus_clicked()
{
    back_devstatusWindow();
}

void Record::initial_tableView()
{
    ui->RecvText->setStyleSheet(
        "QTableView {"
        "   background-color: transparent;"
        "   border: 2px solid cyan;"
        "   color: rgba(0, 255, 255, 50);"
        "   font-size: 25px;"
        "   gridline-color: cyan;"
        "}"
        "QTableView::item {"
        "   background-color: transparent;"
        "   color: rgba(0, 255, 255, 150);"
        "   padding: 8px;"
        "}"
        "QHeaderView::section {"  // 表头样式
        "   background-color: rgba(1, 138, 145, 255);"
        "   color: cyan;"
        "   font-size: 40px;"
        "   padding: 10px;"
        "   border: 1px solid cyan;"  // 表头边框
        "}"
    );

    // 创建模型并设置表头
    theModel = new QStandardItemModel(this);
    QStringList headers = {"序号", "时间", "节点号", "紧急程度", "报警类型", "安全建议"};
    theModel->setHorizontalHeaderLabels(headers);
    ui->RecvText->setModel(theModel);

    theModel = new QStandardItemModel(this);  // 创建一个新的QStandardItemModel对象，用于管理表格数据
    QStringList lstr;  // 创建一个QStringList对象，用于储存表格的列标题
    lstr << "序号" << "时间" << "节点号" << "紧急程度" << "报警类型" << "安全建议";
    theModel->setHorizontalHeaderLabels(lstr);  // 使用QStringList中的字符串设置表格的水平表头
    ui->RecvText->setModel(theModel);  // 将创建的模型设置到ui->RecvText这个表格视图中

    // 设置列宽
    ui->RecvText->setColumnWidth(0, 250);
    ui->RecvText->setColumnWidth(1, 250);
    ui->RecvText->setColumnWidth(2, 250);
    ui->RecvText->setColumnWidth(3, 250);
    ui->RecvText->setColumnWidth(4, 350);
    ui->RecvText->setColumnWidth(5, 377);

    // 隐藏行号列（第一列）
    ui->RecvText->verticalHeader()->setVisible(false);

    ui->RecvText->setContextMenuPolicy(Qt::CustomContextMenu);

    insertSampleData();
}


void Record::insertSampleData()
{
    if (!theModel) return;

    // 清空现有数据
    theModel->removeRows(0, theModel->rowCount());

    // 添加5条示例数据
    for (int i = 1; i <= 5; ++i) {
        QList<QStandardItem*> rowItems;

        // 序号
        rowItems << new QStandardItem(QString::number(i));

        // 时间 (当前时间)
        rowItems << new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        // 节点号 (随机)
        rowItems << new QStandardItem(QString("Node-%1").arg(qrand() % 100));

        // 紧急程度 (低、中、高)
        QString urgency = "低";
        if (i == 3) urgency = "高";
        else if (i == 4) urgency = "中";
        rowItems << new QStandardItem(urgency);

        // 报警类型
        QString alarmType;
        switch(i % 3) {
            case 0: alarmType = "温度过高"; break;
            case 1: alarmType = "压力异常"; break;
            case 2: alarmType = "通信中断"; break;
        }
        rowItems << new QStandardItem(alarmType);

        // 安全建议
        QString suggestion;
        if (urgency == "高") suggestion = "立即停机检查";
        else if (urgency == "中") suggestion = "尽快安排检修";
        else suggestion = "定期观察";
        rowItems << new QStandardItem(suggestion);

        // 设置文字颜色
        for (auto item : rowItems) {
            item->setForeground(QBrush(QColor(0, 255, 255, 150))); // 青色文字
        }

        // 如果是紧急程度高的行，设置整行背景色
        if (urgency == "高") {
            for (auto item : rowItems) {
                item->setBackground(QBrush(QColor(255, 0, 0, 50))); // 半透明红色背景
            }
        }

        theModel->appendRow(rowItems);
    }
}

void Record::getMessage(uint32_t number,QString t_time,uint32_t point,QString emergency,QString t_type,QString suggestion)
{
    // 检查模型是否初始化
    if (!theModel) {
        qWarning() << "Model is not initialized!";
        return;
    }
    // 获取当前模型中的行数，作为新数据插入的行号
    int row = theModel->rowCount();

    // 使用 setItem 将每个字段添加到相应的列
    theModel->setItem(row, 0, new QStandardItem(QString::number(number))); // 序号
    theModel->setItem(row, 1, new QStandardItem(t_time));                      // 时间
    theModel->setItem(row, 2, new QStandardItem(point));                    // 节点
    theModel->setItem(row, 3, new QStandardItem(emergency)); // 晋级程度
    theModel->setItem(row, 4, new QStandardItem(t_type));                    // 类型
    theModel->setItem(row, 5, new QStandardItem(suggestion));               // 建议
}
