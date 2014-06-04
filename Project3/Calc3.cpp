#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<conio.h>

using namespace std;
enum oplevel{ L0, L1, L2, L3, L4, L5 };
enum chartype{ NONE, DIGIT, ALPHA, UNARY, BINARY, FACT, OPEN, CLOSE, FUNC, SPDIGIT };
enum error{ OK, COMPLETE, UNCOMPLETED, INVALID_WORD, INVARID_OPERATOR, INVALID_FORM, CALC_ERROR, INVALID_FUNC, FUNC_ERROR };
enum functype{ INVALID, SIN, COS, TAN, PI, LOG, LOG10, LN, E, EXP, SQRT };

struct op{
	char token;
	chartype type;
	oplevel level;
	string name;
	functype func;
};
op add, sub, mul, mydiv, mypow, fact, open, close, posi, nega, none, mysin, mycos, mytan, myln, mylog, myexp, mysqrt, pi, e;

stack<double>numstack;
stack<op*>opstack;
string word, digits;

op* current_op = &none;
chartype previous_char = NONE;
error err = OK;

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

	mysin.name = "sin";
	mysin.level = L2;
	mysin.type = FUNC;
	mysin.func = SIN;

	mycos.name = "cos";
	mycos.level = L2;
	mycos.type = FUNC;
	mycos.func = COS;

	mytan.name = "tan";
	mytan.level = L2;
	mytan.type = FUNC;
	mytan.func = TAN;

	myln.name = "ln";
	myln.level = L2;
	myln.type = FUNC;
	myln.func = LOG;

	myexp.name = "exp";
	myexp.level = L2;
	myexp.type = FUNC;
	myexp.func = EXP;

	mysqrt.name = "sqrt";
	mysqrt.level = L2;
	mysqrt.type = FUNC;
	mysqrt.func = SQRT;

	pi.name = "pi";
	pi.type = SPDIGIT;
	pi.func = PI;

	e.name = "e";
	e.type = SPDIGIT;
	e.func = E;
}

error opcheck(char token, chartype *prechar, op* *p_op)
{
	if (token == add.token)	{

		if (*prechar == NONE || *prechar == OPEN)
		{
			*p_op = &posi;//符号
//			*prechar = UNARY;
		}
		else{
			*p_op = &add;
//			*prechar = BINARY;
		}
	}
	else if (token == sub.token)
	{
		if (*prechar == NONE || *prechar == OPEN)
		{
			*p_op = &nega;//符号
//			*prechar = UNARY;
		}
		else {
			*p_op = &sub;
//			*prechar = BINARY;
		}
	}
	else if (token == mul.token){
		*p_op = &mul;
//		*prechar = BINARY;
	}
	else if (token == mydiv.token)
	{
		*p_op = &mydiv;
//		*prechar = BINARY;
	}
	else if (token == mypow.token){
		*p_op = &mypow;
//		*prechar = BINARY;
	}
	else if (token == fact.token) {
		*p_op = &fact;
//		*prechar = UNARY;
	}
	else if (token == open.token) {
	*p_op = &open;
//		*prechar = OPEN;
	}
	else if (token == close.token) {
		*p_op = &close;
		//		*prechar = CLOSE;
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
		switch (opr->token)
		{
		case '+':
			//			cout << num1 << "+" << num2 << "=" << (num1 + num2) << endl;
			numstack.push(num1 + num2);
			return OK;
		case '-':
			//			cout << num1 << "-" << num2 << "=" << (num1 - num2) << endl;
			numstack.push(num1 - num2);
			return OK;
		case '*':
			//			cout << num1 << "*" << num2 << "=" << (num1 * num2) << endl;
			numstack.push(num1 * num2);
			return OK;
		case '/':
			//			cout << num1 << "/" << num2 << "=" << (num1 / num2) << endl;
			numstack.push(num1 / num2);
			return OK;
		case '^':
			ans = pow(num1, num2);
			//			cout << num1 << "^" << num2 << "=" << ans << endl;
			numstack.push(ans);
			return OK;
		default:
			numstack.push(num1);
			numstack.push(num2);
			return CALC_ERROR;
		}
	}
}
op* funcProc(string str)
{
	if (str == "sin"){ return &mysin; }
	else if (str == "cos"){ return &mycos; }
	else if (str == "tan"){ return &mytan; }
	else if (str == "pi") { return &pi; }
	else if (str == "e"){ return &e; }
	else if (str == "log"){ return &mylog; }
	else if (str == "ln"){ return &myln; }
	else if (str == "exp"){ return &myexp; }
	else if (str == "sqrt"){ return &mysqrt; }
	else return &none;
}

error funcCalc(functype func)
{
	double var = numstack.top();
	numstack.pop();
	double ans = 0;
	switch (func)
	{
	case SIN:
		ans = sin(var);
//		cout << " sin(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case COS:
		ans = cos(var);
		//cout << "cos(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case TAN:
		ans = tan(var);
		//cout << "tan(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case LOG10:
		ans = log10(var);
		//cout << "log10(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case LN:
		ans = log(var);
		//cout << "ln(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case EXP:
		ans = exp(var);
		//cout << "exp(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	case SQRT:
		ans = sqrt(var);
		//cout << "sqrt(" << var << ") = " << ans << endl;
		numstack.push(ans);
		return OK;
	default:
		return FUNC_ERROR;
	}
}

error compare(op* op2)
{
	//優先度が 前≧後 なら 前から計算→演算子を消す を繰り返す　※前が（　のとき以外
	while (!opstack.empty() && opstack.top()->level >= op2->level && opstack.top()->type != OPEN)
	{
		calc(opstack.top());
		opstack.pop();
	}
	if (!opstack.empty() && opstack.top()->type == OPEN && op2->type == CLOSE)//括弧の中がまとまったら
	{
		opstack.pop();

		if (!opstack.empty() &&opstack.top()->type == FUNC)
		{
			funcCalc(opstack.top()->func);
			opstack.pop();
		}
		else previous_char = CLOSE;
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
	case error::INVARID_OPERATOR:
		cout << "使用できない演算子が含まれています。" << endl;
		break;
	case error::INVALID_FUNC:
		cout << "文字列が正しくないか、使用できない関数です。" << endl;
		break;
	default:
		break;
	}
}

void main()
{
	opset();
	string str1;//
	bool escape = false;
	//	cout << str1 << endl;
	cout << "利用可能な演算子は +, -, *, /, ^, ! (,) ," << endl;
	cout << "関数は sin(), cos(), tan(), sqrt(), exp(), log10(), ln()です。" << endl;
	cout << "数は整数のみ入力可能です。" << endl;

	while (!escape){

		cout << "数式を入力してください。" << endl;
		cin >> str1;

		for (int i = 0; i < str1.length(); i++)
		{
			char token = str1[i];
			if (previous_char == ALPHA&& !islower(token))//英字→それ以外：関数判定
			{
				op* func = funcProc(word);
				word.clear();
				if (func == &none)
				{
					err = INVALID_FUNC;
				}
				else if (func->type == SPDIGIT)
				{
					switch (func->func)
					{
					case PI:
						numstack.push(M_PI);
						break;
					case E:
						numstack.push(M_E);
						break;
					}
				}
				else
				{
					opstack.push(func);
					previous_char = FUNC;
				}
			}
			if (isdigit(token))//数字か
			{
				if (previous_char == DIGIT)//数が連続したら桁を足す
				{
					double num1 = numstack.top();
					num1 = num1 + atof(&token);
					numstack.pop();
					numstack.push(num1);
				}
				else
				{
					numstack.push(atof(&token));
					previous_char = DIGIT;
					if (previous_char == FACT || previous_char == FUNC || previous_char == CLOSE)//括弧後に直置きされた場合、*で繋ぐ
					{
						opstack.push(&mul);
					}
				}
			}
			else if (ispunct(token))//演算子か
			{
				err = opcheck(token, &previous_char, &current_op);//認識
				if (current_op->type == OPEN&& previous_char == DIGIT)//括弧前に直置きされた場合*を入れる
				{
					opstack.push(&mul);
					previous_char = BINARY;
				}
				if (current_op->type == FACT)//!なら直ちに計算
				{
					int f = 1;
					for (int k = 2; k < numstack.top(); k++)
					{
						f = f * k;
					}
					numstack.pop();
					numstack.push(f);
					previous_char = FACT;
				}
				else if (!opstack.empty())//OPスタックが空でない
				{
					compare(current_op);//比較
					previous_char = current_op->type;
				}
				else//OPスタックが空
				{
					opstack.push(current_op);
					previous_char = current_op->type;
				}
			}
			else if (islower(token))//英小文字か
			{
				word += token;
				previous_char = ALPHA;
			}

			if (err != OK)//エラー表示
			{
				dispError(err);
				break;
			}
		}
		//以下、全て読み終わった状態
		if (!word.empty())
		{
			op* func = funcProc(word);
			word.clear();
			if (func->type = SPDIGIT)
			{
				switch (func->func)
				{
				case PI:
					numstack.push(M_PI);
					break;
				case E:
					numstack.push(M_E);
					break;
				}
			}
			else FUNC_ERROR;

		}
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
			err = COMPLETE;
		}
		else//1つ以上
		{
			err = CALC_ERROR;
		}
		dispError(err);
		err = OK;
		while (!numstack.empty()){ numstack.pop(); }
		while (!opstack.empty()){ opstack.pop(); }
		getchar();
		cout << "続ける: Enter " << "やめる：Esc" << endl;
		while (1)
		{
			if (_getch() == 0x1B) {
				return;
			}
			if (_getch() == 0x0D) break;
		}
	}
}