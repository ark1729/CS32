// We have not yet produced the test driver main routine for you.

#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;




int main()
{
	cout << "Test driver not yet written." << endl;
	// string text;
	// if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
	//	cout << "Error hiding!" << endl;
	// string msg;
	// if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
	// 	cout << "Error revealing!" << endl;
	vector<unsigned short> v;
	v.push_back(1);
	v.push_back(5);
	v.push_back(9);
	// We use the :: operator below because encode() is a static
	// member function. You don’t create a BinaryConverter object
	// to use encode(); instead, you call it using the class name.
	string hiddenMessage = BinaryConverter::encode(v);
	cout << hiddenMessage; // prints tabs and spaces
	cerr << endl << "here" << endl;
	vector<unsigned short> v1;
	// Using the -/_ representation in this comment, the string
	// below contains _______________-_____________-_-
	string msg = "_______________-_____________-_-";
	if (BinaryConverter::decode(hiddenMessage, v1))
	{
		cout << "The vector has " << v1.size() << " numbers:";
		for (int k = 0; k != v1.size(); k++)
			cout << ' ' << v1[k];
		cout << endl;
	}
	else
		cout << "The string has a bad character or a bad length.";
	Compressor::compress("AAAAAAAAAB", v1);
	for (int k = 0; k != v1.size(); k++)
		cout << ' ' << v1[k];
	cout << endl;
	Compressor::decompress(v1, msg);
	cout << msg << endl;
	cout << BinaryConverter::encode(v1) << endl;
	string hostOut;
	string temp = "my e";
	string temp2 = "1";
	Steg::hide("<html>   \nQ	QQ	 \nBBB			   \n\nGG \nBBB \n	\nDDD\nEEE </html>   ", temp, hostOut);
	Steg::reveal(hostOut, temp2);
	cout << endl << temp << " " << temp2 << endl;
	if (temp2 == temp)
		cout << endl << "correct" << endl;
	else
		cout << "problem";
	/////////////////////////////////////////////////
	vector<unsigned short> v2;
	vector<unsigned short> v3;
	string msg1 = "ge";
	string msg2;
	Compressor::compress(msg1, v2);
	string encodemsg = BinaryConverter::encode(v2);
	BinaryConverter::decode(encodemsg, v3);
	Compressor::decompress(v3, msg2);
	cout << msg1 << endl;
	for (int k = 0; k != v2.size(); k++)
		cout << v2[k] << ' ';
	cout << endl;
	cout << encodemsg << endl;
	for (int k = 0; k != v3.size(); k++)
		cout << v3[k] << ' ';
	cout << endl;
	cout << msg2 << endl;
	string originalPageText =
		"<html>\n"
		"<head>\n"
		" <title>My Pretend Web Page</title>\n"
		"</head>\n"
		"<body>\n"
		"<h2>My Pretend Web Page<h1>\n"
		"<p>\n"
		"I wish I were creative enough to have something interesting \n"
		"to say here.  I'm not, though, so this is a boring page.\n"
		"</p>\n"
		"<p>\n"
		"Oh, well.\n"
		"</p>\n"
		"</body>\n"
		"</html>\n"
		;
	HTTP().set("http://boring.com", originalPageText);
	string plan =
		"Lefty and Mugsy enter the bank by the back door.\n"
		"Shorty waits out front in the getaway car.\n"
		"Don't let the teller trip the silent alarm.\n"
		;
	string newPageText;
	if (!WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
	{
		cout << "Error hiding!" << endl;
		return 1;
	}
	HTTP().set("http://boring.com", newPageText);
	string msg3;
	if (!WebSteg::revealMessageInPage("http://boring.com", msg3))
	{
		cout << "Error revealing!" << endl;
		return 1;
	}
	if (msg3 != plan)
	{
		cout << "Message not recovered properly:\n" << msg3;
		return 1;
	}
	cout << "Recovered text is what was hidden:\n" << msg3;
}
