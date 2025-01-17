// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля
#include "tset.h"
#include <sstream>

TSet::TSet(int mp) :MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{

}

// конструктор преобразования типа

/*: MaxPower(bf.GetLength()), BitField(bf)*/

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {

}

TSet::operator TBitField()
{
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	if (Elem < 0 || Elem >= MaxPower) throw invalid_argument("incorrect element");
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	if (Elem < 0 || Elem >= MaxPower) throw invalid_argument("incorrect element");
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	if (Elem < 0 || Elem >= MaxPower) throw invalid_argument("incorrect element");
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
	if (this != &s) {
		MaxPower = s.MaxPower;
		BitField = s.BitField;
	}
	return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
	if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
		return 1;

	return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
	if (s == *this)
		return 0;
	else
		return 1;
}



TSet TSet::operator+(const TSet& s) // объединение
{
	TBitField temp = BitField | s.BitField;
	return TSet(temp);
}




TSet TSet::operator+(const int Elem) // объединение с элементом
{
	if (Elem < 0 || Elem >= MaxPower) throw invalid_argument("incorrect element");
	TBitField temp(*this);
	temp.SetBit(Elem);
	return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	if (Elem < 0 || Elem >= MaxPower) throw invalid_argument("incorrect element");
	TBitField temp(*this);
	temp.ClrBit(Elem);
	return temp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
	TBitField temp = BitField & s.BitField;
	return TSet(temp);
}

TSet TSet::operator~(void) // дополнение
{
	TBitField temp = ~BitField;
	return TSet(temp);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
	string t;
	istr.clear();
	getline(istr, t);

	stringstream in(t);
	int temp;
	while (in >> temp) {
		s.InsElem(temp);
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
	for (int i = 0; i < s.MaxPower; i++)
		if (s.BitField.GetBit(i))
			ostr << i << " ";


	return ostr;
}