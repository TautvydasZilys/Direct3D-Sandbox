#pragma once

struct Score
{
	float secondsSurvived;
	int zombiesKilled;

	Score() {}
	Score(float secondsSurvived, int zombiesKilled) :
		secondsSurvived(secondsSurvived), zombiesKilled(zombiesKilled)
	{
	}
	
	bool operator==(const Score& other) const
	{
		return memcmp(this, &other, sizeof(Score)) == 0;
	}

	bool operator!=(const Score& other) const
	{
		return memcmp(this, &other, sizeof(Score)) != 0;
	}
};

class Highscore
{
private:
	Score m_LongestSurvival;
	Score m_MostZombiesKilled;

	Highscore();
	void SaveScore();
	
public:
	~Highscore();
	static Highscore Load();

	inline const Score& GetLongestSurvivalScore() const { return m_LongestSurvival; }
	inline const Score& GetMostZombiesKilledScore() const { return m_MostZombiesKilled; }

	bool SubmitScore(float secondsSurvived, int zombiesKilled);
};
