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

#import "IOSTerminalImpl.h"

#include "LoggerFactory.h"
#include "IOSTerminalException.h"

@implementation IOSTerminalImpl

using namespace keyple::common;
using namespace keyple::plugin::ios;

auto logger = LoggerFactory::getLogger(typeid(IOSTerminalImpl));

- (id) init
{
    logger->trace("init\n");

    if (!(self = [super init]))
        return nil;

    self.mConnected = false;
    self.mCardPresent = false;
    self.mCancelledOrTimeout = false;
    self.mTags = nil;
    self.mSession = nil;

    return self;
}

- (void) waitForCardPresent
{
    logger->trace("waitForCardPresent\n");

    self.mCancelledOrTimeout = false;

    if (@available(iOS 13.0, *)) {
        self.mSession = [[NFCTagReaderSession alloc]
                          initWithPollingOption:NFCPollingISO14443
                          delegate:self
                          queue:nil];

        [self.mSession beginSession];
    } else {
        // Fallback on earlier versions
        logger->error("waitForCardPresent - iOS 13.0 or above required\n");
    }
}

- (void) stopWaitForCard
{
    logger->trace("stopWaitForCard\n");
}

- (void) waitForCardAbsent
{
    logger->trace("stopWaitForCard\n");

    if (@available(iOS 13.0, *)) {
        [self.mSession invalidateSession];
    } else {
        // Fallback on earlier versions
        logger->error("stopWaitForCard - iOS 13.0 or above required\n");
    }
}

- (bool) isCardPresent
{
    return self.mCardPresent;
}

- (bool) didUserCancelOrTimeout
{
    return self.mCancelledOrTimeout;
}

- (void) openAndConnect
{
    logger->trace("openAndConnect\n");

    self.mConnected = false;

    [self.mSession connectToTag:self.mTags.firstObject completionHandler:^(NSError *error) {
        if (error != nil) {
            logger->error("openAndConnect - completionHandler - error ? %\n",
                          std::string([[error localizedDescription] UTF8String]));
            throw IOSTerminalException("Error connecting to tag");
        }
        self.mConnected = true;
    }];

    while (!self.mConnected);

    logger->trace("openAndConnect - connected\n");
}

- (void) closeAndDisconnect
{
    logger->trace("closeAndDisconnect\n");

    self.mConnected = false;
}

- (NSData *) transmitApdu:(NSData *)apdu
{
    logger->trace("transmitApdu\n");

    self.mRApduReady = false;

    NFCISO7816APDU *isoApdu = [[NFCISO7816APDU alloc] initWithData:apdu];
    [self.mTags.firstObject sendCommandAPDU:isoApdu
                            completionHandler:^(NSData *responseData,
                                                uint8_t sw1,
                                                uint8_t sw2,
                                                NSError *error) {
        self.mRApdu = [responseData mutableCopy];
        [self.mRApdu appendBytes:&sw1 length:1];
        [self.mRApdu appendBytes:&sw2 length:1];
        self.mRApduReady = true;
    }];

    while (!self.mRApduReady);
    return self.mRApdu;
}

/* CALLBACKS ------------------------------------------------------------------------------------ */

- (void) tagReaderSession:(nonnull NFCTagReaderSession *)session
            didDetectTags:(nonnull NSArray<__kindof id<NFCTag>> *)tags API_AVAILABLE(ios(13.0))
{
    logger->trace("tagReaderSession - detected % tag(s)\n", (unsigned long)tags.count);

    self.mTags = tags;
    self.mCardPresent = true;
}

- (void) tagReaderSessionDidBecomeActive:(NFCTagReaderSession *)session API_AVAILABLE(ios(13.0))
{
    logger->trace("tagReaderSessionDidBecomeActive\n");
}

- (void) tagReaderSession:(NFCTagReaderSession *)session didInvalidateWithError:(NSError *)error
{
    logger->trace("tagReaderSession - didInvalidateWithError: %\n",
                  std::string([[error localizedDescription] UTF8String]));

    /* Guard time to make sure NFCTagReaderSession can be called again immediately ...*/
    [NSThread sleepForTimeInterval:0.5f];

    self.mConnected = false;
    self.mCardPresent = false;
    self.mCancelledOrTimeout = true;
    self.mTags = nil;
    self.mSession = nil;
}

@end
