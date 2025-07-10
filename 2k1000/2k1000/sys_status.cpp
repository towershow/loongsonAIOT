#include "sys_status.h"
#include "ui_sys_status.h"
#include <QDesktopWidget>

sys_status::sys_status(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sys_status)
{
    ui->setupUi(this);
    initial_ui();
    initial_tableView();
}

sys_status::~sys_status()
{
    delete ui;
}

void sys_status::initial_ui(){
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

void sys_status::setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame){
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

void sys_status::on_toolButton_Record_clicked()
{
    back_recordWindow();

}

void sys_status::on_toolButton_CAN_clicked()
{
    back_CANWindow();

}

void sys_status::on_toolButton_Oscilloscope_clicked()
{
    back_OscilloscopeWindow();
}

void sys_status::on_toolButton_mainWindow_clicked()
{
    back_mainWindow();

}
void sys_status::initial_tableView()
{
    ui->tableView->setStyleSheet(
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
    QStringList headers = {"节点号", "通信链接状态", "传感器数据", "预警状态"};
    theModel->setHorizontalHeaderLabels(headers);
    ui->tableView->setModel(theModel);

    // 设置列宽
    ui->tableView->setColumnWidth(0, 295);
    ui->tableView->setColumnWidth(1, 295);
    ui->tableView->setColumnWidth(2, 295);
    ui->tableView->setColumnWidth(3, 302);

    // 隐藏行号列（第一列）
    ui->tableView->verticalHeader()->setVisible(false);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    insertSampleData();
}
void sys_status::insertSampleData()
{
    // Clear any existing data
    theModel->removeRows(0, theModel->rowCount());

    // 定义字体（加大字号）
    QFont font;
    font.setPointSize(18);  // 比原来的 25px 更大
    // 或者使用像素单位（如果更符合你的需求）
    // font.setPixelSize(30);

    // Insert sample data rows
    QList<QStandardItem*> row;

    // Row 1: A-01
    row.clear();
    row << new QStandardItem("A-01") << new QStandardItem("正常") << new QStandardItem("正常") << new QStandardItem("未预警");
    theModel->appendRow(row);

    // Row 2: B-01
    row.clear();
    row << new QStandardItem("B-01") << new QStandardItem("正常") << new QStandardItem("正常") << new QStandardItem("未预警");
    theModel->appendRow(row);

    // Row 3: B-02
    row.clear();
    row << new QStandardItem("B-02") << new QStandardItem("正常") << new QStandardItem("正常") << new QStandardItem("未预警");
    theModel->appendRow(row);

    // Row 4: C-01
    row.clear();
    row << new QStandardItem("C-01") << new QStandardItem("正常") << new QStandardItem("正常") << new QStandardItem("未预警");
    theModel->appendRow(row);

    // Row 5: C-02
    row.clear();
    row << new QStandardItem("C-02") << new QStandardItem("正常") << new QStandardItem("正常") << new QStandardItem("未预警");
    theModel->appendRow(row);

    // Set text alignment, color, and font for all items
    for(int row = 0; row < theModel->rowCount(); ++row) {
        for(int col = 0; col < theModel->columnCount(); ++col) {
            QStandardItem* item = theModel->item(row, col);
            item->setTextAlignment(Qt::AlignCenter);
            item->setForeground(QBrush(QColor(0, 255, 255))); // 完全不透明，颜色最深
            item->setFont(font);  // 应用更大的字体
        }
    }
}
