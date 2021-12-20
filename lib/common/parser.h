#ifndef PARSER
#define PARSER

#include <vector>

#include "point.h"

std::vector<Point *> parseinputfile(std::string, int &d, int &n);
struct Config parseConfig(std::string);

#endif