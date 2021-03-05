/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
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

#pragma once

#import "ViewController.h"

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "ObservableReader.h"
#include "SeSelection.h"

using namespace keyple::common;
using namespace keyple::core::selection;

class UseCase_Calypso1_ExplicitSelectionAid_IOS final
: public ObservableReader::ReaderObserver,
  public std::enable_shared_from_this<UseCase_Calypso1_ExplicitSelectionAid_IOS> {


public:
    /**
     *
     */
    UseCase_Calypso1_ExplicitSelectionAid_IOS(ViewController *VC);

    /**
     *
     */
    void start();

private:
      /**
       *
       */
      const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
          typeid(UseCase_Calypso1_ExplicitSelectionAid_IOS));

      /**
       *
       */
      std::shared_ptr<SeSelection> seSelection;

      /**
       *
       */
      std::shared_ptr<SeReader> poReader;
      
      /**
       *
       */
      ViewController *mVC;

      /**
       *
       */
      void success(const std::string& msg);

      /**
       *
       */
      void failure(const std::string& msg);

      /**
       *
       */
      void reset();

      /**
       *
       */
      void update(const std::shared_ptr<ReaderEvent> event) override;
};

