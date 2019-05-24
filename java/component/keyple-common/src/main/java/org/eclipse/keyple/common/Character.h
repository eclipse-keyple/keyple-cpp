#pragma once

class Character
{
public:
	static char digit(char ch, int radix)
    {
		(void)radix;

        if (ch >= '0' && ch <= '9')
            return ch - '0';
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 10;
        if (ch >= 'a' && ch <= 'z')
            return ch - 'a' + 10;

        return 0;
	}
};
