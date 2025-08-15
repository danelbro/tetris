#include "SRS.h"

bool operator==(const SRSKey& left, const SRSKey& right)
{
    return left.test_ == right.test_ && left.type_ == right.type_
           && left.currentRotation_ == right.currentRotation_
           && left.newRotation_ == right.currentRotation_;
}
