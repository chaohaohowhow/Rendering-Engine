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
			throw runtime_error("Could not open file.");
		}

		file.seekg(0, ios::end);
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