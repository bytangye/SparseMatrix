#pragma once

# include"Trituple.h"
# include<iostream>
# include<assert.h>

const int DefaultSize = 100;

class SparseMatrix {
private:
	int Rows, Cols, Terms;														//����������������Ԫ�صĸ���
	Trituple *smArray;															//�����Ԫ�ص���Ԫ����
	int maxTerms;																//��Ԫ���������ɵ�Ԫ�ظ���
public:
	SparseMatrix(int maxSz = DefaultSize);										//���캯��
	SparseMatrix(SparseMatrix& SM);												//��ֵ���캯��
	~SparseMatrix();															//��������
	SparseMatrix& operator=(SparseMatrix& SM);									//��ֵ���������
	SparseMatrix Transpose();													//����ת��
	SparseMatrix Add(SparseMatrix& b);											//����ļӷ�
	SparseMatrix Multiply(SparseMatrix& b);										//����ĳ˷�
	friend std::ostream& operator<<(std::ostream& ostr, SparseMatrix& SM);		//�����������غ���
	friend std::istream& operator>>(std::istream& istr, SparseMatrix& SM);		//������������غ���
};

SparseMatrix::SparseMatrix(int maxSz) :maxTerms(maxSz) {     
	//���캯��������һ����СΪmaxTerm����Ԫ�飬�������ͷ���Ԫ�ظ���������
	if (maxSz<1) {
		std::cerr << "�����ʼ������" << std::endl;
		exit(1);
	}
	smArray = new Trituple[maxSz];
	assert(smArray != NULL);
	Rows = Cols = Terms = 0;
}

SparseMatrix::SparseMatrix(SparseMatrix& SM) {  //���ƹ��캯��
	Rows = SM.Rows;      //��ֵ���������
	Cols = SM.Cols;
	Terms = SM.Terms;
	maxTerms = SM.maxTerms;
	smArray = new Trituple[maxTerms];  //������Ԫ�鲢����SM��ͬ��ֵ
	assert(smArray != NULL);
	for (int i = 0; i<Terms; i++)
		smArray[i] = SM.smArray[i];
}

SparseMatrix::~SparseMatrix() {   //�����������ͷ����д洢
	delete[]smArray;
}

SparseMatrix& SparseMatrix::operator=(SparseMatrix& SM) { //��ֵ���������
	Rows = SM.Rows;      //Ԫ�����ʵĸ�ֵ
	Cols = SM.Cols;
	Terms = SM.Terms;
	maxTerms = SM.maxTerms;
	for (int i = 0; i<Terms; i++)      //��Ԫ������Ԫ�ظ�ֵ
		smArray[i] = SM.smArray[i];
	return *this;    //���ص��ǶԵ��øú����Ķ�������ã�����ʽʹ��thisָ�룻
}

std::ostream& operator<<(std::ostream& ostr, SparseMatrix& SM) {  //������������ ��Ϊɶ��ģ��Ͳ��ܵ���row�� ��
	ostr << "# Rows=" << SM.Rows << std::endl;    //����þ��������
	ostr << "# Cols=" << SM.Cols << std::endl;
	ostr << "# Terms=" << SM.Terms << std::endl;
	for (int i = 0; i<SM.Terms; i++)   //����þ������Ԫ�ص�λ�ü�ֵ
		ostr << i + 1 << ": " << "SM<" << SM.smArray[i].row << "," << SM.smArray[i].col << ">=" <<
		SM.smArray[i].value << std::endl;
	return ostr;
}

std::istream& operator>> (std::istream& istr, SparseMatrix& SM) {  //�������������
	std::cout << "Please enter number of rows,columns,and terms of Matrix" << std::endl;
	istr >> SM.Rows >> SM.Cols >> SM.Terms;  //����Ԫ�ص�����
	if (SM.Terms>SM.maxTerms) {
		std::cerr << "Numbers of Terms overflow!" << std::endl;
		exit(1);
	}
	for (int i = 0; i<SM.Terms; i++) {   //�����������Ԫ�ص������ֵ
		std::cout << "Enter row,column,and value of term:" << i + 1 << std::endl;
		std::cin >> SM.smArray[i].row >> SM.smArray[i].col >> SM.smArray[i].value;
	}
	return istr;
}

/*SparseMatrix SparseMatrix::Transpose(){ //ת�ú���
SparseMatrix b(maxTerms);
b.Rows=Rows;
b.Cols=Cols;
b.Terms=Terms;
b.maxTerms=maxTerms;
if(Terms>0){
int i,k,CurrentB=0;
for(k=0;k<b.Cols;k++)
for(i=0;i<Terms;i++)
if(smArray[i].col==k){
b.smArray[CurrentB].row=smArray[i].col;
b.smArray[CurrentB].col=smArray[i].row;
b.smArray[CurrentB].value=smArray[i].value;
CurrentB++;
}
}
return b;
}*/

SparseMatrix SparseMatrix::Transpose() {   //ת�ú���
	int *rowSize = new int[Cols]; //ת�þ���ÿ�з���Ԫ�صĸ���
	int *rowStart = new int[Cols]; //ת�þ���ÿ�е�һ������Ԫ�ض�Ӧ����Ԫ����±�
	SparseMatrix b(maxTerms); //ת�ú�ľ����Ӧ����Ԫ��
	b.Rows = Rows;       //b������
	b.Cols = Cols;
	b.Terms = Terms;
	b.maxTerms = maxTerms;
	if (Terms>0) {
		int i, j, CurrentB = 0;
		for (i = 0; i<Cols; i++)     //��rowSize���鸳ֵ
			rowSize[i] = 0;
		for (i = 0; i<Terms; i++)
			rowSize[smArray[i].col]++;
		rowStart[0] = 0;        //��rowStart���鸳ֵ
		for (i = 1; i<b.Rows; i++)
			rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
		for (i = 0; i<Terms; i++) { //������Ԫ��a���Ѹ���Ԫ�ذ�rowStart�������b����Ӧ��λ��
			j = rowStart[smArray[i].col]; //a�������кŰ���С�����˳�����У�������ͬ������������Ԫ�ؿ϶�������Ӧת�þ�����Ӧ���е���ǰ��
			b.smArray[j].row = smArray[i].col; //�Ѹ�Ԫ�ذ����ҵ����±�j����b��
			b.smArray[j].col = smArray[i].row;
			b.smArray[j].value = smArray[i].value;
			rowStart[smArray[i].col]++;  //��Ϊ��ֵ�Ѿ�����b������ת�þ���ĸ�����һ��Ԫ����b�ж�Ӧ���±�ΪrowStart[smArray[i].col]++��
		}
	}
	delete[] rowSize; //�ͷ�new����Ĵ洢�ռ�
	delete[] rowStart;
	return b;
}
SparseMatrix SparseMatrix::Add(SparseMatrix& b) {   //ת�þ���ļӷ�
	SparseMatrix Result(Rows*Cols);     //�������Result����
	if (Rows != b.Rows || Cols != b.Cols) {    //�����ͬ�ľ���������
		std::cout << "Incompatible matrices" << std::endl;
		return Result;
	}
	Result.Rows = Rows;
	Result.Cols = Cols;
	Result.Terms = 0;
	Result.maxTerms = Rows*Cols;
	int i = 0, j = 0, index_a, index_b; //i������a��Ԫ�飻index_a:��ǰ��ָ��a��Ԫ���ھ����е�λ�ã�
	while (i<Terms&&j<b.Terms) {
		index_a = smArray[i].row*Cols + smArray[i].col;
		index_b = b.smArray[j].row*b.Cols + b.smArray[i].col;
		if (index_a<index_b) {      //��ǰ��ָ��a��b������Ԫ�أ�a��Ԫ��λ����ǰ
			Result.smArray[Result.Terms].row = smArray[i].row;   //ֱ�Ӱ�a��Ԫ�ط���Result����
			Result.smArray[Result.Terms].col = smArray[i].col;
			Result.smArray[Result.Terms].value = smArray[i].value;
			i++; //iָ��ָ��a����һ��Ԫ��
		}
		if (index_a>index_b) {
			Result.smArray[Result.Terms].row = b.smArray[j].row;
			Result.smArray[Result.Terms].col = b.smArray[j].col;
			Result.smArray[Result.Terms].value = b.smArray[j].value;
			j++;
		}
		if (index_a == index_b) {  //λ����ͬ
			if (smArray[i].value + b.smArray[j].value) {   //�������ֵ��ӵĺͲ�Ϊ��
				Result.smArray[Result.Terms].row = smArray[j].row;   //����ӵĽ������Result��
				Result.smArray[Result.Terms].col = smArray[j].col;
				Result.smArray[Result.Terms].value = smArray[i].value + b.smArray[j].value;
				i++;
				j++;
			}
		}
		Result.Terms++;  //��һ��Ԫ�أ�����Ԫ�صĸ���+1��
	}
	//b��Ԫ���Ѿ������꣬��aʣ���Ԫ�ط���Result���棬��ʱi��ָ�ĵ�һ��Ԫ��λ�ÿ϶���b�����һ��Ԫ�غ���
	for (; i<Terms; i++) {       
		Result.smArray[Result.Terms].row = smArray[i].row;
		Result.smArray[Result.Terms].col = smArray[i].col;
		Result.smArray[Result.Terms].value = smArray[i].value;
		i++;
		Result.Terms++;
	}
	for (; j<b.Terms; j++) {
		Result.smArray[Result.Terms].row = b.smArray[j].row;
		Result.smArray[Result.Terms].col = b.smArray[j].col;
		Result.smArray[Result.Terms].value = b.smArray[j].value;
		j++;
		Result.Terms++;
	}
	return Result;
}

SparseMatrix SparseMatrix::Multiply(SparseMatrix& b) {  //����ĳ˷�
	SparseMatrix Result(Rows*b.Cols);      //��ž�����˵Ľ��
	if (Cols != b.Rows) {         //������������˵��Ⱦ���������һ�����������ڵڶ���������
		std::cerr << "Incompatible matrices" << std::endl;
		return Result;
	}
	int *rowSize = new int[b.Rows];   //b����ÿ�еķ���Ԫ�ظ���
	int *rowStart = new int[b.Rows + 1];    //b����ÿ�е�һ������Ԫ����b�е��±ꣻΪ�μ�һ��
	int *temp = new int[b.Cols];  // ��ʱ���Resultÿһ��ÿ��Ԫ�ص�������
	int i, Current, lastInResult, RowA, ColA, ColB;  //Current��a��ָ�룻 lastInResult��Result��ָ��
	for (i = 0; i<b.Rows; i++)    //��roeSize���鸳ֵ
		rowSize[i] = 0;
	for (i = 0; i<b.Terms; i++)
		rowSize[b.smArray[i].row]++;
	rowStart[0] = 0;      //��rowStart���鸳ֵ
	for (i = 1; i<b.Rows; i++)
		rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
	Current = 0;   //���±�0��ʼ����a
	lastInResult = -1; //����ֵ-1��Ϊ�˼��㷽�㣬������뼴���˽�
	while (Current<Terms) {  //������Ԫ��a��ÿһ��Ԫ��
		RowA = smArray[Current].row; //ȡ�õ�һ��Ԫ�ص�������
		for (i = 0; i<b.Cols; i++) //��temp���鸳��ֵΪ0��
			temp[i] = 0;
		while (Current<Terms&&smArray[Current].row == RowA) { //�Ը��е�����Ԫ�ؽ��в���
			ColA = smArray[Current].col; //��Ԫ�����ڵ���ColA���Ǹ�Ԫ�ض�Ӧ��˵�b��Ԫ�ص�����
			for (i = rowStart[ColA]; i<rowStart[ColA + 1]; i++) { //����b�и��е�����Ԫ��
				ColB = b.smArray[i].col; //a�ĸ�Ԫ����b�ж�Ӧ�е�ColB�е�Ԫ����˵Ľ��Ӧ�ñ��ŵ�temp[ColB]�У�
				temp[ColB] += smArray[Current].value*b.smArray[i].value;
			}
			Current++; //a�и�Ԫ�ص��Ѿ����ʹ����ָ��a����һ��Ԫ��
		}
		for (i = 0; i<b.Cols; i++) {  //�Ѹ��е�����������Result����
			if (temp[i] != 0) {
				lastInResult++;
				Result.smArray[lastInResult].row = smArray[Current].row;
				Result.smArray[lastInResult].col = i;
				Result.smArray[lastInResult].value = temp[i];
			}
		}
	}
	Result.Rows = Rows;     //��Result�����ʽ��и�ֵ����
	Result.Cols = b.Cols;
	Result.Terms = lastInResult + 1;
	delete[] rowSize; //�ͷ�new�����Ĵ洢�ռ�
	delete[] rowStart;
	delete[] temp;
	return Result;
}
