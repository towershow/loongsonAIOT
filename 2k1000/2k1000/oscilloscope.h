#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H
#include<QMediaPlayer>
#include <QMainWindow>
class QToolButton;
class QFrame;
namespace Ui {
class Oscilloscope;
}

class Oscilloscope : public QMainWindow
{
    Q_OBJECT

public:
    explicit Oscilloscope(QWidget *parent = nullptr);
    ~Oscilloscope();
    void initial_ui();
    void setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_toolButton_mainWindow_clicked();

    void on_toolButton_CAN_clicked();

    void on_toolButton_Record_clicked();

    void on_toolButton_deviceStatus_clicked();

    void on_pushButton1_clicked();

    void on_pushButton3_clicked();

    void on_pushButton4_clicked();

    void on_pushButton2_clicked();

signals:
    void back_mainWindow();
    void back_CANWindow();
    void back_recordWindow();
    void back_devstatusWindow();
private:
    Ui::Oscilloscope *ui;
    QMediaPlayer  *player;

};

#endif // OSCILLOSCOPE_H
