#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <string>
#include <vector>
#include <utility>
#include "RoadNode.h"

class Purchase
{
private:
	RoadNode addr;
	vector<RoadNode> validMarkets;
	pair<int, int> closestMarket;
public:
	Purchase(RoadNode &address);
	RoadNode getAddr() const;
	void setAddr(RoadNode &address);
	vector<RoadNode> getValidMarkets() const;
	void addValidMarket(RoadNode &market);
	int getClosestMarketIndex() const;
	bool setClosestMarketIndex(int index, int distance);
};

bool operator==(const Purchase &p1, const Purchase &p2);

#endif /* PURCHASE_H_ */
