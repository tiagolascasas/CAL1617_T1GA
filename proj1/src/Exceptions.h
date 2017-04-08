#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <string>
using namespace std;

class FileNotFound
{
public:
	string filename;	/// Name of the file the program unsuccessfully tried to open
	 /**
	  * Creates an instance of FileNotFound used for exception throwing
	  * @param filename name of the file the program unsuccessfully tried to open
	  */
	FileNotFound(string filename): filename(filename){};
};



#endif /* EXCEPTIONS_H_ */
