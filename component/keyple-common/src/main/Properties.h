/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <istream>
#include <map>
#include <string>

#include "Export.h"

namespace keyple {
namespace common {

class EXPORT Properties {
private:
    /*
     *
     */
    std::map<std::string, std::string> props;

public:
    /**
     * Creates a new empty property list with no default values.
     */
    Properties();

    /**
     * Reads a property list from an input stream.  The stream should
     * have the following format: <br>
     *
     * An empty line or a line starting with <code>#</code> or
     * <code>!</code> is ignored.  An backslash (<code>\</code>) at the
     * end of the line makes the line continueing on the next line
     * (but make sure there is no whitespace after the backslash).
     * Otherwise, each line describes a key/value pair. <br>
     *
     * The chars up to the first whitespace, = or : are the key.  You
     * can include this caracters in the key, if you precede them with
     * a backslash (<code>\</code>). The key is followed by optional
     * whitespaces, optionally one <code>=</code> or <code>:</code>,
     * and optionally some more whitespaces.  The rest of the line is
     * the resource belonging to the key. <br>
     *
     * Escape sequences <code>\t, \n, \r, \\, \", \', \!, \#, \ </code>(a
     * space), and unicode characters with the
     * <code>\\u</code><em>xxxx</em> notation are detected, and
     * converted to the corresponding single character. <br>
     *
     *
     * <pre># This is a comment
     * key     = value
     * k\:5      \ a string starting with space and ending with newline\n
     * # This is a multiline specification; note that the value contains
     * # no white space.
     * weekdays: Sunday,Monday,Tuesday,Wednesday,\\
     *           Thursday,Friday,Saturday
     * # The safest way to include a space at the end of a value:
     * label   = Name:\\u0020</pre>
     *
     * @param inStream the input stream
     * @throws IOException if an error occurred when reading the input
     * @throws NullPointerException if in is null
     */
    void load(std::ifstream& inStream);

    /**
     * Gets the property with the specified key in this property list.
     * If the key is not found, the default property list is searched.
     * If the property is not found in the default, null is returned.
     *
     * @param key The key for this property
     * @return the value for the given key, or null if not found
     * @throws ClassCastException if this property contains any key or
     *         value that isn't a string
     * @see #defaults
     * @see #setProperty(String, String)
     * @see #getProperty(String, String)
     */
    std::string getProperty(std::string key);

    /**
     * Gets the property with the specified key in this property list.  If
     * the key is not found, the default property list is searched.  If the
     * property is not found in the default, the specified defaultValue is
     * returned.
     *
     * @param key The key for this property
     * @param defaultValue A default value
     * @return The value for the given key
     * @throws ClassCastException if this property contains any key or
     *         value that isn't a string
     * @see #defaults
     * @see #setProperty(String, String)
     */
    std::string getProperty(std::string key, std::string defaultValue);
};

}
}
