
#include "save.h"

void MakeSave(const char * SaveName) {

    save_msg SaveMsg{};

    // make save obj potentially with a save context pointer for received msgs to add saves to
    // set saveobj() func to some function to use or not use when msg is received

    obj& SimObj = *GetObj(*TomCtx.SimTag);
    MsgDown(SimObj, SaveMsg);
}

void LoadSave(const char * SaveName) {

}
