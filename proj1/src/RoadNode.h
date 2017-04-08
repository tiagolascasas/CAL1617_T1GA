#ifndef ROADNODE_H_
#define ROADNODE_H_

#include <string>
using namespace std;

class RoadNode
{
private:
	long long id;								/// Node's id
	float degLat, radLat, degLong, radLong;		/// Geographical coordinates (longitude and latitude) in both degrees and radians
	const double RADIUS;						/// earth's radius in meters
public:
	/**
	 * Creates an instance of RoadNode setting all data members to 0 and radius to 6371e3
	 */
	RoadNode();

	/**
	 * Creates an instance of RoadNode setting radius to 6371e3
	 * @param id RoadNode's id
	 * @param degLat RoadNode's latitude in degrees
	 * @param radLat RoadNode's latitude in radians
	 * @param degLong RoadNode's longitude in degrees
	 * @param radLong RoadNode's longitude in radians
	 */
	RoadNode(long long id, float degLat, float radLat, float degLong, float radLong);

	/**
	 * Gets the RoadNode's id
	 * @return Current RoadNode's id
	 */
	long long getID() const;

	/**
	 * Gets the node's geographical location in degrees
	 * @return string with coordinates formated as (Latitude, Longitude)
	 */
	string getDegLocation() const;

	/**
	 * Gets the node's geographical location in radians
	 * @return string with coordinates formated as (Latitude, Longitude)
	 */
	string getRadLocation() const;

	/**
	 * Gets the node's Longitude in radians
	 * @return value stored in RoadNode::radLong
	 */
	float getRadLong() const;

	/**
	 * Gets the node's Latitude in radians
	 * @return value stored in RoadNode::radLat
	 */
	float getRadLat() const;

	/**
	 * Gets the node's Longitude in degrees
	 * @return value stored in RoadNode::degLong
	 */
	float getDegLong() const;

	/**
	 * Gets the node's Latitude in degrees
	 * @return value stored in RoadNode::degLat
	 */
	float getDegLat() const;

	/**
	 * Calculates distance between this RoadNode and the one given as a parameter
	 * @param n RoadNode to calculate distance from current node
	 * @return distance between the two nodes as an integer
	 */
	int getDistanceBetween(const RoadNode &n) const;

	/**
	 * Copy assignment operator overload
	 * @param n RoadNode to copy into
	 * @return RoadNode equal to the one received as a parameter
	 */
	RoadNode& operator=(RoadNode n) const;
};

/**
 * 'equal to' operator overload
 * @param n1 first RoadNode to compare
 * @param n2 second RoadNode to compare
 * @return true if the RoadNode are equals and false otherwise
 */
bool operator==(const RoadNode n1, const RoadNode n2);

/**
 * 'not equal to' operator overload
 * @param n1 first RoadNode to compare
 * @param n2 second RoadNode to compare
 * @return true if the RoadNode are not equals and false otherwise
 */
bool operator!=(const RoadNode n1, const RoadNode n2);

/**
 * Ostream operator overload
 * @param out ostream to write to
 * @param n node used in output
 * @return ostream with node information formatted as "node (node.id) at coordinate (longitude,latitude)"
 */
ostream& operator<<(ostream &out, const RoadNode n);

struct road_t
{
	string name;		/// road's name
	long long id;		/// road's id
	bool twoWay;		/// true if road is a two-way street, false otherwise
};

#endif /* ROADNODE_H_ */
