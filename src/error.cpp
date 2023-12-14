#include "error.h"
#include <iostream>

#include <string>
void errif(bool condition, const std::string err_msg){
	if(condition){
		perror(err_msg.c_str());
		exit(EXIT_FAILURE);
	}
}
