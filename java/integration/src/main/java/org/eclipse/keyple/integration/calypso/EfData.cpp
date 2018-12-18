#include "EfData.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {

                    EfData::EfData(int recNumb, char sfi, int recSize) {
                        this->recNumb = recNumb;
                        this->sfi = sfi;
                        this->recSize = recSize;
                    }

                    int EfData::getRecNumb() {
                        return recNumb;
                    }

                    char EfData::getSfi() {
                        return sfi;
                    }

                    int EfData::getRecSize() {
                        return recSize;
                    }
                }
            }
        }
    }
}
