#include "Log.h"

void create_new_log()
{
	ofstream log_file(LOG_PATH);
	if (log_file.is_open()) {
		log_file.close();

		cout << "New log file created successfully" << endl;
	}
}

void cout_log(string message)
{
	//ios::app   如果没有文件，那么生成空文件；如果有文件，那么在文件尾追加
	ofstream log_file(LOG_PATH, ios::app);
	if (log_file.is_open()) {
		log_file << message;

		log_file.close();
	}
}

void cout_log(token_type type)
{
	ofstream log_file(LOG_PATH, ios::app);
	if (log_file.is_open()) {
		log_file << (int)type;

		log_file.close();
	}
}

void endl_log()
{
	ofstream log_file(LOG_PATH, ios::app);
	if (log_file.is_open()) {
		log_file << endl;

		log_file.close();
	}
}