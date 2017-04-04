#include "Purchase.h"
#include <climits>

Purchase::Purchase(RoadNode &address): addr(address), closestMarket(pair<int, int>(-1, INT_MAX)){};

RoadNode Purchase::getAddr() const
{
	return addr;
}

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

int Purchase::getClosestMarketIndex() const
{
	return closestMarket.first;
}

bool Purchase::setClosestMarketIndex(int index, int distance)
{
	if (closestMarket.second > distance)
	{
		pair<int, int> p(index, distance);
		closestMarket = p;
		return true;
	}
	else
		return false;
}

bool operator==(const Purchase &p1, const Purchase &p2)
{
	return (p1.getAddr() == p2.getAddr());
}
