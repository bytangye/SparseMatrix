#pragma once

struct Trituple {        //�Զ������ݽṹ������Ԫ�ص��У��У�ֵ��
	int row, col;
	int value;
	Trituple& operator=(Trituple& x) {  //��ֵ���������
		row = x.row;
		col = x.col;
		value = x.value;
		return *this;
	}
};