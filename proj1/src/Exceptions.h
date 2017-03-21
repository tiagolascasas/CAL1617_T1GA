#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <string>
using namespace std;

class FileNotFound
{
public:
	string filename;
	FileNotFound(string filename): filename(filename){};
};



#endif /* EXCEPTIONS_H_ */
