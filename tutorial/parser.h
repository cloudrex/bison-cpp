#pragma once

#include <map>

std::map<char, int> BinopPrecedence;

int getNextToken();
void MainLoop();