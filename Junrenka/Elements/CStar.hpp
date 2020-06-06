#pragma once

#include <windows.h>

#define CHEAT_LUMINOSITY 2
#define STAR_NAME_LENGTH (8 + 1)


#include <time.h> 

class CStar {

private:
	float m_Luminosity; //光度
	PWCHAR m_pStarName;
	HANDLE m_Heap;

public:

	///<summary>CStar　デフォルトコンストラクタ</summary>
	///<remarks>名前が指定されていない場合は0x00000000-0xFFFFFFFFの間の無名のみたいな名前が付く</remarks>
	CStar()
	{
		m_Heap = ::GetProcessHeap();
		m_Luminosity = (float)rand() / 32767.0; //0～1


		if ((m_pStarName = (PWCHAR)::HeapAlloc(m_Heap, HEAP_ZERO_MEMORY, STAR_NAME_LENGTH << 1)))
		{
			::wsprintfW(m_pStarName, L"%08X", (DWORD)this);
		}
	}

	///<summary>CStar　コンストラクタ</summary>
	///<param name='Name'>つけたい星の名前</param>
	CStar(PWCHAR Name)
	{
		m_Luminosity = 0;
		m_Heap = ::GetProcessHeap();
		DWORD NameLength = wcslen(Name) + 1;
		if ((m_pStarName = (PWCHAR)::HeapAlloc(m_Heap, HEAP_ZERO_MEMORY, NameLength << 1)))
		{
			wcscpy_s(m_pStarName, NameLength, Name);
		}
	}

	///<summary>CStar　デストラクタ</summary>
	~CStar()
	{
		if (m_pStarName) 
		{
			::HeapFree(m_Heap,0, m_pStarName);
			m_pStarName = NULL;
		}
	}

	///<summary>異常な光度を設定</summary>
	void UseCheat() { m_Luminosity = CHEAT_LUMINOSITY; }

	///<summary>星の光度を取得</summary>
	///<returns>光度</returns>
	float GetLuminosity() { return m_Luminosity; }

	///<summary>星の名前を取得</summary>
	///<returns>星の名前</returns>
	virtual PWCHAR GetName()
	{
		return m_pStarName;
	}

};
