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
	return closeness > as.getCloseness();
}

void trim(string &s)
{
	s.erase(0, s.find_first_not_of(' '));
	s.erase(s.find_last_not_of(' ') + 1);
}

string kmpStringMatching(string &text, string pattern, bool caseSensitive)
{
	int size;

	if(pattern.size()<2){
		size=2;
	}
	else{
		size=pattern.size();
	}

	int t[size];

	t[0]=-1;
	t[1]=0;

	int pos=2;
	int cnd=0;

	while(pos<pattern.size()){
		if(caseSensitive ? pattern[pos-1] == pattern[cnd] : tolower(pattern[pos-1]) == tolower(pattern[cnd]))
		{
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

	int i = 0;
	int m = 0;
	string result="";

	for(int i2 =0;i2< pattern.size();i2++){
		result=result+" ";
	}

	while(m+i<text.size()){
		if(caseSensitive ? pattern[i] == text[m+i] : tolower(pattern[i]) == tolower(text[m+i])){
			if(i==pattern.size()-1){
				result[i]=text[m+i];

				if(text[m+i+2]==' ' && text[m+i+1]==' '){
				return result;
				}
				else{
					return "";
				}
			}
			else{
				result[i]=text[m+i];
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
	return "";
}

priority_queue<ApproxString> approximateStringMatching(vector<string> &text, string pattern, bool caseSensitive)
{
	priority_queue<ApproxString> res;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		res.push(ApproxString(text[i], levenshtein_distance(text[i], pattern, caseSensitive)));
		if (res.top().getCloseness() == 0)
			return res;
	}
	return res;
}

int levenshtein_distance(const string &t, const string &p, bool caseSensitive)
{
	int textSize = t.size(), patternSize = p.size();
	int last_diagonal, old_diagonal, substitutionCost;
	bool aux;


	int* d = new int[textSize + 1];
	iota(d + 1, d + textSize + 1, 1);

	for (int i = 1; i <= patternSize; i++) {
		d[0] = i;
		last_diagonal = i - 1;
		for (int j = 1; j <= textSize; j++) {
			if(caseSensitive)
				aux=(t[j - 1] == p[i - 1]);
			else
				aux=(tolower(t[j - 1]) == tolower(p[i - 1]));

			if(aux) substitutionCost=0;
			else 	substitutionCost=1;

			old_diagonal = d[j];
			d[j] = min(d[j] + 1,min(d[j - 1] + 1,
					last_diagonal + substitutionCost));
			last_diagonal = old_diagonal;
		}
	}

	int result = d[textSize];
	delete[] d;
	return result;
}



