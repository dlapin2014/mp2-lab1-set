// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"



TBitField::TBitField(int len)
{
	if (len < 0) throw invalid_argument("negative bitfield length");

	BitLen = len;
	MemLen = (len + 8 * sizeof(TELEM) - 1) / (8 * sizeof(TELEM));
	//cout << "||||" << MemLen <<" "  << BitLen<< "|||||||";
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;

}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1u << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw invalid_argument("negative bit index");
	if (n >=BitLen) throw invalid_argument("too large bit index");


	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{

	if (n < 0) throw invalid_argument("negative bit index");
	if (n >= BitLen) throw invalid_argument("too large bit index");

	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw invalid_argument("negative bit index");
	if (n >= BitLen) throw invalid_argument("too large bit index");
	return (pMem[GetMemIndex(n)] >> (n % (8 * sizeof(TELEM)))) & 1u;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf) {



		if (BitLen != bf.BitLen) {
			delete[] pMem;

			MemLen = bf.MemLen;
			BitLen = bf.BitLen;
			pMem = new TELEM[MemLen];
		}


		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}

	}
	return *this;
}


int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (MemLen != bf.MemLen)
		return 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (bf.pMem[i] != pMem[i])
				return 0;

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (bf == *this)
		return 0;
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{

	int MaxBitLen = 0;
	int MinBitLen = 0;
	if (bf.BitLen < BitLen) {
		MaxBitLen = BitLen;
		MinBitLen = bf.BitLen;
	}
	else {
		MaxBitLen = bf.BitLen;
		MinBitLen = BitLen;
	}


	TBitField temp(MaxBitLen);

	int MinMemLen = (MinBitLen + 8 * sizeof(TELEM) - 1) / (8 * sizeof(TELEM));

	for (int i = 0; i < MinMemLen; i++)
		temp.pMem[i] = bf.pMem[i] | pMem[i];





	return temp;

}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int MaxBitLen = 0;
	int MinBitLen = 0;
	if (bf.BitLen < BitLen) {
		MaxBitLen = BitLen;
		MinBitLen = bf.BitLen;
	}
	else {
		MaxBitLen = bf.BitLen;
		MinBitLen = BitLen;
	}


	TBitField temp(MaxBitLen);

	int MinMemLen = (MinBitLen + 8 * sizeof(TELEM) - 1) / (8 * sizeof(TELEM));

	for (int i = 0; i < MinMemLen; i++)
		temp.pMem[i] = bf.pMem[i] & pMem[i];

	return temp;


}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	for (int i = 0; i < MemLen - 1; i++) {
		temp.pMem[i] = ~temp.pMem[i];
	}
	int offset = (8 * sizeof(TELEM) - (BitLen % (8 * sizeof(TELEM)))) % (8 * sizeof(TELEM));
	temp.pMem[MemLen - 1] = (~(temp.pMem[MemLen - 1] << offset)) >> offset;
	return temp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{

	string temp;
	istr >> temp;
	unsigned int length = temp.length();
	temp.resize(8 * sizeof(TELEM) * bf.MemLen);
	for (int i = length; i < 8 * sizeof(TELEM) * bf.MemLen; i++)
		temp[i] = '0';

	unsigned int t;



	for (int i = 0; i < bf.MemLen; i++) {
		for (int j = 0; j < 8 * sizeof(TELEM); j++) {
			t = temp[i * 8 * sizeof(TELEM) + j] - '0';
			bf.pMem[i] = bf.pMem[i] | (t << j);
		}
	}




	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.MemLen; i++) {
		TELEM f = 1u;
		for (int j = 0; j < 8 * sizeof(TELEM); j++) {
			if (bf.pMem[i] & f) {
				ostr << "1";
			}
			else
				ostr << "0";
			f = f << 1;
		}

		ostr << " ";

	}

	return ostr;
}
