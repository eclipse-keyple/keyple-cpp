#include "KeyReference.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {

KeyReference::KeyReference(char kif, char kvc) : kif(kif), kvc(kvc)
{
}

char KeyReference::getKif()
{
        return kif;
}

char KeyReference::getKvc()
{
        return kvc;
}
}
}
}
}
