#include "one_of_four.h"

void One_of_four::input(QTextStream& file_stream)
{
    this->question = file_stream.readLine();
    for (size_t i = 0; i < 4; i++)
    {
        this->answers[i] = file_stream.readLine();
    }
    this->correct_answer = file_stream.readLine().toULongLong();
}
