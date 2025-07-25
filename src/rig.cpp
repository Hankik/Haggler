
#include "rig.h"
#include "tray.h"
#include "flipbook.h"

obj* MakeRigObj() {
    obj* RigObj = MakeObj();
    RigObj->Children = MakeTray<obj*>(3);

    rig_tag* RigTag = (rig_tag*) MakeRigTag();

    tag* TagsToAdd[] {
        RigTag
    };
    AddTags(*RigObj, ArrayToTray(TagsToAdd));

    obj* EyeObj = MakeFlipbookObj();
    RigTag->EyeObjId = EyeObj->Id;

    obj* MouthObj = MakeFlipbookObj();
    RigTag->MouthObjId = MouthObj->Id;
    
    obj* BodyObj = MakeFlipbookObj();
    RigTag->BodyObjId = BodyObj->Id;

    obj* ObjsToAdd[] {
        BodyObj,
        MouthObj,
        EyeObj,
    };
    TryAddObjs(*RigObj, ArrayToTray(ObjsToAdd));

    

    return RigObj;
}

tag* MakeRigTag() {
    rig_tag * RigTag = (rig_tag*) MakeAlloc<rig_tag>();
    *RigTag = rig_tag{};
    RigTag->TickFn = RigTagTick;
    RigTag->DrawFn = RigTagDraw;
    RigTag->OnGetMsgFn = OnRigGetMsg;
    RigTag->Type = RIG;
    return RigTag;
}


// TODO(hank): finish setting up set states and test
void SetEyeState(rig_tag& RigTag, eye_state NewState) {
    flipbook_tag* EyeAnimTag = (flipbook_tag*) GetObjTag(*hmget(TomCtx.ObjMap, RigTag.EyeObjId), FLIPBOOK);
    const char * AnimName = RigTag.EyeAnims[(int)NewState];
    EyeAnimTag->Frames = shget(TomCtx.AnimMap, AnimName);
    RigTag.EyeState = NewState;
}

void SetMouthState(rig_tag& RigTag, mouth_state NewState) {
    flipbook_tag* MouthAnimTag = (flipbook_tag*) GetObjTag(*hmget(TomCtx.ObjMap, RigTag.MouthObjId), FLIPBOOK);
    const char * AnimName = RigTag.MouthAnims[(int)NewState];
    MouthAnimTag->Frames = shget(TomCtx.AnimMap, AnimName);
    RigTag.MouthState = NewState;
}

void SetBodyState(rig_tag& RigTag, body_state NewState) {
    flipbook_tag* BodyAnimTag = (flipbook_tag*) GetObjTag(*hmget(TomCtx.ObjMap, RigTag.BodyObjId), FLIPBOOK);
    const char * AnimName = RigTag.BodyAnims[(int)NewState];
    BodyAnimTag->Frames = shget(TomCtx.AnimMap, AnimName);
    RigTag.BodyState = NewState;
}

void RigTagTick(tag& Tag) { 

}

void RigTagDraw(const tag& Tag) {

}



bool OnRigGetMsg(tag& Tag, msg& Msg) {

    return false;
}

