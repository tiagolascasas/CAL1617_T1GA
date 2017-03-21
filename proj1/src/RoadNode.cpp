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

int RoadNode::getID()
{
	return id;
}

string RoadNode::getDegLocation()
{
	ostringstream ss;
	ss << '(' << degLat << ", " << degLong << ')';
	return ss.str();
}

string RoadNode::getRadLocation()
{
	ostringstream ss;
	ss << '(' << radLat << ", " << radLong << ')';
	return ss.str();
}
