#pragma once

#include "CPerson.hpp"

class CBestFriend : public CPerson
{
	friend class CSelf;

public:
	CBestFriend(PWCHAR _Name):CPerson(_Name){}

};

class CSelf : public CPerson
{
	friend class BestFriend;
public:
	CSelf(PWCHAR _Name):CPerson(_Name) {}

	///<summary>料理によって人に対する好感度を更新</summary>
	///<param name='pTarget'>好感度を設定する対象</param>
	///<remarks>すでに恋人がいる場合は例外を投げます</remarks>
	void UpdateFavorableSensitibity(CPerson *pTarget,TASTELEVEL _FoodLevel)
	{
		EMOTION UpdateEmotion;
		switch (_FoodLevel)
		{
		case TASTELEVEL::Delicious:
			//おいしかったらベタ惚れ
			UpdateEmotion = EMOTION::Happy;
			break;
		case TASTELEVEL::Normal:
			UpdateEmotion = EMOTION::None;
			break;
		case TASTELEVEL::Uncomfortable:
			UpdateEmotion = EMOTION::Angry;
			break;
		default:
			UpdateEmotion = EMOTION::None;
			break;
		}

		if (m_vFavorableSensitibity.size() > 0)
		{
			auto it = std::find_if(m_vFavorableSensitibity.begin(), m_vFavorableSensitibity.end(),
				[pTarget](FAVORABLE_SENSITIBITY& fs) {
					return (fs.pPerson == pTarget);
				});
			if (it != m_vFavorableSensitibity.end())
			{
				it->Emotion = UpdateEmotion;
				return;
			}
		}

		m_vFavorableSensitibity.push_back(FAVORABLE_SENSITIBITY{ pTarget ,UpdateEmotion });

	}

	void NotifyGameResult(GAME_RESULT Result) override
	{
		switch (Result)
		{
		case GAME_RESULT::Draw:
			m_Emotion = EMOTION::None;
			break;
		case GAME_RESULT::Win:
			m_Emotion = EMOTION::Happy;
			break;
		case GAME_RESULT::Lose:
			//負けてマジギレ
			m_Emotion = EMOTION::Majigire;
			break;
		default:
			m_Emotion = EMOTION::None;
			break;
		}
	}

	virtual void SetPartner(CPerson* pNewPartner) override
	{
		CPerson::SetPartner(pNewPartner);

		//まじめな顔をする
		m_Emotion = EMOTION::None;
	}
};