#pragma once

struct Trituple {        //自定义数据结构：矩阵元素的行，列，值；
	int row, col;
	int value;
	Trituple& operator=(Trituple& x) {  //赋值运算符重载
		row = x.row;
		col = x.col;
		value = x.value;
		return *this;
	}
};