#ifndef SOME_OF_FOUR_H
#define SOME_OF_FOUR_H

#include "question_base.h"

#include <vector>

class Some_of_four : public Question_base
{
public:
    Some_of_four() = default;

    void input(QTextStream& file_stream) override;

    std::vector<int> get_current_answer() const {return current_answer;}
    void set_current_answer(std::vector<int> i) {this->current_answer = i;}

    const std::vector<int>& get_correct_answer(){return correct_answer;}
    const std::array<QString, 4>& get_answers(){return answers;}

private:
    std::array<QString, 4> answers;
    std::vector<int> correct_answer;
    std::vector<int> current_answer;
};

#endif // SOME_OF_FOUR_H
