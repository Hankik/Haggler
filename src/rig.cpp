
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
        EyeObj,
        MouthObj,
        BodyObj
    };
    TryAddObjs(*RigObj, ArrayToTray(ObjsToAdd));

    

    return RigObj;
}

tag* MakeRigTag() {
    rig_tag * RigTag = (rig_tag*) MakeAlloc<rig_tag>();
    *RigTag = rig_tag{};
    RigTag->Type = RIG;
    return RigTag;
}


// TODO(hank): finish setting up set states and test
void SetEyeState(rig_tag& RigTag, eye_state NewState) {
    flipbook_tag* EyeAnimTag = (flipbook_tag*) (*hmget(TomCtx.ObjMap, RigTag.EyeObjId), FLIPBOOK);
    const char * AnimName = RigTag.EyeAnims[(int)NewState];
    EyeAnimTag->Frames = shget(TomCtx.AnimMap, AnimName);
    RigTag.EyeState = NewState;
}

void RigTagTick(tag& Tag) { 

}

void RigTagDraw(const tag& Tag) {

}



bool OnRigGetMsg(tag& Tag, msg& Msg) {

    return false;
}

