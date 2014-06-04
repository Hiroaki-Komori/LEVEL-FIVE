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
			*p_op = &posi;//����
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
			*p_op = &nega;//����
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
	//�X�^�b�N�̐擪�̗D��x���X�^�b�N����Ⴍ�Ȃ�܂ŌJ��Ԃ���r���v�Z�����Z�q������
	while (!opstack.empty() && opstack.top()->level >= op2->level && opstack.top()->type != OPEN)
	{
		calc(opstack.top());
		opstack.pop();
	}
	if (!opstack.empty() && opstack.top()->type == OPEN && op2->type == CLOSE)//���ʂ̒����܂Ƃ܂�����
	{
		opstack.pop();
		previous_char = CLOSE;
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
	case error::INVALID_WORD:
		cout << "���̊֐��͎g�p�ł��܂���B" << endl;
		break;
	case error::INVARID_OPERATOR:
		cout << "���̉��Z�q�͎g�p�ł��܂���B" << endl;
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
	cout << "���p�\�ȉ��Z�q�� +, -, *, /, ^, ! (,) �ł��B" << endl;
	cout << "��������͂��Ă��������B" << endl;
	cin >> str1;
	for (int i = 0; i < str1.length(); i++)
	{
		char token = str1[i];
		cout << token << " ";
		if (isdigit(token))//������
		{
			numstack.push(atof(&token));
			previous_char = DIGIT;
		}
		else if (ispunct(token))//���Z�q��
		{
			e = opcheck(token, &previous_char, &current_op);//�F��
			if (current_op->type == FACT)//!�Ȃ璼���Ɍv�Z
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
			else if (!opstack.empty())//OP�X�^�b�N����łȂ�
			{
				compare(current_op);//��r
			}
			else//OP�X�^�b�N����
			{
				opstack.push(current_op);
			}
		}

		if (e != OK)//�G���[�\��
		{
			dispError(e);
			break;
		}
	}
	//�ȉ��A�S�ēǂݏI��������
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
		e = COMPLETE;
	}
	else//1�ȏ�
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

//error parenthesis()//�@�j�������@�ł͂Ȃ��āA�i�j������������Ă悵 ���̉��Z�q���������ɂ́E�E�E
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
