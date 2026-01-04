#include <string>
#include <format>
#include <iostream>
#include <algorithm>
using namespace std;
template <typename T>
	requires(std::same_as<T, int> || std::same_as<T, double>)
string fmt_number(T number)
{
	bool neg = number < 0;
	string unformatted;
	if constexpr (is_same_v<T, int>)
	{
		unformatted = format("{}", abs(number));
		string formatted = "";
		reverse(unformatted.begin(), unformatted.end());
		int i = 0;
		for (char c : unformatted)
		{
			if (i && !(i % 3))
			{
				formatted += ',';
			}
			formatted += c;
			i++;
		}
		reverse(formatted.begin(), formatted.end());
		if (neg)
		{
			formatted.insert(formatted.begin(), '-');
		}
		return formatted;
	}
	else
	{
		unformatted = format("{:.2f}", number);
		size_t dot = unformatted.find('.');
		return format("{}.{}", fmt_number<int>(stoi(unformatted.substr(0, dot))), unformatted.substr(dot + 1));
	}
}
int main()
{
	return 0;
}