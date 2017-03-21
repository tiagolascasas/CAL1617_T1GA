#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <string>

class Purchase
{
private:
	string addr;
public:
	Purchase(string addr);
	string getAddr() const;
	void setAddr(string address);
};

#endif /* PURCHASE_H_ */
