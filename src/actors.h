#include "raylib.h"

#include <array>
#include <unordered_set>
#include <vector>
#include "mail.h"

#pragma once

class component;
class actor;

extern int NextActorId;
extern int NextComponentId;
extern std::array<actor *, 100> Actors;
extern std::array<component *, 500> Components;

class actor
{

protected:
    void Remove();

public:
    Vector2 Position{0.0f, 0.0f};
    int Id = NextActorId++;
    int ParentId = -1;
    std::vector<int> ChildrenIds;
    std::vector<int> ComponentIds;
    std::unordered_set<int> ComponentIdsPendingRemoval;
    std::unordered_set<int> ChildrenIdsPendingRemoval;

    void Update(float DeltaTime);
    void Display();
    bool SendUp(const mail &Mail);
    bool SendDown(const mail &Mail);
    bool SendDirect(const mail &Mail);

    void AddActor(actor *newActor);
    void AddComponent(component *newComponent);

    template <typename component_type>
    component_type *GetComponent()
    {
        for (int ComponentId : ComponentIds)
        {
            component *Component = Components[ComponentId];
            if (component_type *desired = dynamic_cast<component_type *>(Component))
            {
                return desired;
            }
        }
        return nullptr;
    }

    virtual ~actor() = default;
};

class component
{

public:
    int Id = NextComponentId++;
    int ActorId = -1;
    bool IsPendingRemoval = false;

    virtual ~component() = default;
    virtual void Update(float DeltaTime) = 0;
    virtual void Display() = 0;
    virtual bool OnReceive(const mail &m) = 0;
    void RemoveFromActor();
};
