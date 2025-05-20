#include "actors.h"

class player : public actor {
public:
    
    void Update(float DeltaTime) {
        actor::Update(DeltaTime);
    }
    
    void Display() {}
};

class test_component : public component {};