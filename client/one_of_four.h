#ifndef ONE_OF_FOUR_H
#define ONE_OF_FOUR_H

#include "question_base.h"

#include <array>
#include <QDebug>

class One_of_four : public Question_base
{
public:
    One_of_four() = default;

    void input(QTextStream& file_stream) override;

    size_t get_current_answer() const {return current_answer;}
    void   set_current_answer(size_t i) {this->current_answer = i;}

    size_t get_correct_answer(){return correct_answer;}
    const std::array<QString, 4>& get_answers(){return answers;}

private:
    std::array<QString, 4> answers;
    size_t correct_answer;
    size_t current_answer;
};

#endif // ONE_OF_FOUR_H
