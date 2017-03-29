#include "RoadNode.h"
#include <sstream>
#include <cmath>
#include <iostream>

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

float RoadNode::getRadLong() const
{
	return radLong;
}

float RoadNode::getRadLat() const
{
	return radLat;
}

int RoadNode::getDistanceBetween(const RoadNode &n) const
{
	//http://andrew.hedges.name/experiments/haversine/
	float dLong = n.getRadLong() - radLong;
	float dLat = n.getRadLat() - radLat;
	float a = pow((sin(dLat / 2)), 2) + cos(radLat) * cos(n.getRadLat()) * pow(sin(dLong / 2), 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	float d = RADIUS * c;
	return static_cast<int>(d);
}

bool operator==(const RoadNode n1, const RoadNode n2)
{
	return (n1.getID() == n2.getID());
}

ostream& operator<<(ostream &out, const RoadNode n)
{
	out << "node " << n.getID() << " at coordinate " << n.getDegLocation();
	return out;
}

