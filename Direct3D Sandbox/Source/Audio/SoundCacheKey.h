#pragma once

#include "PrecompiledHeader.h"

struct SoundCacheKey
{
	wstring path;
	bool loopForever;
	bool hasReverb;

	SoundCacheKey(const wstring& path, bool loopForever, bool hasReverb) :
		path(path), loopForever(loopForever), hasReverb(hasReverb)
	{
	}

	bool operator==(const SoundCacheKey& other) const
	{
		return path == other.path && loopForever == other.loopForever && hasReverb == other.hasReverb;
	}
};

template <>
struct hash<SoundCacheKey>
{
	size_t operator()(const SoundCacheKey& value)
	{
		return hash<wstring>()(value.path) ^ hash<int>()(value.loopForever + 2 * value.hasReverb);
	}
};
