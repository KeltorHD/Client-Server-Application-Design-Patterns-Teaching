#ifndef test_widget_H
#define test_widget_H

#include "test.h"

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QFrame>
#include <QPushButton>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Test_widget; }
QT_END_NAMESPACE

enum class state_type
{
    start = 0,
    one_of_four = 1,
    some_of_four = 2,
    write_answer = 3,
    installation_of_correspondence = 4
};

class Test_widget : public QWidget
{
    Q_OBJECT

public:
    Test_widget(QWidget *parent = nullptr);
    ~Test_widget();

    void update_progress_bar();
    void init(QString filename);

    void stop_timer();

private slots:
    void on_start_testing_clicked();

    void on_to_test_clicked();

    void on_next_button_clicked();

    void on_prev_button_clicked();

    void on_check_button_clicked();

    void on_answer1_clicked();

    void on_answer2_clicked();

    void on_answer3_clicked();

    void on_answer4_clicked();

    void on_to_show_clicked();

    void on_done_clicked();

private:
    Ui::Test_widget *ui;
    QTimer* time_line;
    const int seconds{600};
    Test test;
    /*отображение текста*/
    QLabel* result;
    std::array<QLabel*, 10> questions;
    std::array<QLabel*, 10> results;
    std::array<QFrame*, 11> lines;

    void enable_buttons(bool enable);

    void fill_question();

public:
    std::vector<QPushButton*> back;
    QPushButton* result_done;
    int count_correct;
    QString pattern;
};
#endif // test_widget_H
