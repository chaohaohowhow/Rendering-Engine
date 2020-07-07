#pragma once
using namespace std;

namespace Library
{
	class Utility final
	{
	public:
		Utility() = delete;
		Utility(const Utility&) = delete;
		Utility& operator=(const Utility&) = delete;
		Utility(Utility&&) = delete;
		Utility& operator=(Utility&&) = delete;
		~Utility() = default;

		static void LoadBinaryFile(const std::string& filename, std::vector<char>& data);
		static std::tuple<std::string, std::string> GetFileNameAndDirectory(const std::string& inputPath);
	};
}