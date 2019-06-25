#pragma once

#include <vector>
#include <algorithm>
#include <memory>

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {


                    /**
                     * A class dedicated to managing the security settings involved in managing secure sessions.
                     * <p>
                     * The object provides default values when instantiated, they can be modified with the putKeyInfo
                     * method.
                     * <p>
                     * The getKeyInfo method returns the specified setting value.
                     */
                    class SecuritySettings : public std::enable_shared_from_this<SecuritySettings> {
                        /**
                         * List of key information that can be provided when the secure session is created.
                         */
                    public:
                        enum class DefaultKeyInfo {
                            /** KIF for personalization used when not provided by the PO */
                            SAM_DEFAULT_KIF_PERSO,
                            /** KIF for load used when not provided by the PO */
                            SAM_DEFAULT_KIF_LOAD,
                            /** KIF for debit used when not provided by the PO */
                            SAM_DEFAULT_KIF_DEBIT,
                            /** Key record number to use when KIF/KVC is unavailable */
                            SAM_DEFAULT_KEY_RECORD_NUMBER
                        };

                        /** The default KIF value for personalization */
                    private:
                        static const char DEFAULT_KIF_PERSO = static_cast<char>(0x21);
                        /** The default KIF value for loading */
                        static const char DEFAULT_KIF_LOAD = static_cast<char>(0x27);
                        /** The default KIF value for debiting */
                        static const char DEFAULT_KIF_DEBIT = static_cast<char>(0x30);
                        /** The default key record number */
                        static const char DEFAULT_KEY_RECORD_NUMER = static_cast<char>(0x00);
                        /** List of authorized KVCs */
                        std::vector<Byte> authorizedKvcList;

                        /** Enummap containing the key information */
                        const std::shared_ptr<EnumMap<DefaultKeyInfo, Byte>> keySettings = std::make_shared<EnumMap<DefaultKeyInfo, Byte>>(DefaultKeyInfo::typeid);

                        /**
                         * Constructor.
                         * <p>
                         * Initialize default values
                         */
                    public:
                        SecuritySettings();

                        /**
                         * Associates the specified value with the specified key information in the keySettings map. If
                         * the map previously contained a mapping for this key, the old value is replaced.
                         * 
                         * @param keyInfo - the keyInfo with which the specified value is to be associated
                         * @param value - the value to be associated with the specified key
                         * @return the previous value associated with specified key, or null if there was no mapping for
                         *         key.
                         */
                        virtual char putKeyInfo(DefaultKeyInfo keyInfo, Byte value);

                        /**
                         * Returns the value to which the specified key is mapped, or null if this map contains no
                         * mapping for the key.
                         * 
                         * @param keyInfo - the keyInfo whose associated value is to be returned
                         * @return the value to which the specified key is mapped, or null if this map contains no
                         *         mapping for the key
                         */
                        virtual char getKeyInfo(DefaultKeyInfo keyInfo);

                        /**
                         * Provides a list of authorized KVC
                         *
                         * If this method is not called, the list will remain empty and all KVCs will be accepted.
                         *
                         * If a list is provided and a PO with a KVC not belonging to this list is presented, a
                         * {@link KeypleCalypsoSecureSessionUnauthorizedKvcException} will be raised.
                         *
                         * @param authorizedKvcList the list of authorized KVCs
                         */
                        virtual void setAuthorizedKvcList(std::vector<Byte> &authorizedKvcList);

                        /**
                         * CHeck if the provided kvc value is authorized or not.
                         * <p>
                         * If no list of authorized kvc is defined (authorizedKvcList null), all kvc are authorized.
                         * 
                         * @param kvc to be tested
                         * @return true if the kvc is authorized
                         */
                        virtual bool isAuthorizedKvc(char kvc);
                    };

                }
            }
        }
    }
}
