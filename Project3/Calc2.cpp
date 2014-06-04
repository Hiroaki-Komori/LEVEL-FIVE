#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#define _USE_MATH_DEFINES

using namespace std;
enum oplevel{ L0, L1, L2, L3, L4, L5 };
//enum optype{ NG, ADD, SUB, MUL ,DIV ,POW ,FACT ,OPEN,CLOSE, UNARY, BINARY};
enum chartype{ NONE, DIGIT, ALPHA, UNARY, BINARY, FACT, OPEN, CLOSE };
enum error{ OK, COMPLETE, UNCOMPLETED, INVALID_WORD, INVARID_OPERATOR, INVALID_FORM, CALC_ERROR };
struct op{
	char token;
	chartype type;
	oplevel level;
};

op add, sub, mul, mydiv, mypow, fact, open, close, posi, nega, none;

stack<double>numstack;
stack<op*>opstack;
op* current_op = &none;
chartype previous_char = NONE;
error e = OK;

void opset()
{
	add.token = '+';
	add.level = L1;
	add.type = BINARY;

	sub.token = '-';
	sub.level = L1;
	sub.type = BINARY;

	mul.token = '*';
	mul.level = L2;
	mul.type = BINARY;

	mydiv.token = '/';
	mydiv.level = L2;
	mydiv.type = BINARY;

	mypow.token = '^';
	mypow.level = L3;
	mypow.type = BINARY;

	fact.token = '!';
	fact.level = L4;
	fact.type = FACT;

	open.token = '(';
	open.level = L5;
	open.type = OPEN;

	close.token = ')';
	close.level = L0;
	close.type = CLOSE;

	posi.token = '+';
	posi.level = L2;
	posi.type = UNARY;

	nega.token = '-';
	nega.level = L2;
	nega.type = UNARY;

	none.level = L0;
	none.type = NONE;
}

error opcheck(char token, chartype *prechar, op* *p_op)
{
	if (token == add.token)	{

		if (*prechar == NONE || *prechar == OPEN)
		{
			*p_op = &posi;//符号
			*prechar = UNARY;
		}
		else{
			*p_op = &add;
			*prechar = BINARY;
		}
	}
	else if (token == sub.token)
	{
		if (*prechar == NONE || *prechar == OPEN)
		{
			*p_op = &nega;//符号
			*prechar = UNARY;
		}
		else {
			*p_op = &sub;
			*prechar = BINARY;
		}
	}
	else if (token == mul.token){
		*p_op = &mul;
		*prechar = BINARY;
	}
	else if (token == mydiv.token)
	{
		*p_op = &mydiv;
		*prechar = BINARY;
	}
	else if (token == mypow.token){
		*p_op = &mypow;
		*prechar = BINARY;
	}
	else if (token == fact.token) {
		*p_op = &fact;
		*prechar = UNARY;
	}
	else if (token == open.token) {
		*p_op = &open;
		*prechar = OPEN;
	}
	else if (token == close.token) {
		*p_op = &close;
		*prechar = CLOSE;
	}
	else return INVARID_OPERATOR;
	return OK;
}

error calc(op* opr)
{
	double num1 = 0;
	double num2 = 0;

	if (numstack.empty())return INVALID_FORM;//スタックが空
	else{
		num2 = numstack.top();
		numstack.pop();
	}

	if (opr->type == UNARY)
	{
		if (opr->token == '-')
		{
			numstack.push(-1 * num2);
			return OK;
		}
		else numstack.push(num2);
		return OK;
	}
	else if (opr->type == BINARY)
	{
		double ans = 0;
		if (numstack.empty())return INVALID_FORM;//スタックが空
		else{
			num1 = numstack.top();
			numstack.pop();
		}
		cout << endl;
		switch (opr->token)
		{
		case '+':
			cout << num1 << "+" << num2 << "=" << (num1 + num2) << endl;
			numstack.push(num1 + num2);
			return OK;
		case '-':
			cout << num1 << "-" << num2 << "=" << (num1 - num2) << endl;
			numstack.push(num1 - num2);
			return OK;
		case '*':
			cout << num1 << "*" << num2 << "=" << (num1 * num2) << endl;
			numstack.push(num1 * num2);
			return OK;
		case '/':
			cout << num1 << "/" << num2 << "=" << (num1 / num2) << endl;
			numstack.push(num1 / num2);
			return OK;
		case '^':
			ans = pow(num1, num2);
			cout << num1 << "^" << num2 << "=" << ans << endl;
			numstack.push(ans);
			return OK;
		default:
			numstack.push(num1);
			numstack.push(num2);
			return CALC_ERROR;
		}
	}
}

error compare(op* op2)
{
	//スタックの先頭の優先度がスタック後より低くなるまで繰り返し比較→計算→演算子を消す
	while (!opstack.empty() && opstack.top()->level >= op2->level && opstack.top()->type != OPEN)
	{
		calc(opstack.top());
		opstack.pop();
	}
	if (!opstack.empty() && opstack.top()->type == OPEN && op2->type == CLOSE)//括弧の中がまとまったら
	{
		opstack.pop();
		previous_char = CLOSE;
	}
	else opstack.push(op2);	//それ以外の場合は後の演算子をスタックへ
	return OK;
}

void dispError(error e)
{
	switch (e)
	{
	case error::COMPLETE:
		cout << "答えは " << numstack.top() << " です。" << endl;
		break;
	case error::CALC_ERROR:
		cout << "計算に失敗しました。" << endl;
		break;
	case error::INVALID_FORM:
		cout << "式が間違っています。" << endl;
		break;
	case error::INVALID_WORD:
		cout << "その関数は使用できません。" << endl;
		break;
	case error::INVARID_OPERATOR:
		cout << "その演算子は使用できません。" << endl;
		break;
	default:
		break;
	}
}

void main()
{
	opset();
	string str1;//
	//	cout << str1 << endl;
	cout << "利用可能な演算子は +, -, *, /, ^, ! (,) です。" << endl;
	cout << "数式を入力してください。" << endl;
	cin >> str1;
	for (int i = 0; i < str1.length(); i++)
	{
		char token = str1[i];
		cout << token << " ";
		if (isdigit(token))//数字か
		{
			numstack.push(atof(&token));
			previous_char = DIGIT;
		}
		else if (ispunct(token))//演算子か
		{
			e = opcheck(token, &previous_char, &current_op);//認識
			if (current_op->type == FACT)//!なら直ちに計算
			{
				int f = 1;
				for (int k = 2; k < numstack.top(); k++)
				{
					f = f * k;
				}
				numstack.pop();
				numstack.push(f);
				previous_char = DIGIT;
			}
			else if (!opstack.empty())//OPスタックが空でない
			{
				compare(current_op);//比較
			}
			else//OPスタックが空
			{
				opstack.push(current_op);
			}
		}

		if (e != OK)//エラー表示
		{
			dispError(e);
			break;
		}
	}
	//以下、全て読み終わった状態
	if (numstack.size() > 1)//スタックに数が2以上
	{
		while (!opstack.empty())	//演算子が空になるまで
		{
			op* op2 = opstack.top();
			opstack.pop();
			if (op2->type == FACT)// ! ならただちに計算
			{
				calc(op2);
			}
			else if (!opstack.empty())//まだ残っていたら
			{
				compare(op2);
				if (opstack.size() == 2)//前の優先度が低い
				{
					calc(op2);
					opstack.pop();
				}
			}
			else calc(op2);
		}
	}
	if (numstack.size() == 1)//スタックに数が1つ
	{
		e = COMPLETE;
	}
	else//1つ以上
	{
		e = CALC_ERROR;
	}
	dispError(e);
	cout << "End" << endl;
	while (1)
	{
		if (getchar()) break;
	}
}

//error parenthesis()//　）が来た　ではなくて、（）が揃ったら閉じてよし 中の演算子を回収するには・・・
//{
//	op* op2 = opstack.top();
//
//	while (op2->type != OPEN)
//	{
//		opstack.pop();
//		op* op1 = opstack.top();
//		compare(op1, op2);
//	}
//}
