#pragma once

#include <string>
#include <vector>
#include <memory>
#include "stringhelper.h"

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
namespace opencard {
    namespace opt {
        namespace util {
            /* Code derived from the original OpenCard Framework */

            /*
             * Copyright (c) 1997 - 1999 IBM Corporation.
             *
             * Redistribution and use in source (source code) and binary (object code) forms, with or without
             * modification, are permitted provided that the following conditions are met: 1. Redistributed
             * source code must retain the above copyright notice, this list of conditions and the disclaimer
             * below. 2. Redistributed object code must reproduce the above copyright notice, this list of
             * conditions and the disclaimer below in the documentation and/or other materials provided with the
             * distribution. 3. The name of IBM may not be used to endorse or promote products derived from this
             * software or in any other form without specific prior written permission from IBM. 4.
             * Redistribution of any modified code must be labeled "Code derived from the original OpenCard
             * Framework".
             *
             * THIS SOFTWARE IS PROVIDED BY IBM "AS IS" FREE OF CHARGE. IBM SHALL NOT BE LIABLE FOR
             * INFRINGEMENTS OF THIRD PARTIES RIGHTS BASED ON THIS SOFTWARE. ANY EXPRESS OR IMPLIED WARRANTIES,
             * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
             * PARTICULAR PURPOSE ARE DISCLAIMED. IBM DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED IN THIS
             * SOFTWARE WILL MEET THE USER'S REQUIREMENTS OR THAT THE OPERATION OF IT WILL BE UNINTERRUPTED OR
             * ERROR-FREE. IN NO EVENT, UNLESS REQUIRED BY APPLICABLE LAW, SHALL IBM BE LIABLE FOR ANY DIRECT,
             * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
             * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
             * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
             * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
             * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. ALSO, IBM IS UNDER NO OBLIGATION TO
             * MAINTAIN, CORRECT, UPDATE, CHANGE, MODIFY, OR OTHERWISE SUPPORT THIS SOFTWARE.
             */

            /**
             * This class represents Tags as defined in the Basic Encoding Rules for ASN.1 defined in ISO 8825.
             * A tag consists of two class bits (0 - 4), a flag indicating whether the TLV is constructed or
             * primitive.
             * <p>
             * 
             * The encoding is like this (C = class bit, c = composed flag, X = data bit):
             * 
             * <pre>
             * Range from 0x0 - 0x1E:
             * C C c X X X X X
             * <br>
             * Range from 0x1F - 0x80:
             * C C c 1 1 1 1 1 0 X X X X X X X
             * <br>
             * Range from 0x81 - 0x4000:
             * C C c 1 X X X X 1 X X X X X X X 0 X X X X X X X
             * <br>
             * Range from 0x4001 - 0x200000:
             * C C c 1 X X X X 1 X X X X X X X 1 X X X X X X X 0 X X X X X X X
             * <br>
             * Range from 0x200001 - 0x10000000:
             * C C c 1 X X X X 1 X X X X X X X 1 X X X X X X X 0 X X X X X X X 0 X X X X X X X
             * </pre>
             * 
             * @author Thomas Schaeck
             * 
             * @see opencard.opt.util.TLV
             */

            class Tag : public std::enable_shared_from_this<Tag> {
                /** tag number */
            private:
                int tag = 0;
                /** tag class, 0 - 4 */
                char tagclass = 0;
                /** constructed flag */
                bool constructed = false;

                /** tag classes definition */
            public:
                static const char UNIVERSAL = static_cast<char>(0x00);
                static const char APPLICATION = static_cast<char>(0x01);
                static const char CONTEXT = static_cast<char>(0x02);
                static const char PRIVATE = static_cast<char>(0x03);

                /** tag type definition */
                static constexpr bool PRIMITIVE = false;
                static constexpr bool CONSTRUCTED = true;

                /**
                 * Create a null tag.
                 * <p>
                 */
                Tag();

                /**
                 * Clone a tag.
                 * <p>
                 * 
                 * @param t The <tt>Tag</tt> object to be cloned.
                 */
                Tag(std::shared_ptr<Tag> t);

                /**
                 * Creates a tag from a given tag value, class and constructed flag.
                 * <p>
                 * 
                 * @param tag An integer representing the value of the tag.
                 * @param tagClass A byte value representing the class of the tag.
                 * @param constructed A boolean value <tt>true</tt> signals that the tag is constructed,
                 *        <tt>false</tt> signals that the tag is primitive.
                 */
                Tag(int tag, char tagClass, bool constructed);

                /**
                 * Create a tag from binary representation.
                 * <p>
                 * 
                 * @param binary The byte array from which the tag shall be generated.
                 * @param offset An integer value giving the offset into the the byte array from where to start.
                 */
                Tag(std::vector<char> &binary, std::vector<int> &offset);

                Tag(std::vector<char> &binary);

                /**
                 * Return the number of bytes which are required to BER-code the tag value.
                 * <p>
                 * 
                 * @return An integer giving the number of bytes.
                 */
                virtual int size();

                /**
                 * Initialize the <tt>Tag</tt> object from a BER-coded binary representation.
                 * <p>
                 * 
                 * @param binary A byte array containing the BER-coded tag.
                 * @param offset An integer giving an offset into the byte array from where to start.
                 */
                virtual void fromBinary(std::vector<char> &binary, std::vector<int> &offset);

                /**
                 * Gets a byte array representing the tag.
                 * 
                 * @return the tag as a byte array
                 */
                virtual std::vector<char> getBytes();

                /**
                 * Convert the tag to binary representation.
                 * <p>
                 * 
                 * @param binary A byte array to which the BER-coded binary representation of the tag shall be
                 *        written.
                 * @param offset An integer value giving an offset into the byte array from where to start.
                 */
                virtual void toBinary(std::vector<char> &binary, std::vector<int> &offset);

                /**
                 * Set the tag number, class and constructed flag of this <tt>Tag</tt> to the given values.
                 * <p>
                 * 
                 * @param tag An integer value giving the tag value.
                 * @param tagclass A byte value giving the class.
                 * @param constructed A boolean representing the constructed flag.
                 */
                virtual void set(int tag, char tagclass, bool constructed);

                /**
                 * Set the constructed flag of this <tt>Tag</tt> to the given value.
                 * <p>
                 * 
                 * @param constructed A boolean representing the constructed flag.
                 */
                virtual void setConstructed(bool constructed);

                /**
                 * Get the code of the tag.
                 * <p>
                 * 
                 * @return An integer value representing the tag's code.
                 */
                virtual int code();

                /**
                 * Check whether this <tt>Tag</tt> is constructed.
                 * <p>
                 * 
                 * @return <tt>true</tt> if it is constructed, <tt>false</tt> otherwise.
                 */
                virtual bool isConstructed();

                /**
                 * Compute a hash code for this tag.
                 * <p>
                 * 
                 * @return An integer value representing the hash code.
                 */
                virtual int hashCode();

                /**
                 * Check for equality.
                 * <p>
                 * 
                 * @return <tt>true</tt>, if this <tt>Tag</tt> instance equals the given tag, <tt>false</tt>
                 *         otherwise.
                 */
                virtual bool equals(std::shared_ptr<void> o);

                /**
                 * Get a string representation for this tag.
                 * <p>
                 * 
                 * @return The string representation.
                 */
                virtual std::string toString();
            };

        }
    }
}
