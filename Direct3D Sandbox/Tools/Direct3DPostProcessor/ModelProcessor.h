#pragma once

namespace ModelProcessor
{
	void ProcessModel(const wstring& path, const wstring& outputPath);
	void ProcessAnimatedModel(const wstring& rootPath, const wstring& outputPath);
}