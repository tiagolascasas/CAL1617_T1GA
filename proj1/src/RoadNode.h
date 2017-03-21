#ifndef ROADNODE_H_
#define ROADNODE_H_

#include <string>
using namespace std;

class RoadNode
{
private:
	int id;
	float degLat, radLat, degLong, radLong;

public:
	RoadNode(int id, float degLat, float radLat, float degLong, float radLong);
	int getID();
	string getDegLocation();
	string getRadLocation();
};


#endif /* ROADNODE_H_ */
