#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTcpSocket>
#include <QFileDialog>
#include <QtSql>
#include <QPixmap>
#include <QDir>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>

#include <memory>
#include <vector>
#include <fstream>

#include "tinyxml2.h"
#include "popup.h"
#include "base64.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct user_info_t
{
    QString login;
    QString password;
    QString path_to_image;
};

struct result_test_t
{
    QString name;
    QString result;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_login_2_clicked();

    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   ();

    void on_to_register_clicked();

    void on_to_login_clicked();

    void on_register_2_clicked();

    void on_reg_img_clicked();

    void on_quit_clicked();

    void on_to_result_test_clicked();

    void on_to_personal_area_clicked();

    void on_load_result_clicked();

private:
    enum class screen
    {
        login = 0,
        registration = 1,
        personal_area = 2,
        patterns = 3,
        result_test = 4
    };
    enum class type_forward
    {
        login,
        registration,
        load_result,   /*загрузка результатов тестов с сервера*/
        upload_result, /*отгрузка данных с клиента на сервер*/
    };

    Ui::MainWindow *ui;
    Popup *popup;
    QPixmap pic;
    QSqlDatabase db;
    type_forward forward;
    QString base64_file;
    QString file_type;

    std::vector<QLabel*> results_test;
    std::vector<QFrame*> lines;

    std::unique_ptr<QTcpSocket> socket;
    const QString host{"localhost"};
    const quint16 port{20002};

    void send(const QString& data);
    QString recv();
    void create_db();

    void send_auth(const QString& login, const QString& password);

    void fill_db_login(tinyxml2::XMLElement* body);
    void fill_db_register();
    void fill_personal_area();
    void fill_result_test(tinyxml2::XMLElement* body);

    void fill_result_test_form();

    void save_img_to_file(const QString& path, const QString& img);

    /*выборка из базы данных*/
    user_info_t get_user_info();
    std::vector<result_test_t> get_result_test_info();
};
#endif // MAINWINDOW_H
