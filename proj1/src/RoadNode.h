#ifndef ROADNODE_H_
#define ROADNODE_H_

#include <string>
using namespace std;

class RoadNode
{
private://
	int id;
	float degLat, radLat, degLong, radLong;
	static const double RADIUS = 6371e3;
public:
	RoadNode(int id, float degLat, float radLat, float degLong, float radLong);
	int getID() const;
	string getDegLocation() const;
	string getRadLocation() const;
	float getRadLong() const;
	float getRadLat() const;
	int getDistanceBetween(const RoadNode &n) const;
};

bool operator==(const RoadNode n1, const RoadNode n2);

ostream& operator<<(ostream &out, const RoadNode n);

struct road_t
{
	string name;
	int id;
	bool twoWay;
};

#endif /* ROADNODE_H_ */
