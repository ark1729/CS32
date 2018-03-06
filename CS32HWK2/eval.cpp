#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;


bool pref(char top, char push);
bool number(char c);
bool eval(string pfix, const bool value[], bool& result);

int evaluate(string infix, const bool values[], string& postfix, bool& result);
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	if (infix.length() == 0)
		return 1; 

	string nospace;
	for (int i = 0; i < infix.size(); i++)
	{
		if (infix[i] != ' ')
			nospace += infix[i];
	}

	postfix = "";
	stack<char> oper;
	stack<char> check; //infix comparing easier to compare in stack than array 


	for (int k = 0; k < nospace.size(); k++)
	{
		char c = nospace[k];

		if (number(c))
		{
			if (!check.empty())
			{
				if (number(check.top()))  //check number follows number
					return 1;
			}
			postfix += c;
			check.push(c);
		}
		
		else if (c == '(')
		{
			if (!check.empty())
			{
				if (number(check.top())) //number must have operator after it
					return 1;
			}
			oper.push(c);
			check.push(c);
		}

		else if (c == ')')
		{
			if (!check.empty())
			{
				if (check.top() == '(')
					return 1;
			}
			
			while (oper.top() != '(' && !oper.empty())
			{
				postfix += oper.top();
				oper.pop();
			}
			if (oper.empty()) //checks matching and if first 
				return 1;
			oper.pop();
			check.push(c);
		}

		else if (c == '&' || c == '|')
		{
			if (postfix.length() == 0) //checks operator is first
				return 1;
			if (!oper.empty() && oper.top() != '(' && pref(oper.top(), c) && oper.top() != '!')
			{
				postfix += oper.top();
				oper.pop();
				oper.push(c);
				check.push(c);
			}

			else if (!oper.empty() && oper.top() == '!')
			{
				while (oper.top() == '!')
				{
					postfix += oper.top();
					oper.pop();
				}
				oper.push(c);
				check.push(c);
			}
			else
			{
				oper.push(c);
				check.push(c);
			}
		}

		else if (c == '!')
		{
			oper.push(c);
			check.push(c);
		}
		else
			return 1;
	}

	while (!oper.empty())
	{
		postfix += oper.top();
		oper.pop();
	}

	if (!eval(postfix, values, result))
		return 1;

	return 0;
}

bool eval(string pfix, const bool value[], bool& result)
{
	stack<bool> operand;
	for (int i = 0; i < pfix.size(); i++)
	{
		char c = pfix[i];

		if (number(c))
		{
			int num = c - 48;
			bool op = value[num];
			operand.push(op);
		}

		else if (c == '!')
		{
			if (operand.empty())
				return false;
			bool not = !(operand.top());
			operand.pop();
			operand.push(not);
		}

		else
		{
			if (operand.empty())
				return false;
			bool operand_l = operand.top();
			operand.pop();
			if (operand.empty())
				return false;
			bool operand_r = operand.top();
			operand.pop();

			bool tempresult;

			switch (c)
			{
			case '&':
				tempresult = operand_l && operand_r;
				break;
			case'|':
				tempresult = operand_l || operand_r;
				break;
			}

			operand.push(tempresult);
		}
	}

	result = operand.top();
	return true;
}

bool number(char c)
{
	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;
	default:
		return false;

	}

}

bool pref(char top, char push) //preference true is higher or equal
{
	switch (top)
	{
	case '|':
		if (push == '&')
			return false;
	case '&':
		if (push == '|')
			return false;
	case '!':
		return true;
	default:
		return true;
	}
}




int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true, true, true, false, false, false, true, false, true, false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|"  &&  !answer);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9"  &&  !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  !answer);
	cout << "Passed all tests" << endl;
}