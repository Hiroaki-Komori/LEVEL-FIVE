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
			*p_op = &posi;//����
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
			*p_op = &nega;//����
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

	if (numstack.empty())return INVALID_FORM;//�X�^�b�N����
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
		if (numstack.empty())return INVALID_FORM;//�X�^�b�N����
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
	//�D��x�� �O���� �Ȃ� �O����v�Z�����Z�q������ ���J��Ԃ��@���O���i�@�̂Ƃ��ȊO
	while (!opstack.empty() && opstack.top()->level >= op2->level && opstack.top()->type != OPEN)
	{
		calc(opstack.top());
		opstack.pop();
	}
	if (!opstack.empty() && opstack.top()->type == OPEN && op2->type == CLOSE)//���ʂ̒����܂Ƃ܂�����
	{
		opstack.pop();

		if (!opstack.empty() &&opstack.top()->type == FUNC)
		{
			funcCalc(opstack.top()->func);
			opstack.pop();
		}
		else previous_char = CLOSE;
	}
	else opstack.push(op2);	//����ȊO�̏ꍇ�͌�̉��Z�q���X�^�b�N��
	return OK;
}

void dispError(error e)
{
	switch (e)
	{
	case error::COMPLETE:
		cout << "������ " << numstack.top() << " �ł��B" << endl;
		break;
	case error::CALC_ERROR:
		cout << "�v�Z�Ɏ��s���܂����B" << endl;
		break;
	case error::INVALID_FORM:
		cout << "�����Ԉ���Ă��܂��B" << endl;
		break;
	case error::INVARID_OPERATOR:
		cout << "�g�p�ł��Ȃ����Z�q���܂܂�Ă��܂��B" << endl;
		break;
	case error::INVALID_FUNC:
		cout << "�����񂪐������Ȃ����A�g�p�ł��Ȃ��֐��ł��B" << endl;
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
	cout << "���p�\�ȉ��Z�q�� +, -, *, /, ^, ! (,) ," << endl;
	cout << "�֐��� sin(), cos(), tan(), sqrt(), exp(), log10(), ln()�ł��B" << endl;
	cout << "���͐����̂ݓ��͉\�ł��B" << endl;

	while (!escape){

		cout << "��������͂��Ă��������B" << endl;
		cin >> str1;

		for (int i = 0; i < str1.length(); i++)
		{
			char token = str1[i];
			if (previous_char == ALPHA&& !islower(token))//�p��������ȊO�F�֐�����
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
			if (isdigit(token))//������
			{
				if (previous_char == DIGIT)//�����A�������猅�𑫂�
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
					if (previous_char == FACT || previous_char == FUNC || previous_char == CLOSE)//���ʌ�ɒ��u�����ꂽ�ꍇ�A*�Ōq��
					{
						opstack.push(&mul);
					}
				}
			}
			else if (ispunct(token))//���Z�q��
			{
				err = opcheck(token, &previous_char, &current_op);//�F��
				if (current_op->type == OPEN&& previous_char == DIGIT)//���ʑO�ɒ��u�����ꂽ�ꍇ*������
				{
					opstack.push(&mul);
					previous_char = BINARY;
				}
				if (current_op->type == FACT)//!�Ȃ璼���Ɍv�Z
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
				else if (!opstack.empty())//OP�X�^�b�N����łȂ�
				{
					compare(current_op);//��r
					previous_char = current_op->type;
				}
				else//OP�X�^�b�N����
				{
					opstack.push(current_op);
					previous_char = current_op->type;
				}
			}
			else if (islower(token))//�p��������
			{
				word += token;
				previous_char = ALPHA;
			}

			if (err != OK)//�G���[�\��
			{
				dispError(err);
				break;
			}
		}
		//�ȉ��A�S�ēǂݏI��������
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
		if (numstack.size() > 1)//�X�^�b�N�ɐ���2�ȏ�
		{
			while (!opstack.empty())	//���Z�q����ɂȂ�܂�
			{
				op* op2 = opstack.top();
				opstack.pop();
				if (op2->type == FACT)// ! �Ȃ炽�����Ɍv�Z
				{
					calc(op2);
				}
				else if (!opstack.empty())//�܂��c���Ă�����
				{
					compare(op2);
					if (opstack.size() == 2)//�O�̗D��x���Ⴂ
					{
						calc(op2);
						opstack.pop();
					}
				}
				else calc(op2);
			}
		}
		if (numstack.size() == 1)//�X�^�b�N�ɐ���1��
		{
			err = COMPLETE;
		}
		else//1�ȏ�
		{
			err = CALC_ERROR;
		}
		dispError(err);
		err = OK;
		while (!numstack.empty()){ numstack.pop(); }
		while (!opstack.empty()){ opstack.pop(); }
		getchar();
		cout << "������: Enter " << "��߂�FEsc" << endl;
		while (1)
		{
			if (_getch() == 0x1B) {
				return;
			}
			if (_getch() == 0x0D) break;
		}
	}
}