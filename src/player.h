#include "actors.h"
#include "iostream"

class player_component : public component
{
public:
    player_component() {}
    void Update(float DeltaTime) {}
    void Display() {}

    bool OnReceive(const mail &Mail)
    {

        // TODO(hank): make the movement not terrible
        if (auto KPM = dynamic_cast<const key_pressed_mail *>(&Mail))
        {
            actor *Player = Actors[ActorId];
            if (KPM->Key == 'w')
                Player->Position.y -= 10;
            if (KPM->Key == 's')
                Player->Position.y += 10;
            if (KPM->Key == 'a')
                Player->Position.x -= 10;
            if (KPM->Key == 'd')
                Player->Position.x += 10;
        }
        return false;
    }
};

class player : public actor
{
public:
    player()
    {
        AddComponent(new player_component());
    }

    void Update(float DeltaTime)
    {
        actor::Update(DeltaTime);
    }

    void Display()
    {
        DrawCircle(Position.x, Position.y, 32, RED);
    }
};
