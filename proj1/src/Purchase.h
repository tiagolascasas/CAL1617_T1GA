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

bool operator==(const Purchase &p1, const Purchase &p2);

#endif /* PURCHASE_H_ */
