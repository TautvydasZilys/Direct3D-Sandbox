#include "PrecompiledHeader.h"
#include "Highscore.h"
#include "Tools.h"

static inline const wstring GetHighscoreFilePath()
{
	return Tools::GetAppDataPath(L"Zombie survival") + L"\\data";
}

Highscore::Highscore()
{
	ifstream in(GetHighscoreFilePath());

	if (in.is_open())
	{	
		in.read(reinterpret_cast<char*>(&m_LongestSurvival), sizeof(m_LongestSurvival));
		in.read(reinterpret_cast<char*>(&m_MostZombiesKilled), sizeof(m_MostZombiesKilled));
	}
	else
	{
		m_LongestSurvival = Score(0.0f, 0);
		m_MostZombiesKilled = Score(0.0f, 0);
	}
}

Highscore::~Highscore()
{
}

Highscore Highscore::Load()
{
	return Highscore();
}

void Highscore::SaveScore()
{
	ofstream out(GetHighscoreFilePath());
	Assert(out.is_open());
	
	out.write(reinterpret_cast<char*>(&m_LongestSurvival), sizeof(m_LongestSurvival));
	out.write(reinterpret_cast<char*>(&m_MostZombiesKilled), sizeof(m_MostZombiesKilled));
}

bool Highscore::SubmitScore(float secondsSurvived, int zombiesKilled)
{
	bool changed = false;

	if (secondsSurvived > m_LongestSurvival.secondsSurvived)
	{
		m_LongestSurvival = Score(secondsSurvived, zombiesKilled);
		changed = true;
	}

	if (zombiesKilled > m_MostZombiesKilled.zombiesKilled)
	{
		m_MostZombiesKilled = Score(secondsSurvived, zombiesKilled);
		changed = true;
	}

	if (changed)
	{
		SaveScore();
	}

	return changed;
}