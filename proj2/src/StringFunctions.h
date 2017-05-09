#ifndef STRINGFUNCTIONS_H_
#define STRINGFUNCTIONS_H_

#include <string>
#include <queue>
using namespace std;

/**
 * This class holds a string and the "closeness" of that string
 * to other. It is simply used to provide a method to sort strings
 * based on their closeness to a predetermined pattern
 */
class ApproxString
{
private:
	string s;		/// the string
	int closeness;	/// the closeness of the string to the pattern

public:
	/**
	 * Constructor for an ApproxString object
	 * @param s the string
	 * @param closeness the closeness of the string to the pattern
	 */
	ApproxString(string s, int closeness);

	/**
	 * Returns the string
	 * @return the string
	 */
	string getString() const;
	/**
	 * Returns the closeness of the string
	 * @return the closeness of the string
	 */
	int getCloseness() const;

	/**
	 * Compares two ApproxString objects using the < operator
	 * @param as the object to compare to
	 * @return true if closeness of "this" < closeness of "as", false otherwise
	 */
	bool operator<(const ApproxString as) const;
};

/**
 * Removes all whitespaces at the begginning and
 * the ending of a string
 * @param s the string to be trimmed (passed by reference)
 */
string trim(string &s);

/**
 * Checks if a certain pattern can be found in a
 * certain text using the Knuth-Morris-Pratt algorithm
 * @param text the text to analyze
 * @param pattern the pattern to search for
 * @param caseSensitive true to consider case sensitiveness, false to not consider
 * @return true if the pattern was found, false otherwise
 */
bool kmpStringMatching(string &text, string pattern, bool caseSensitive);

/**
 * Scans a set of strings, ordering them by resemblance to a certain pattern
 * @param text the set of strings
 * @param pattern the pattern to compare to
 * @param caseSensitive true to consider case sensitiveness, false to not consider
 * @return a priority queue with the strings, with the most approximate one at the head
 */
priority_queue<ApproxString> approximateStringMatching(vector<string> &text, string pattern, bool caseSensitive);

#endif /* STRINGFUNCTIONS_H_ */
