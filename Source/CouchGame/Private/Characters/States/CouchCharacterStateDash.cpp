#include "Characters/States/CouchCharacterStateDash.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"
#include "GameFramework/Character.h"

ECouchCharacterStateID UCouchCharacterStateDash::GetStateID()
{
    return ECouchCharacterStateID::Dash;
}

void UCouchCharacterStateDash::StateEnter(ECouchCharacterStateID PreviousStateID)
{
    Super::StateEnter(PreviousStateID);

    GEngine->AddOnScreenDebugMessage(
        -1,
        3.f,
        FColor::Purple,
        TEXT("Enter StateDash")
    );
    
    FVector2D InputMove = Character->GetInputMove();
    if (!InputMove.IsNearlyZero())
    {
        FVector DashDirection = FVector(InputMove.Y, InputMove.X, 0.f).GetSafeNormal();
        Character->LaunchCharacter(DashDirection * DashForce, true, true);
    }
    Character->DashDuration = DashDuration;
    StateMachine->ChangeState(ECouchCharacterStateID::Idle);
}

void UCouchCharacterStateDash::StateExit(ECouchCharacterStateID NextStateID)
{
    Super::StateExit(NextStateID);

    GEngine->AddOnScreenDebugMessage(
        -1,
        3.f,
        FColor::Purple,
        TEXT("Exit StateDash")
    );
}

void UCouchCharacterStateDash::StateTick(float DeltaTime)
{
    Super::StateTick(DeltaTime);

    GEngine->AddOnScreenDebugMessage(
        -1,
        DeltaTime,
        FColor::Purple,
        TEXT("Tick StateDash")
    );
    
}
