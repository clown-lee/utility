#include <fstream>
#include <string>
#include <iterator>
inline std::string LoadFileContent_Binary(const char*path)
{
	std::ifstream in("readme.txt", std::ios::in| std::ios::binary);
	std::istreambuf_iterator<char> beg(in), end;
	std::string strdata(beg, end);
	in.close();
	return strdata;
}
