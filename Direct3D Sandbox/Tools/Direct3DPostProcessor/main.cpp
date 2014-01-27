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

	if (argc != 4)
	{
		wcout << L"Invalid number of parameters! Usage: " << argv[0] << L"<shaderDirectory> <modelInputDirectory> <modelOutputDirectory>" << endl;
	}

	for (auto& shaderPath : Tools::GetFilesInDirectory(argv[1], L"*.cso", true))
	{
		ShaderReflector::ReflectShader(shaderPath);
	}

	const wstring modelOutputPath(argv[3]);
	if (!Tools::DirectoryExists(modelOutputPath))
	{
		CreateDirectory(modelOutputPath.c_str(), nullptr);
	}

	for (auto& modelPath : Tools::GetFilesInDirectory(argv[2], L"*.obj", true))
	{
		ModelProcessor::ProcessModel(modelPath, modelOutputPath);
	}

	LocalFree(argv);
	return 0;
}