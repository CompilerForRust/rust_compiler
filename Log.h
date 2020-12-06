#pragma once
#include <fstream>
#include <iostream>
#include "Token.h"

#define LOG_PATH "log.txt"

using namespace std;

//创建一个新的日志
void create_new_log();

//输出到日志文件
void cout_log(string message);
void cout_log(token_type type);

//输出一个换行符
void endl_log();
