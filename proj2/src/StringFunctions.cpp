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
	int t[];

	if(pattern.size()<2){
		t=new int[2];
		t[0]=-1;
		t[1]=0;
	}
	else{
		t=new int[pattern.size()];
		t[0]=-1;
		t[1]=0;
	}

	int pos=2;
	int cnd=0;

	while(pos<pattern.size()){
		if(caseSensitive){
			if(pattern[pos-1]==pattern[cnd]){
				t[pos]=cnd+1;
				cnd++;
				pos++;
			}
			else if(cnd>0){
				cnd=t[cnd];
			}
			else{
				t[pos]=0;
				pos++;
			}
		}
		else{
			if(tolower(pattern[pos-1])==tolower(pattern[cnd])){
				t[pos]=cnd+1;
				cnd++;
				pos++;
			}
			else if(cnd>0){
				cnd=t[cnd];
			}
			else{
				t[pos]=0;
				pos++;
			}
		}
	}


	///////////////////////////

	int i = 0;
	int m = 0;

	while(m+i<text.size()){
		if(caseSensitive){
			if(pattern[i]==text[m+i]){
				if(i==pattern.size()-1){
					return true;
				}
				else{
					i++;
				}
			}
			else{
				if(t[i]>-1){
					m=m+i-t[i];
					i=t[i];
				}
				else{
					m=m+i+1;
					i=0;
				}
			}
		}
		else{
			if(tolower(pattern[i])==tolower(text[m+i])){
				if(i==pattern.size()-1){
					return true;
				}
				else{
					i++;
				}
			}
			else{
				if(t[i]>-1){
					m=m+i-t[i];
					i=t[i];
				}
				else{
					m=m+i+1;
					i=0;
				}
			}

		}
	}

	return false;
}

priority_queue<ApproxString> approximateStringMatching(vector<string> &text, string pattern, bool caseSensitive)
{
	priority_queue<ApproxString> res;
	return res;
}



