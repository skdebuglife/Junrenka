#pragma once

#include <windows.h>

#define CHEAT_LUMINOSITY 2
#define STAR_NAME_LENGTH (8 + 1)


#include <time.h> 

class CStar {

private:
	float m_Luminosity; //���x
	PWCHAR m_pStarName;
	HANDLE m_Heap;

public:

	///<summary>CStar�@�f�t�H���g�R���X�g���N�^</summary>
	///<remarks>���O���w�肳��Ă��Ȃ��ꍇ��0x00000000-0xFFFFFFFF�̊Ԃ̖����݂̂����Ȗ��O���t��</remarks>
	CStar()
	{
		m_Heap = ::GetProcessHeap();
		m_Luminosity = (float)rand() / 32767.0; //0�`1


		if ((m_pStarName = (PWCHAR)::HeapAlloc(m_Heap, HEAP_ZERO_MEMORY, STAR_NAME_LENGTH << 1)))
		{
			::wsprintfW(m_pStarName, L"%08X", (DWORD)this);
		}
	}

	///<summary>CStar�@�R���X�g���N�^</summary>
	///<param name='Name'>���������̖��O</param>
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

	///<summary>CStar�@�f�X�g���N�^</summary>
	~CStar()
	{
		if (m_pStarName) 
		{
			::HeapFree(m_Heap,0, m_pStarName);
			m_pStarName = NULL;
		}
	}

	///<summary>�ُ�Ȍ��x��ݒ�</summary>
	void UseCheat() { m_Luminosity = CHEAT_LUMINOSITY; }

	///<summary>���̌��x���擾</summary>
	///<returns>���x</returns>
	float GetLuminosity() { return m_Luminosity; }

	///<summary>���̖��O���擾</summary>
	///<returns>���̖��O</returns>
	virtual PWCHAR GetName()
	{
		return m_pStarName;
	}

};
