/*
 * json_exception.h
 *
 *  Created on: Apr 16, 2018
 *      Author: kevindeoliveira
 */

#ifndef JSON_EXCEPTION_H_
#define JSON_EXCEPTION_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <ctime>
#include <string>

class JsonLog{
private:
	std::ofstream error_log;
	char* error_txt;

	time_t now;
	tm *ltm;
public:
	JsonLog(){
		now = time(0);
		ltm = localtime(&now);

		error_log.open("JSON_log.txt", std::ofstream::app);
		error_txt=nullptr;
		error_log.close();
	}
	JsonLog(char const* eT){
		now = time(0);
		ltm = localtime(&now);

		error_log.open("JSON_log.txt", std::ofstream::app);
		error_txt = new char[strlen(eT)];
		strcpy(error_txt, eT);
		error_log<<this->getTime()<<' '<<eT<<'\n';
		error_log.close();
	}
	char* getTxt(){
		return this->error_txt;
	}

	std::string getTime(){
		std::stringstream ss;
		std::string time;
		ss<<ltm->tm_mday<<"/"<<1 + ltm->tm_mon<<"/"<<1900+ltm->tm_year<<"-"<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<":";
		ss>>time;
		return time;
	}
};

class JsonException{
public:
		class FilePathException: public std::exception, JsonLog{
			const char* what() const throw(){
				JsonLog j1("file_path_error");
				return j1.getTxt();
			}
		};
		class OpenFileException: public std::exception,JsonLog{
			const char* what() const throw(){
				JsonLog j1("open_file_error");
				return j1.getTxt();
			}
		};
		class InvalidArgumentsException: public std::exception, JsonLog{
			const char* what() const throw(){
				JsonLog j1("invalid_argument_error");
				return j1.getTxt();
			}
		};

		class InvalidSortArgumentException:public std::exception, JsonLog{
			const char* what() const throw(){
				JsonLog j1("file_sorting_error");
				return j1.getTxt();
			}
		};
};


#endif /* JSON_EXCEPTION_H_ */

	//namespace JException
namespace JException{
	using FilePathException = JsonException::FilePathException;
	using InvalidArgumentsException = JsonException::InvalidArgumentsException;
	using InvalidSortArgumentException = JsonException::InvalidSortArgumentException;
	using OpenFileException = JsonException::OpenFileException;
}
