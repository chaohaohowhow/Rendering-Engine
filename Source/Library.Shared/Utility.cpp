#include "pch.h"
#include "Utility.h"

using namespace std;

namespace Library
{
	// TODO: Understand this function better
	void Utility::LoadBinaryFile(const string& filename, vector<char>& data)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			wstringstream filePath;
			filePath << "ERROR: " << std::filesystem::current_path().wstring() << L"\\" << filename.c_str() << " does not exist!" << std::endl;
			OutputDebugStringW(filePath.str().c_str());
			throw runtime_error("Could not open file.");
		}

		// Set position to the last character
		file.seekg(0, ios::end);
		// Gets the position and set it as the size
		size_t size = static_cast<size_t>(file.tellg());

		if (size > 0)
		{
			data.resize(size);
			file.seekg(0, ios::beg);
			file.read(&data.front(), size);
		}

		file.close();
	}
}