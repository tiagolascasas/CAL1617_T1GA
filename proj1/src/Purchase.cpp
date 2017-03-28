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
