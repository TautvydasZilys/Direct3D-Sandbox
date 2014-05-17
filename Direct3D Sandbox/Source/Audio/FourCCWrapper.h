#pragma once

struct FourCCWrapper
{
	char cc[4];

	FourCCWrapper(int fourCC);
	~FourCCWrapper();

	inline bool operator==(const FourCCWrapper& right) const
	{
		return memcmp(this, &right, sizeof(FourCCWrapper)) == 0;
	}

	inline bool operator==(const int& right) const
	{
		return memcmp(this, &right, sizeof(FourCCWrapper)) == 0;
	}
};

template<>
struct hash<FourCCWrapper>
{	
	inline size_t operator()(const FourCCWrapper& value) const
	{
		return hash<int>()(*reinterpret_cast<const int*>(value.cc));
	}
};