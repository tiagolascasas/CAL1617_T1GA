#ifndef PURCHASE_H_
#define PURCHASE_H_

#include <string>
#include <vector>
#include <utility>
#include "RoadNode.h"

class Purchase
{
private:
	RoadNode addr;					/// Client's address and delivery point
	vector<RoadNode> validMarkets;	/// Vector containing closest market to this client
	pair<int, int> closestMarket;	/// Closest market given by its index (in the Program::markets vector) and the distance to this client
public:
	/**
	 * Creates an instance of Purchase for a given client's address
	 * @param address client's address
	 */
	Purchase(RoadNode &address);

	/**
	 * Gets the delivery's address
	 * @return value saved in Purchase::addr
	 */
	RoadNode getAddr() const;

	/**
	 * Sets the clients address as the one given as parameter
	 * @param address RoadNode representing client's new address
	 */
	void setAddr(RoadNode &address);

	/**
	 * Gets all the markets that can reach this client
	 * @return vector of RoadNode with all the valid markets
	 */
	vector<RoadNode> getValidMarkets() const;

	/**
	 * Adds the given market to validMarkets vector
	 * @param market pointer to a RoadNode object representing a market
	 */
	void addValidMarket(RoadNode &market);

	/**
	 * Gets the index of closest market
	 * @return first member of the pair closestMarket
	 */
	int getClosestMarketIndex() const;

	/**
	 * Receives the info for a potential closest market and compares its distance against the current one's distance to the client
	 * @param index market's index in the Program::markets vector
	 * @param distance distance between this client and the given market
	 * @return true if potential market is indeed closer than previous one. Returns false otherwise
	 */
	bool setClosestMarketIndex(int index, int distance);
};
/**
 * Compares two purchases given that: two purchases are the same if the address is the same.
 * @param p1 first purchase to compare
 * @param p2 second purchase to compare
 * @return true if p1 and p2 are equals and false otherwise
 */
bool operator==(const Purchase &p1, const Purchase &p2);

#endif /* PURCHASE_H_ */
