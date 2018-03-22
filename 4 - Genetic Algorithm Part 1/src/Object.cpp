#include "Object.hpp"

Object::Object() : queuedForRemoval(false)
{
    transform = AddComponent<C_Transform>();
    instanceID = AddComponent<C_InstanceID>();
}

void Object::Awake()
{
    for (auto& c : components)
    {
        c->Awake();
    }
}

void Object::Start()
{
    for (auto& c : components)
    {
        c->Start();
    }
}

void Object::Update(float timeDelta)
{
    for (const auto& component : components)
    {
        component->Update(timeDelta);
    }
}

void Object::LateUpdate(float timeDelta)
{
    for (const auto& component : components)
    {
        component->LateUpdate(timeDelta);
    }
}

void Object::Draw(Window& window)
{
    for (const auto& component : components)
    {
        component->Draw(window);
    }
}

void Object::QueueForRemoval()
{
    queuedForRemoval = true;
}

bool Object::IsQueuedForRemoval()
{
    return queuedForRemoval;
}
