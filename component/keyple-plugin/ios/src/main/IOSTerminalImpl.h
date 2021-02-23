/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#import <Foundation/Foundation.h>
#import <CoreNFC/CoreNFC.h>

API_AVAILABLE(ios(13.0))
@interface IOSTerminalImpl : NSObject <NFCTagReaderSessionDelegate>

/**
 *
 */
@property (strong, nullable, nonatomic) NSArray<__kindof id<NFCTag>> *mTags;

/**
 *
 */
@property (strong, nullable, nonatomic) NFCTagReaderSession *mSession;

/**
 *
 */
@property (atomic) BOOL mConnected;

/**
 *
 */
@property (atomic) BOOL mCardPresent;

/**
 *
 */
@property (atomic) BOOL mRApduReady;

/**
 *
 */
@property (atomic) BOOL mCancelledOrTimeout;

/**
 *
 */
@property (strong, nullable, nonatomic) NSMutableData *mRApdu;

/**
 *
 */
- (nonnull id) init;

/**
 *
 */
- (void) waitForCardPresent;

/**
 *
 */
- (void) stopWaitForCard;

/**
 *
 */
- (void) waitForCardAbsent;

/**
 *
 */
- (bool) isCardPresent;

/**
 *
 */
- (bool) didUserCancelOrTimeout;

/**
 *
 */
- (void) openAndConnect;

/**
 *
 */
- (void) closeAndDisconnect;

/**
 *
 */
- (void) tagReaderSession:(nonnull NFCTagReaderSession *)session
   didInvalidateWithError:(nonnull NSError *)error API_AVAILABLE(ios(13.0));

/**
 *
 */
- (void) tagReaderSessionDidBecomeActive:(nonnull NFCTagReaderSession *)session API_AVAILABLE(ios(13.0));

/**
 *
 */
- (void) tagReaderSession:(nonnull NFCTagReaderSession *)session
            didDetectTags:(nonnull NSArray<__kindof id<NFCTag>> *)tags API_AVAILABLE(ios(13.0));

/**
 *
 */
- (nullable NSData *) transmitApdu:(nonnull NSData *)apdu;

@end
