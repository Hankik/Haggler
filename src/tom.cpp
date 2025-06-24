#include "raylib.h"
#include "tom.h"

// void RemoveTagFromObj(tag* Tag)
// {
//     Actors[ActorId]->ComponentIdsPendingRemoval.insert(Id);
//     IsPendingRemoval = true;
// }

// void actor::Update(float DeltaTime)
// {

//     if (ComponentIdsPendingRemoval.size() > 0)
//     {
//         for (int Index = ComponentIds.size() - 1; Index >= 0; --Index)
//         {
//             int ComponentId = ComponentIds[Index];
//             if (ComponentIdsPendingRemoval.find(ComponentId) != ComponentIdsPendingRemoval.end())
//             {
//                 component *Component = Components[ComponentId];
//                 Components[ComponentId] = nullptr;
//                 ComponentIds.erase(ComponentIds.begin() + Index);
//                 delete Component;
//             }
//         }

//         ComponentIdsPendingRemoval.clear();
//     }

//     if (ChildrenIdsPendingRemoval.size() > 0)
//     {
//         for (int Index = ChildrenIds.size() - 1; Index >= 0; --Index)
//         {
//             int ChildId = ChildrenIds[Index];
//             if (ChildrenIdsPendingRemoval.find(ChildId) != ChildrenIdsPendingRemoval.end())
//             {
//                 actor *Child = Actors[ChildId];
//                 ChildrenIds.erase(ChildrenIds.begin() + Index);
//                 Child->Remove();
//                 delete Child; // same thing down here
//             }
//         }
//         ChildrenIdsPendingRemoval.clear();
//     }

//     for (int ComponentId : ComponentIds)
//     {
//         Components[ComponentId]->Update(DeltaTime);
//     }

//     for (int ChildId : ChildrenIds)
//     {
//         Actors[ChildId]->Update(DeltaTime);
//     }
// }

// void actor::Display()
// {
// }

// // must be called before delete
// void actor::Remove()
// {

//     for (int ComponentId : ComponentIds)
//     {

//         Components[ComponentId]->RemoveFromActor();
//     }

//     for (int ChildId : ChildrenIds)
//     {

//         ChildrenIdsPendingRemoval.insert(ChildId);
//         Actors[ChildId]->Remove();
//     }
//     Update(0);
// }

// void actor::AddActor(actor *Actor)
// {

//     Actors[Actor->Id] = Actor;
//     ChildrenIds.push_back(Actor->Id);
//     Actor->ParentId = Id;
// }

// void actor::AddComponent(component *Component)
// {

//     Component->ActorId = Id;
//     Components[Component->Id] = Component;
//     ComponentIds.push_back(Component->Id);
// }

// bool actor::SendUp(const mail &Mail)
// {

//     for (int ComponentId : ComponentIds)
//     {

//         component *Component = Components[ComponentId];
//         bool Consumed = Component->OnReceive(Mail);
//         if (Consumed)
//         {
//             return true;
//         }
//     }

//     for (const int &ChildId : ChildrenIds)
//     {
//         actor *Child = Actors[ChildId];
//         bool Consumed = Child->SendUp(Mail);
//         if (Consumed)
//         {
//             return true;
//         }
//     }
//     return false;
// }

// bool actor::SendDown(const mail &Mail)
// {

//     for (int ComponentId : ComponentIds)
//     {

//         component *Component = Components[ComponentId];
//         bool Consumed = Component->OnReceive(Mail);
//         if (Consumed)
//         {
//             return true;
//         }
//     }

//     for (int ChildId : ChildrenIds)
//     {
//         actor *Child = Actors[ChildId];
//         bool Consumed = Child->SendDown(Mail);
//         if (Consumed)
//         {
//             return true;
//         }
//     }
//     return false;
// }

// bool actor::SendDirect(const mail &Mail)
// {

//     for (int ComponentId : ComponentIds)
//     {

//         component *Component = Components[ComponentId];
//         bool Consumed = Component->OnReceive(Mail);
//         if (Consumed)
//         {
//             return true;
//         }
//     }
//     return false;
// }
