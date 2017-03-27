#include "RoadNode.h"
#include <sstream>

RoadNode::RoadNode(int id, float degLat, float radLat, float degLong, float radLong)
{
	this->id = id;
	this->degLat = degLat;
	this->radLat = radLat;
	this->degLong = degLong;
	this->radLong = radLong;
}

int RoadNode::getID() const
{
	return id;
}

string RoadNode::getDegLocation() const
{
	ostringstream ss;
	ss << '(' << degLat << ", " << degLong << ')';
	return ss.str();
}

string RoadNode::getRadLocation() const
{
	ostringstream ss;
	ss << '(' << radLat << ", " << radLong << ')';
	return ss.str();
}

bool operator==(const RoadNode n1, const RoadNode n2)
{
	return (n1.getID() == n2.getID());
}
