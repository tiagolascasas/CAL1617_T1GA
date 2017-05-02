/*
 * Utils.h
 *
 *  Created on: 02/05/2017
 *      Author: up201503616
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
using namespace std;

inline std::string trim(string &str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ')+1);
	return str;
}

// algoritmos de string aqui

#endif /* UTILS_H_ */
