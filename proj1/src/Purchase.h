#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <string>
#include <vector>
#include "RoadNode.h"

class Purchase
{
private:
	string addr;
	vector<RoadNode> validMarkets;
public:
	Purchase(string addr);
	string getAddr() const;
	void setAddr(string address);
	vector<RoadNode> getValidMarkets();
	void addValidMarket(RoadNode market);
};

#endif /* PURCHASE_H_ */
