#ifndef TEST_H
#define TEST_H

#include "one_of_four.h"
#include "some_of_four.h"
#include "write_answer.h"
#include "installation_of_correspondence.h"

#include <array>
#include <vector>
#include <QString>
#include <QFile>
#include <QDebug>

class Test
{
public:
    Test() = default;

    /*считывание из файл*/
    void init(QString filename);

    void reset();

    void set_current_index(size_t index) {this->current_index = index;}

    bool is_current_question_closed() const {return this->answers[this->current_index];}
    bool is_current_question_correct() const {return this->questions[this->current_index].second;}

    size_t get_counter_answers() const;
    size_t get_counter_correct_answers() const;
    const size_t& get_current_index() const {return current_index;}

    const test_type &get_current_type() const;
    const QString& get_question() const {return this->questions[this->current_index].first->get_question();}

    std::array<QString, 4> get_answers_1() const;
    std::array<QString, 4> get_answers_2() const;
    std::pair<std::array<QString, 4>, std::array<QString, 4>> get_answers_4() const;

    /*ответ на первый тип, номер ответа по счету*/
    void check_answer1(size_t answer);
    /*ответ на второй тип, да или нет, по счету*/
    void check_answer2(std::vector<int> answers);
    /*ответ на третий тип*/
    void check_answer3(QString answer);
    /*ответ на четвертый тип*/
    void check_answer4(std::array<QString, 4> answer);

    /*получение ответа, введенного пользователем ранее*/
    size_t                 get_closed_answer1() const;
    std::vector<int>       get_closed_answer2() const;
    QString                get_closed_answer3() const;
    std::array<QString, 4> get_closed_answer4() const;

    size_t                 get_correct_answer1() const;
    std::vector<int>       get_correct_answer2() const;
    QString                get_correct_answer3() const;
    std::array<QString, 4> get_correct_answer4() const;

private:
    size_t current_index{0};
    /*вопрос и его статус: правильно или нет*/
    std::array<std::pair<Question_base*, bool>, 10> questions{};
    /*есть ли ответ на вопрос*/
    std::array<bool, 10> answers;
};

#endif // TEST_H
