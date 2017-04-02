#include "Purchase.h"

Purchase::Purchase(RoadNode &address): addr(address){};

RoadNode Purchase::getAddr() const
{
	return addr;
}
//
void Purchase::setAddr(RoadNode &address)
{
	addr = address;
}

vector<RoadNode> Purchase::getValidMarkets() const
{
	return validMarkets;
}

void Purchase::addValidMarket(RoadNode &market)
{
	validMarkets.push_back(market);
}

bool operator==(const Purchase &p1, const Purchase &p2)
{
	return (p1.getAddr() == p2.getAddr());
}
