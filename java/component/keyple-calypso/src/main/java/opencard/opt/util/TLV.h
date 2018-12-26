#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace opencard { namespace opt { namespace util { class Tag; } } }

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
             * This class represents a TLV (Tag Length Value) structure. There are methods for creating trees
             * consisting of TLV objects from ASN.1 BER encoded byte sequences and for creating byte sequences
             * from TLV object trees. All manipulations are done on the tree structure.
             *
             * @author Thomas Schaeck
             * @version $Id: TLV.java,v 1.2 2005/09/19 10:21:22 asc Exp $
             * @see opencard.opt.util.Tag
             */

            class TLV : public std::enable_shared_from_this<TLV> {
            private:
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                std::shared_ptr<Tag> tag_Renamed; // Tag of this TLV
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                int length_Renamed = 0; // Length of this TLV's value
                std::vector<char> value; // Value of this TLV
                std::shared_ptr<TLV> parent; // Parent of this TLV
                std::shared_ptr<TLV> sibling; // Next sibling of this TLV
                std::shared_ptr<TLV> child; // First child of this TLV
                std::shared_ptr<TLV> lastChild; // Last child of this TLV

                /**
                 * Create an empty TLV.
                 */
            public:
                TLV();

                /**
                 * Create a <tt>TLV</tt> object from an ASN.1 BER encoded byte array.
                 * <p>
                 *
                 * @param binary A byte array containing the binary representation of a TLV structure, encoded
                 *        conforming to the ASN.1 Basic Encoding Rules defined in ISO 8825.
                 */
                TLV(std::vector<char> &binary);

                /**
                 * Create a <tt>TLV</tt> object from an ASN.1 BER encoded byte array.
                 * <p>
                 *
                 * @param binary A byte array containing the binary representation of a TLV structure, encoded
                 *        conforming to the ASN.1 Basic Encoding Rules defined in ISO 8825.
                 */
                TLV(std::vector<char> &binary, std::vector<int> &offset);

                /**
                 * Create a TLV object from the given <tt>Tag</tt> object and data.
                 * <p>
                 * <p>
                 * If the given <tt>Tag</tt> object has the constructed bit set, the result will be a TLV tree,
                 * otherwise it's just a primitive TLV that contains the data given in value field.
                 * <p>
                 *
                 * @param tag An instance of class <tt>Tag</tt> representing the tag field of the TLV to be
                 *        created.
                 * @param value An array of bytes representing the Value field of the TLV to be created.
                 */
                TLV(std::shared_ptr<Tag> tag, std::vector<char> &value);

                /**
                 * Create a primitive TLV object from a given tag and positive integer.
                 * <p>
                 *
                 * @param tag An instance of class <tt>Tag</tt> representing the tag field of the TLV to be
                 *        created.
                 * @param number An integer representing the Value field of the TLV to be created.
                 */
                TLV(std::shared_ptr<Tag> tag, int number);

                /**
                 * Create a constructed TLV object from the given <tt>Tag</tt> object and <tt>TLV</tt> object to
                 * be contained.
                 *
                 * @param tag An instance of class <tt>Tag</tt> representing the tag field of the TLV to be
                 *        created.
                 * @param tlv An instance of class <tt>TLV</tt> representing the Value field of the TLV to be
                 *        created.
                 */
                TLV(std::shared_ptr<Tag> tag, std::shared_ptr<TLV> tlv);

                /**
                 * Add the given <tt>TLV</tt> object to this <tt>TLV</tt> instance (only if constructed).
                 * <p>
                 *
                 * @param tlv The <tt>TLV</tt> object to be concatenated to this <tt>TLV</tt> instance.
                 */
                virtual std::shared_ptr<TLV> add(std::shared_ptr<TLV> tlv);

                /**
                 * Search for a given tag value and return the first TLV found.
                 * <p>
                 *
                 * @param tag The <tt>Tag</tt> object representing the tag to be searched for, <tt>null</tt> for
                 *        any tag.
                 * @param cursor A reference to a <tt>TLV</tt> object where the search should start; if
                 *        <tt>null</tt>, the search is started with the child of this <tt>TLV</tt> instance.
                 * @return The first <tt>TLV</tt> object found, which has the given tag value; <tt>null</tt> if
                 *         no match is found.
                 */
                virtual std::shared_ptr<TLV> findTag(std::shared_ptr<Tag> tag, std::shared_ptr<TLV> cursor);

                /**
                 * Read a <tt>TLV</tt> object from a binary representation.
                 * <p>
                 *
                 * @param binary A byte array containing the binary representation of a TLV structure, encoded
                 *        conforming to the ASN.1 Basic Encoding Rules defined in ISO 8825.
                 * @param offset An integer value giving the offset, where the binary representation starts.
                 * @param tlv The <tt>TLV</tt> object to be read from the binary representation.
                 * @param parent The <tt>TLV</tt> object representing the parent of the object to be read.
                 */
            private:
                static void fromBinary(std::vector<char> &binary, std::vector<int> &offset, std::shared_ptr<TLV> tlv, std::shared_ptr<TLV> parent);

                /**
                 * Return the number of bytes required for the coding of the length of this TLV as described in
                 * the ASN.1 Basic Encoding Rules.
                 * <p>
                 *
                 * @return An integer value giving the number of bytes.
                 */
                int lenBytes();

                /**
                 * Return the number of bytes required for coding the passed integer value as described in the
                 * ASN.1 Basic Encoding Rules.
                 * <p>
                 *
                 * @param length An integer value.
                 * @return An integer value giving the number of bytes.
                 */
                static int lenBytes(int length);

                /**
                 * Get the length of this TLV's value field in bytes.
                 * <p>
                 *
                 * @return An integer giving the length.
                 */
            public:
                virtual int length();

                /**
                 * BER-code the length of this TLV.
                 * <p>
                 *
                 * @param length The length to be encoded
                 * @return The BER-coded length field
                 */
                static std::vector<char> lengthToBinary(int length);

                /**
                 * Set the value field of this TLV from the byte array.
                 * <p>
                 *
                 * @param newValue The byte array for the value field.
                 */
                virtual void setValue(std::vector<char> &newValue);

                /**
                 * Get the tag of this TLV.
                 * <p>
                 *
                 * @return The <tt>Tag</tt> object of this <tt>TLV</tt> object.
                 */
                virtual std::shared_ptr<Tag> tag();

                /**
                 * BER-code this TLV.
                 * <p>
                 *
                 * @return A byte array containing the BER-coded representation of this <tt>TLV</tt> instance.
                 */
                virtual std::vector<char> toBinary();

                /**
                 * BER-code this TLV's value field.
                 * <p>
                 *
                 * @return A byte array containing the BER-coded binary representation of the value field of
                 *         this <tt>TLV</tt> instance.
                 */
                virtual std::vector<char> toBinaryContent();

                /**
                 * Convert this TLV to it's BER-coded representation.
                 * <p>
                 *
                 * @param binary The byte array to which the BER-coded representation shall be written.
                 * @param offset An integer giving the offset into the byte array, from where the binary
                 *        representation shall start.
                 * @param max An integer giving the index of the last valid byte.
                 */
            private:
                void toBinaryHelper(std::vector<char> &binary, std::vector<int> &offset, int max);

                /**
                 * Convert this TLV's value field to it's BER-coded representation.
                 * <p>
                 *
                 * @param binary The byte array to which the BER-coded representation of this <tt>TLV</tt>
                 *        instance shall be written.
                 * @param offset An integer giving the offset into the byte array, from where the BER-coded
                 *        representation shall start.
                 * @param max An integer giving the index of the last valid byte.
                 */
                void toBinaryHelperContent(std::vector<char> &binary, std::vector<int> &offset, int max);

                /**
                 * Convert the length of this TLV to it's binary representation according to the ASN.1 Basic
                 * Encoding Rules defined in ISO 8825.
                 * <p>
                 *
                 * @param binary The byte array to which the BER-coded length field shall be added.
                 * @param offset The offset, where the BER-coded length field shall be added.
                 */
                void toBinaryLength(std::vector<char> &binary, std::vector<int> &offset);

                /**
                 * Convert a TLV to a string.
                 * <p>
                 *
                 * @return A <tt>String</tt> object representing this <tt>TLV</tt> object.
                 */
            public:
                virtual std::string toString();

                /**
                 * Convert a TLV to a string.
                 * <p>
                 *
                 * @param ht A <tt>Hashtable</tt> object mapping <tt>Tag</tt> objects to <String> objects.
                 * @param level An integer value giving the indention level to be used.
                 * @return A <tt>String</tt> object representing this <tt>TLV</tt> object.
                 */
                virtual std::string toString(std::unordered_map ht, int level);

                /**
                 * Get the value field of this TLV as a byte array.
                 * <p>
                 *
                 * @return A byte array representing the value field of this <tt>TLV</tt> instance;
                 *         <tt>null</tt> if the TLV is constructed.
                 */
                virtual std::vector<char> valueAsByteArray();

                /**
                 * Get the value of this TLV as a positive integer number.
                 * <p>
                 *
                 * @return An integer representing the value (unsigned int) of this <tt>TLV</tt> instance's
                 *         value field.
                 */
                virtual int valueAsNumber();
            }; // class TLV

        }
    }
}
