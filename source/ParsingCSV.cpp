#include "ParsingCSV.hpp"

char DELIMETER = '\t';  //tab by default, unless you change it

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}
