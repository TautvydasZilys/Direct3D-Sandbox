#include "PrecompiledHeader.h"
#include "..\..\Source\Core\Tools.h"
#include "ModelProcessor.h"
#include "ShaderReflector.h"

int CALLBACK wWinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPWSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	int argc;
	wchar_t** argv = CommandLineToArgvW(lpCmdLine, &argc);

	if (argc != 3)
	{
		wchar_t exeName[MAX_PATH];
		GetModuleFileName(nullptr, exeName, MAX_PATH);

		wcout << L"Invalid number of parameters! Usage: " << exeName << L" <shaderDirectory> <modelInputDirectory> <modelOutputDirectory>" << endl;
	}

	for (auto& shaderPath : Tools::GetFilesInDirectory(argv[0], L"*.cso", true))
	{
		ShaderReflector::ReflectShader(shaderPath);
	}

	const wstring modelOutputPath(argv[2]);
	if (!Tools::DirectoryExists(modelOutputPath))
	{
		CreateDirectory(modelOutputPath.c_str(), nullptr);
	}

	for (auto& modelPath : Tools::GetFilesInDirectory(argv[1], L"*.obj", true))
	{
		ModelProcessor::ProcessModel(modelPath, modelOutputPath);
	}

	LocalFree(argv);
	return 0;
}