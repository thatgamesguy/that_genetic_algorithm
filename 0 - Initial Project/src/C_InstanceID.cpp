#include "C_InstanceID.hpp"

int C_InstanceID::count = 0;

C_InstanceID::C_InstanceID(Object* owner) : Component(owner)
{
    id = count++;
}

int C_InstanceID::Get() const
{
    return id;
}
