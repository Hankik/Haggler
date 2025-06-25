#include "raylib.h"
#include "tom.h"
#include "globals.h"
#include "tray.h"

// void RemoveTagFromObj(tag* Tag)
// {
//     Actors[ActorId]->ComponentIdsPendingRemoval.insert(Id);
//     IsPendingRemoval = true;
// }

void ObjTick(obj* Obj)
{
}

void ObjDraw(obj* Obj)
{
}

void AddObjs(obj* Obj, const tray<obj*>& NewObjs) {
    int PrevAmt = Obj->Children->Amt;
    Obj->Children = (*Obj->Children) + NewObjs;
    for (int Index = PrevAmt; Index < Obj->Children->Amt; ++Index) {
        (*Obj->Children)[Index]->Parent = Obj;
    }
}

void AddTags(obj* Obj, const tray<tag*>& NewTags) {
    int PrevAmt = Obj->Tags->Amt;
    Obj->Tags = (*Obj->Tags) + NewTags;
    for (int Index = PrevAmt; Index < Obj->Tags->Amt; ++Index) {
        (*Obj->Tags)[Index]->Obj = Obj;
    }
}

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
