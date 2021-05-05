#include "test_widget.h"
#include "ui_test_widget.h"

Test_widget::Test_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Test_widget)
{
    ui->setupUi(this);
    this->back.push_back(this->ui->back);
    this->back.push_back(this->ui->back2);
    this->result_done = this->ui->to_show;

    this->time_line = new QTimer(this);
    this->ui->progress_bar->setRange(0, seconds);
    this->ui->to_show->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->type_answers->setCurrentIndex(int(state_type::start));
    ui->result_stacked_widget->setCurrentIndex(0);
}

Test_widget::~Test_widget()
{
    delete ui;
}

void Test_widget::enable_buttons(bool enable)
{
    ui->progress_bar->setEnabled(enable);
    ui->next_button->setEnabled(enable);
    ui->prev_button->setEnabled(enable);
    ui->check_button->setEnabled(enable);
    ui->done->setEnabled(enable);
}

void Test_widget::fill_question()
{
    this->ui->question->setText(this->test.get_question());
    this->ui->question_2->setText(this->test.get_question());
    this->ui->question_3->setText(this->test.get_question());
    this->ui->question_4->setText(this->test.get_question());
    /*clear*/
    this->ui->answer->setText("");
    bool is_closed{this->test.is_current_question_closed()};

    std::array<QString, 4> arr4;
    switch (this->test.get_current_type())
    {
    case test_type::one_of_four:
        this->ui->answer1->setChecked(false);
        this->ui->answer2->setChecked(false);
        this->ui->answer3->setChecked(false);
        this->ui->answer4->setChecked(false);
        ui->answer1->setText(this->test.get_answers_1()[0]);
        ui->answer2->setText(this->test.get_answers_1()[1]);
        ui->answer3->setText(this->test.get_answers_1()[2]);
        ui->answer4->setText(this->test.get_answers_1()[3]);
        if (is_closed)
        {
            switch (this->test.get_closed_answer1())
            {
            case 0:
                ui->answer1->setChecked(true);
                break;
            case 1:
                ui->answer2->setChecked(true);
                break;
            case 2:
                ui->answer3->setChecked(true);
                break;
            case 3:
                ui->answer4->setChecked(true);
                break;
            }
        }
        break;
    case test_type::some_of_four:
        this->ui->answer1_2->setChecked(false);
        this->ui->answer2_2->setChecked(false);
        this->ui->answer3_2->setChecked(false);
        this->ui->answer4_2->setChecked(false);
        ui->answer1_2->setText(this->test.get_answers_2()[0]);
        ui->answer2_2->setText(this->test.get_answers_2()[1]);
        ui->answer3_2->setText(this->test.get_answers_2()[2]);
        ui->answer4_2->setText(this->test.get_answers_2()[3]);
        if (is_closed)
        {
            for (size_t i = 0; i < this->test.get_closed_answer2().size(); i++)
            {
                switch (this->test.get_closed_answer2()[i])
                {
                case 0:
                    ui->answer1_2->setChecked(true);
                    break;
                case 1:
                    ui->answer2_2->setChecked(true);
                    break;
                case 2:
                    ui->answer3_2->setChecked(true);
                    break;
                case 3:
                    ui->answer4_2->setChecked(true);
                    break;
                }
            }
        }
        break;
    case test_type::write_answer:
        if (is_closed)
            ui->answer->setText(this->test.get_closed_answer3());
        break;
    case test_type::installation_of_correspondence:
        this->ui->combo1->clear();
        this->ui->combo2->clear();
        this->ui->combo3->clear();
        this->ui->combo4->clear();
        ui->text1->setText(this->test.get_answers_4().first[0]);
        ui->text2->setText(this->test.get_answers_4().first[1]);
        ui->text3->setText(this->test.get_answers_4().first[2]);
        ui->text4->setText(this->test.get_answers_4().first[3]);
        arr4 = this->test.get_answers_4().second;
        for (size_t i = 0; i < 4; i++)
        {
            ui->combo1->addItem(arr4[i]);
            ui->combo2->addItem(arr4[i]);
            ui->combo3->addItem(arr4[i]);
            ui->combo4->addItem(arr4[i]);
        }
        if (is_closed)
        {
            ui->combo1->setCurrentText(this->test.get_closed_answer4()[0]);
            ui->combo2->setCurrentText(this->test.get_closed_answer4()[1]);
            ui->combo3->setCurrentText(this->test.get_closed_answer4()[2]);
            ui->combo4->setCurrentText(this->test.get_closed_answer4()[3]);
        }
        break;
    }
}

void Test_widget::clear()
{
    size_t i{};
    for (i = 0; i < this->questions.size(); i++)
    {
        delete this->questions[i];
        delete this->results[i];
        delete this->lines[i];
    }
    if (i)
        delete this->lines[i];

    delete this->result;
    this->questions.clear();
    this->results.clear();
    this->lines.clear();
}

void Test_widget::update_progress_bar()
{
    if (this->ui->progress_bar->value() >= seconds)
    {
        ui->tabWidget->setCurrentIndex(2);
        this->time_line->stop();
        this->ui->progress_bar->setValue(0);
        this->enable_buttons(false);
        this->ui->to_show->setEnabled(true);
        ui->type_answers->setCurrentIndex(int(state_type::start));
    }
    else
    {
        this->ui->progress_bar->setValue(this->ui->progress_bar->value() + 1);
    }
}

void Test_widget::init(QString filename)
{
    this->clear();
    this->test.init(filename);

    /*results*/
    size_t i{};
    for (i = 0; i < this->test.get_count_questions(); i++)
    {
        this->questions.push_back(new QLabel(this));
        this->results.push_back(new QLabel(this));
        this->lines.push_back(new QFrame());
        this->lines[i]->setFrameShape(QFrame::HLine);
    }
    this->lines.push_back(new QFrame());
    this->lines[i]->setFrameShape(QFrame::HLine);
    this->result = new QLabel(this);

    this->ui->progress_bar->setRange(0, seconds);
    this->ui->to_show->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->type_answers->setCurrentIndex(int(state_type::start));
    ui->result_stacked_widget->setCurrentIndex(0);

    this->ui->label->setText("Вам предложено пройти тест, состоящий из " + QString::number(this->test.get_count_questions()) + " вопросов. Время на прохождение: 10 минут. По окончании тестирования можно просмотреть свой результат в 3-ей вкладке. Вопросы в тесте могут быть четырех видов: выбор одного ответа из четырех, выбор нескольких ответов из четырех, вписать ответ на вопрос, установить соответствие.");
    this->ui->label_2->setText("Вы готовы к тестированию по теме \"" + this->pattern + "\"? Вам будет дано 10 минут.");
}

void Test_widget::stop_timer()
{
    this->time_line->stop();
    this->ui->progress_bar->setValue(0);
}

void Test_widget::on_start_testing_clicked()
{
    this->enable_buttons(true);
    this->ui->to_show->setEnabled(false);
    this->ui->result_stacked_widget->setCurrentIndex(0);
    this->time_line->start(1000);
    connect(time_line, &QTimer::timeout, this, &Test_widget::update_progress_bar);

    this->test.reset();
    this->ui->type_answers->setCurrentIndex(int(state_type(int(this->test.get_current_type()))));
    this->fill_question();
    this->ui->prev_button->setEnabled(false); /*выключено, потому что назад нельзя*/

    if (this->test.get_count_questions() <= 1)
        this->ui->next_button->setEnabled(false);
}

void Test_widget::on_to_test_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Test_widget::on_next_button_clicked()
{
    this->ui->prev_button->setEnabled(true);
    this->test.set_current_index(this->test.get_current_index() + 1);

    if (this->test.get_current_index() == this->test.get_count_questions() - 1)
        this->ui->next_button->setEnabled(false);

    this->ui->type_answers->setCurrentIndex(int(this->test.get_current_type()));
    this->fill_question();
}

void Test_widget::on_prev_button_clicked()
{
    this->ui->next_button->setEnabled(true);
    this->test.set_current_index(this->test.get_current_index() - 1);

    if (this->test.get_current_index() == 0)
        this->ui->prev_button->setEnabled(false); /*выключенно, потому что назад нельззя*/

    this->ui->type_answers->setCurrentIndex(int(this->test.get_current_type()));
    this->fill_question();
}

void Test_widget::on_check_button_clicked()
{
    std::vector<int> arr2;
    std::array<QString, 4> arr4;

    switch (this->test.get_current_type())
    {
    case test_type::one_of_four:
        if (this->ui->answer1->isChecked())
            this->test.check_answer1(0);
        if (this->ui->answer2->isChecked())
            this->test.check_answer1(1);
        if (this->ui->answer3->isChecked())
            this->test.check_answer1(2);
        if (this->ui->answer4->isChecked())
            this->test.check_answer1(3);
        break;
    case test_type::some_of_four:
        if (this->ui->answer1_2->isChecked())
            arr2.push_back(0);
        if (this->ui->answer2_2->isChecked())
            arr2.push_back(1);
        if (this->ui->answer3_2->isChecked())
            arr2.push_back(2);
        if (this->ui->answer4_2->isChecked())
            arr2.push_back(3);
        if (!arr2.empty())
            this->test.check_answer2(arr2);
        break;
    case test_type::write_answer:
        if (this->ui->answer->text().size() != 0)
            this->test.check_answer3(this->ui->answer->text());
        break;
    case test_type::installation_of_correspondence:
        arr4[0] = this->ui->combo1->currentText();
        arr4[1] = this->ui->combo2->currentText();
        arr4[2] = this->ui->combo3->currentText();
        arr4[3] = this->ui->combo4->currentText();
        this->test.check_answer4(arr4);
        break;
    }
}

void Test_widget::on_answer1_clicked()
{
    this->ui->answer2->setChecked(false);
    this->ui->answer3->setChecked(false);
    this->ui->answer4->setChecked(false);
}

void Test_widget::on_answer2_clicked()
{
    this->ui->answer1->setChecked(false);
    this->ui->answer3->setChecked(false);
    this->ui->answer4->setChecked(false);
}

void Test_widget::on_answer3_clicked()
{
    this->ui->answer2->setChecked(false);
    this->ui->answer1->setChecked(false);
    this->ui->answer4->setChecked(false);
}

void Test_widget::on_answer4_clicked()
{
    this->ui->answer2->setChecked(false);
    this->ui->answer3->setChecked(false);
    this->ui->answer1->setChecked(false);
}

void Test_widget::on_to_show_clicked()
{
    /*заполнение данных о прохождении теста*/
    this->result->setText("Ваш результат прохождения теста: "
                            + QString::number(this->test.get_counter_correct_answers()) + "/"
                            + QString::number(this->test.get_count_questions()));
    this->ui->layout_show->addWidget(this->result);
    this->ui->layout_show->addWidget(this->lines[0]);

    this->test.set_current_index(0);
    for (size_t i = 0; i < this->test.get_count_questions() ; i++ )
    {
        this->questions[i]->setText("Вопрос: " + this->test.get_question());

        QString result_tmp{QString("Вы ответили: ") + (this->test.is_current_question_correct() ? "правильно" : "неправильно") + "\n"};
        switch (this->test.get_current_type())
        {
        case test_type::one_of_four:
            result_tmp += "Ваш ответ: ";
            if (this->test.is_current_question_closed())
            {
                result_tmp += this->test.get_answers_1()[this->test.get_closed_answer1()] + "\n";
            }
            else
            {
                result_tmp += "нет ответа\n";
            }
            result_tmp += "Правильный ответ: " + this->test.get_answers_1()[this->test.get_correct_answer1()] + "\n";
            break;
        case test_type::some_of_four:
            result_tmp += "Ваш ответ: ";
            if (this->test.is_current_question_closed())
            {
                for (size_t j = 0; j < this->test.get_closed_answer2().size(); j++)
                {
                    result_tmp += this->test.get_answers_2()[this->test.get_closed_answer2()[j]] + (j != this->test.get_closed_answer2().size() - 1 ? ", " : "\n");
                }
            }
            else
            {
                result_tmp += "нет ответа\n";
            }
            result_tmp += "Правильный ответ: ";
            for (size_t j = 0; j < this->test.get_correct_answer2().size(); j++)
            {
                result_tmp += this->test.get_answers_2()[this->test.get_correct_answer2()[j]] + (j != this->test.get_correct_answer2().size() - 1 ? ", " : "\n");
            }
            break;
        case test_type::write_answer:
            result_tmp += "Ваш ответ: ";
            if (this->test.is_current_question_closed())
            {
                result_tmp += this->test.get_closed_answer3() + "\n";
            }
            else
            {
                result_tmp += "нет ответа\n";
            }
            result_tmp += "Правильный ответ: " + this->test.get_correct_answer3() + "\n";
            break;
        case test_type::installation_of_correspondence:
            result_tmp += "Ваш ответ: ";
            if (this->test.is_current_question_closed())
            {
                for (size_t j = 0; j < this->test.get_closed_answer4().size(); j++)
                {
                    result_tmp += this->test.get_answers_4().first[j] + ":" + this->test.get_closed_answer4()[j] + (j != this->test.get_closed_answer4().size() - 1 ? ", " : "\n");
                }
            }
            else
            {
                result_tmp += "нет ответа\n";
            }
            result_tmp += "Правильный ответ: ";
            for (size_t j = 0; j < this->test.get_correct_answer4().size(); j++)
            {
                result_tmp += this->test.get_answers_4().first[j] + ":" + this->test.get_correct_answer4()[j] + (j != this->test.get_correct_answer4().size() - 1 ? ", " : "\n");
            }
            break;
        }

        this->results[i]->setText(result_tmp);
        this->results[i]->setWordWrap(true);
        this->questions[i]->setWordWrap(true);
        this->ui->layout_show->addWidget(this->questions[i]);
        this->ui->layout_show->addWidget(this->results[i]);
        this->ui->layout_show->addWidget(this->lines[i+1]);

        this->test.set_current_index(i+1);
    }

    this->ui->result_stacked_widget->setCurrentIndex(1);
}

void Test_widget::on_done_clicked()
{
    /*test done*/
    this->ui->to_show->setEnabled(true);
    this->ui->tabWidget->setCurrentIndex(2);
    this->enable_buttons(false);
    this->time_line->stop();
    this->ui->progress_bar->setValue(0);
    this->ui->type_answers->setCurrentIndex(int(state_type::start));

    this->count_correct = this->test.get_counter_correct_answers();
}

void Test_widget::on_back_to_show_clicked()
{
    this->ui->result_stacked_widget->setCurrentIndex(0);
}
