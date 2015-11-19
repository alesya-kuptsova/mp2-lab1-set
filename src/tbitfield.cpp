// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		int a;
		BitLen = len;
		a = BitLen / 32;
		MemLen = a + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = 0;
		}
	}
	else
	{
		throw len;
	}


}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}

}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int k = n % 32;
	TELEM M = 1;
	//делаем сдвиг:
	M = M << k;
	return M;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((-1 < n) && (n < BitLen)) // нумерацию битов делаем с 0, потому и >-1
	{
		int index;
		index = GetMemIndex(n);
		TELEM Mask;
		Mask = GetMemMask(n); // получили индекс, получили маску
		pMem[index] = pMem[index] | Mask; // сделали побитовое ИЛИ
	}
	else
	{
		throw n;
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((-1 < n) && (n < BitLen))
	{
		int index;
		index = GetMemIndex(n);
		TELEM Mask;
		Mask = GetMemMask(n);
		pMem[index] = pMem[index] & ~Mask;
	}
	else
	{
		throw n;
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((-1 < n) && (n < BitLen))
	{
		int index;
		index = GetMemIndex(n);
		TELEM Mask;
		Mask = GetMemMask(n);
		TELEM M = Mask;
		M = Mask & pMem[index];
		int CheckPro;
		CheckPro = 0;
		if (Mask == M)
		{
			CheckPro = 1;
		}
		return CheckPro;
	}
	else
	{
		throw n;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (*this != bf)
	{
		if (MemLen == bf.MemLen)
		{
			BitLen = bf.BitLen;
			for (int i = 0; i < MemLen; i++)
				pMem[i] = bf.pMem[i];
		}
		else
		{
			MemLen = bf.MemLen;
			BitLen = bf.BitLen;
			for (int i = 0; i < MemLen; i++)
			{
				pMem[i] = bf.pMem[i];
			}
		}
	}
	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение (равно)
{
	int c;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < BitLen; i++)
		{
			if (GetBit(i) == bf.GetBit(i))
			{
				c = 1;
			}
			if (GetBit(i) != bf.GetBit(i))
			{
				c = 0;
				break;
			}
		}
	}
	else
	{
		c = 0;
	}
	return c;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение (не равно)
{
	int c;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < BitLen; i++)
		{
			if (GetBit(i) == bf.GetBit(i))
			{
				c = 0;
			}
			if (GetBit(i) != bf.GetBit(i))
			{
				c = 1;
				break;
			}
		}
	}
	else
	{
		c = 1;
	}
	return c;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int MLen;
	MLen = BitLen;
	if (bf.BitLen > BitLen)
	{
		MLen = bf.BitLen;
	}
	TBitField c(MLen);
	for (int i = 0; i < MemLen; i++)
	{
		c.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		c.pMem[i] |= bf.pMem[i];
	}
	return (c);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int MLen;
	MLen = BitLen;
	if (bf.BitLen > BitLen)
	{
		MLen = bf.BitLen;
	}
	TBitField c(MLen);
	for (int i = 0; i < MemLen; i++)
	{
		c.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		c.pMem[i] &= bf.pMem[i];
	}
	return (c);
}

TBitField TBitField::operator~(void) // отрицание
{
	//давай-ка побитово
	TBitField c(BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		c.pMem[i] = ~pMem[i];
	}
	return c;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	for (i = 0; i < bf.BitLen; i++)
	{
		int c; // - c клавы вводим
		istr >> c;
		if (c == 1)
		{
			bf.SetBit(i);
		}
		else if (c == 0)
		{
			bf.ClrBit(i);
		}
		else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int c;
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i) << " ";
	return ostr;

}
