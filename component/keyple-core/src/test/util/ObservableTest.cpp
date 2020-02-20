#include "ObservableTest.h"

using namespace keyple::core::util;

        namespace keyple {
            namespace core {
                namespace util {

                using namespace testing;

                ObservableTest::Event::Event(const std::string &name) : name(name) {
                }

                std::string ObservableTest::Event::getName() {
                    return name;
                }

                std::string ObservableTest::Event::toString() {
                    return "Event{" + name + "}";
                }

                ObservableTest::EventPlus::EventPlus(const std::string &name) : Event(name) {
                }

                std::string ObservableTest::EventPlus::toString() {
                    return "EventPlus{" + getName() + "}";
                }

                //int ObservableTest::Observer::getNbCalls() {
                //    return nbCalls;
                //}

                //void ObservableTest::Observer::update(std::shared_ptr<Event> arg) {
                //    // System.out.println(name + " received" + event);
                //    nbCalls += 1;
                //}

                void ObservableTest::sample()
                {
                    std::shared_ptr<Observable<std::shared_ptr<Event>>> pub = std::make_shared<Observable<std::shared_ptr<Event>>>();
                    //std::shared_ptr<Observer> sub1 = std::make_shared<Observer>();
                    //std::shared_ptr<Observer> sub2 = std::make_shared<Observer>();
                    //pub->addObserver(sub1);
                    //pub->notifyObservers(std::make_shared<Event>("ev1"));
                    //pub->addObserver(sub2);
                    //pub->notifyObservers(std::make_shared<EventPlus>("ev2"));
                    //ASSERT_EQ(2, sub1->getNbCalls());
                    //ASSERT_EQ(1, sub2->getNbCalls());
                }
            }
        }
    }

TEST(ObservableTest, testA) 
{
    std::shared_ptr<ObservableTest> LocalTest = std::make_shared<ObservableTest>();
    LocalTest->sample();
}
