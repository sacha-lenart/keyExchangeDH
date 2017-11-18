#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <QString>
#include <QObject>
#include <QJSEngine>

#include "Matrix.hpp"

Matrix parser(string inputFile, Matrix M);
void replaceAll(std::string& str, const std::string& from, const std::string& to) ;
int moduloInt(int a, int modulo);

#endif // PARSER_HPP
