#pragma once

#include <vector>

class Arrays {
private:
	std::vector<char> privateArray;

public:
	static bool equals(const std::vector<char> &a1, const std::vector<char> &a2) {
		if (a1.size() != a2.size())
			return false;

		for (auto i1 = a1.begin(), i2 = a2.begin(); i1 != a1.end(); i1++, i2++){
			if (*i1 != *i2)
				return false;
		}

		return true;
	}

	static int hashCode(const std::vector<char> a) {
		int hash = 0;

		for (auto i = a.begin(); i != a.end(); i++)
			hash ^= *i;

		return hash;
	}

	static std::vector<char> &copyOfRange(const std::vector<char> &original, int from, int to) {
		static std::vector<char> privateArray;
		privateArray.reserve(to - from);
		std::copy(&original[from], &original[to], privateArray.begin());
		return privateArray;
	}
};
