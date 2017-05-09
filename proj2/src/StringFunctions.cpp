#include "StringFunctions.h"

ApproxString::ApproxString(string s, int closeness): s(s), closeness(closeness){};

string ApproxString::getString() const
{
	return s;
}

int ApproxString::getCloseness() const
{
	return closeness;
}


bool ApproxString::operator<(const ApproxString as) const
{
	return closeness < as.getCloseness();
}

string trim(string &s)
{
	s.erase(0, s.find_first_not_of(' '));
	s.erase(s.find_last_not_of(' ') + 1);
}

bool kmpStringMatching(string &text, string pattern, bool caseSensitive)
{
	return false;
}

priority_queue<ApproxString> approximateStringMatching(vector<string> &text, string pattern, bool caseSensitive)
{
	priority_queue<ApproxString> res;
	return res;
}



