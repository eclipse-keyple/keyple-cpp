/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ObservablePlugin.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ObservablePluginMock : public ObservablePlugin {
public:
    void addObserver(const std::shared_ptr<PluginObserver> observer) override
    {
        observers.push_back(observer);
    }

    void removeObserver(const std::shared_ptr<PluginObserver> observer) override
    {
        observers.erase(std::remove(observers.begin(), observers.end(),
	                            observer), observers.end());
    }

    void notifyObservers(const std::shared_ptr<PluginEvent> event) override
    {
        (void)event;
    }

    void clearObservers() override
    {
        observers.clear();
    }

    int countObservers() const override
    {
        return observers.size();
    }

    const std::string& getName() const override
    {
        return name;
    }

    const std::map<const std::string, const std::string> getParameters() const
        override
    {
        return parameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        (void)key;
        (void)value;
    }

    void setParameters(const std::map<std::string, std::string>& parameters)
        override
    {
        (void)parameters;
    }

    const std::set<std::string> getReaderNames() const override
    {
        return readerNames;
    }

    std::set<std::shared_ptr<SeReader>>& getReaders() override
    {
        return readers;
    }

    const std::shared_ptr<SeReader> getReader(const std::string& name) const
        override
    {
        (void)name;

        return reader;
    }

private:
    std::vector<std::shared_ptr<PluginObserver>> observers;

    std::string name;

    std::map<const std::string, const std::string> parameters;

    std::set<std::string> readerNames;

    std::set<std::shared_ptr<SeReader>> readers;

    std::shared_ptr<SeReader> reader;
};

TEST(ObservablePluginTest, ObservablePlugin)
{
	ObservablePluginMock plugin;
}

