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

	wcout << endl << "Starting up PostProcessor. Got " << argc << " arguments: " << endl;
	for (int i = 0; i < argc; i++)
	{
		wcout << L"\"" << argv[i] << L"\" ";
	}
	wcout << endl;

	if (argc != 3)
	{
		wchar_t exeName[MAX_PATH];
		GetModuleFileName(nullptr, exeName, MAX_PATH);

		wcout << L"Invalid number of arguments! Usage: " << exeName << L" <shaderDirectory> <modelInputDirectory> <modelOutputDirectory>" << endl;
	}
	
	wcout << endl;
	for (auto& shaderPath : Tools::GetFilesInDirectory(argv[0], L"*.cso", true))
	{
		wcout << "Reflecting on shader: " << shaderPath << endl;
		ShaderReflector::ReflectShader(shaderPath);
	}

	const wstring modelOutputPath(argv[2]);
	if (!Tools::DirectoryExists(modelOutputPath))
	{
		CreateDirectory(modelOutputPath.c_str(), nullptr);
	}
	
	wcout << endl;
	for (auto& modelPath : Tools::GetFilesInDirectory(argv[1], L"*.obj", true))
	{
		wcout << L"Processing model: " << modelPath << endl;
		ModelProcessor::ProcessModel(modelPath, modelOutputPath);
	}

	LocalFree(argv);
	return 0;
}