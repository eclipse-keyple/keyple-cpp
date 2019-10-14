#include <fstream>

#include "Character.h"
#include "Integer.h"
#include "Properties.h"
#include "stringbuilder.h"

Properties::Properties()
{
}

void Properties::load(std::ifstream& inStream)
{
    std::string line;

	try
    {

        while (std::getline(inStream, line))
        {
            char c  = 0;
            int pos = 0;

            /* Leading whitespaces must be deleted first */
            while (pos < (int)line.length() && Character::isWhitespace(c = line.at(pos)))
                pos++;

            /* If empty line or begins with a comment character, skip this line */
            if ((line.length() - pos) == 0 || line.at(pos) == '#' || line.at(pos) == '!')
                continue;

            /*
			 * The characters up to the next Whitespace, ':', or '='
			 * describe the key.  But look for escape sequences.
			 * Try to short-circuit when there is no escape char.
			 */
            int start        = pos;
            bool needsEscape = line.find('\\', pos) == std::string::npos ? false : true;
            StringBuilder key;

            while (pos < (int)line.length() && !Character::isWhitespace(c = line.at(pos++)) && c != '=' && c != ':')
            {
                if (needsEscape && c == '\\')
                {
                    if (pos == (int)line.length())
                    {
                        /*
						 * The line continues on the next line.  If there
						 * is no next line, just treat it as a key with an
						 * empty value.
						 */
                        if (!std::getline(inStream, line))
                            line = "";
                        pos = 0;

                        while (pos < (int)line.length() && Character::isWhitespace(c = line.at(pos)))
                            pos++;
                    }
                    else
                    {
                        c = line.at(pos++);
                        switch (c)
                        {
                        case 'n':
                            key.append('\n');
                            break;
                        case 't':
                            key.append('\t');
                            break;
                        case 'r':
                            key.append('\r');
                            break;
                        case 'u':
                            if (pos + 4 <= (int)line.length())
                            {
                                char uni = (char)Integer::parseInt(line.substr(pos, pos + 4), 16);
                                key.append(uni);
                                pos += 4;
                            } // else throw exception?
                            break;
                        default:
                            key.append(c);
                            break;
                        }
                    }
                }
                else if (needsEscape)
                    key.append(c);
            }

            bool isDelim = (c == ':' || c == '=');
            std::string keyString;

            if (needsEscape)
                keyString = key.toString();
            else if (isDelim || Character::isWhitespace(c))
                keyString = line.substr(start, pos - 1);
            else
                keyString = line.substr(start, pos);

            while (pos < (int)line.length() && Character::isWhitespace(c = line.at(pos)))
                pos++;

            if (!isDelim && (c == ':' || c == '='))
            {
                pos++;
                while (pos < (int)line.length() && Character::isWhitespace(c = line.at(pos)))
                    pos++;
            }

            // Short-circuit if no escape chars found.
            if (!needsEscape)
            {
                props.insert(std::pair<std::string, std::string>(keyString, line.substr(pos)));
                continue;
            }

            // Escape char found so iterate through the rest of the line.
            StringBuilder element(line.length() - pos);
            while (pos < (int)line.length())
            {
                c = line.at(pos++);
                if (c == '\\')
                {
                    if (pos == (int)line.length())
                    {
                        // The line continues on the next line.
                        if (!std::getline(inStream, line))
                            // We might have seen a backslash at the end of
                            // the file.  The JDK ignores the backslash in
                            // this case, so we follow for compatibility.
                            break;

                        pos = 0;
                        while (pos < (int)line.length() && Character::isWhitespace(c = line.at(pos)))
                            pos++;
                        element.ensureCapacity(line.length() - pos + element.length());
                    }
                    else
                    {
                        c = line.at(pos++);
                        switch (c)
                        {
                        case 'n':
                            element.append('\n');
                            break;
                        case 't':
                            element.append('\t');
                            break;
                        case 'r':
                            element.append('\r');
                            break;
                        case 'u':
                            if (pos + 4 <= (int)line.length())
                            {
                                char uni = (char)Integer::parseInt(line.substr(pos, pos + 4), 16);
                                element.append(uni);
                                pos += 4;
                            } // else throw exception?
                            break;
                        default:
                            element.append(c);
                            break;
                        }
                    }
                }
                else
                    element.append(c);
            }

            props.insert(std::pair<std::string, std::string>(keyString, element.toString()));
        }
    } catch (std::ios_base::failure e) {
    }
}

std::string Properties::getProperty(std::string key)
{
    return (std::string)props[key];
}

std::string Properties::getProperty(std::string key, std::string defaultValue)
{
    std::string prop = getProperty(key);
    if (prop.empty())
        prop = defaultValue;
    return prop;
}
