/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#include <list>

#include "SeResponse.hpp"
#include "SeResponseSet.hpp"

using namespace keyple::seproxy;
using namespace keyple::seproxy::message;

SeResponseSet::SeResponseSet(std::list<SeResponse> &seResponses)
: seResponses(seResponses)
{
}

SeResponseSet::~SeResponseSet()
{
}

std::list<SeResponse> SeResponseSet::getResponses()
{
    return seResponses;
}
//
//    /**
//     * Create an {@link SeResponseSet} from a single {@link SeResponse}
//     *
//     * @param response single {@link SeRequest}
//     */
//  public
//    SeResponseSet(SeResponse response)
//    {
//        List<SeResponse> seResponses = new ArrayList<SeResponse>();
//        seResponses.add(response);
//        this.seResponses = seResponses;
//    }
//
//    /**
//     * Return the response when the list contains only one
//     *
//     * @return response
//     */
//  public
//    SeResponse getSingleResponse()
//    {
//        if (seResponses.size() != 1)
//        {
//            throw new IllegalStateException("This method only support ONE element");
//        }
//        return seResponses.get(0);
//    }
//
//    @Override public String toString()
//    {
//        return seResponses.size() + " SeReponse(s)";
//    }
//}
