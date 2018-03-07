#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	string temp;
	HTTP().get(url, temp);
	if(!(Steg::hide(temp, msg, host)))
		return false;  // This compiles, but may not be correct
	return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string temp;
	HTTP().get(url, temp);
	if (!(Steg::reveal(temp,msg)))
		return false;  // This compiles, but may not be correct
	return true;
}
