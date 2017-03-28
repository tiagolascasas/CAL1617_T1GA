#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <string>
#include <vector>
#include "RoadNode.h"

class Purchase
{
private:
	RoadNode addr;
	vector<RoadNode> validMarkets;
public:
	Purchase(RoadNode &address);
	RoadNode getAddr() const;
	void setAddr(RoadNode &address);
	vector<RoadNode> getValidMarkets() const;
	void addValidMarket(RoadNode &market);
};

#endif /* PURCHASE_H_ */
