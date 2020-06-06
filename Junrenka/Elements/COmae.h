#pragma once

#include "CPerson.hpp"
#include "CStar.hpp"

class COmae : public CPerson, public CStar
{
private:


public:
	COmae(PWCHAR Name, PWCHAR Favourite = NULL, TASTELEVEL TastLevel = TASTELEVEL::Normal)
		:CPerson(Name, Favourite, TastLevel),CStar(Name)
	{
		UseCheat();
	}
	COmae(){}

	virtual void NotifyOthersEmotion(EMOTION OthersEmotion)
	{
		//人のマジギレみたら楽しい
		if (OthersEmotion == EMOTION::Majigire)
		{
			m_Emotion = EMOTION::Happy;
		}
		else
		{
			m_Emotion = OthersEmotion;
		}
	}
};