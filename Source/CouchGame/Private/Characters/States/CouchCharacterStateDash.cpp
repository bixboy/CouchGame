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

    // Initialiser le timer à 0 lors de l'entrée dans l'état de dash
    DashTimer = 0.f;

    // Appliquer une impulsion initiale dans la direction du mouvement
    FVector2D InputMove = Character->GetInputMove();
    if (!InputMove.IsNearlyZero())
    {
        FVector DashDirection = FVector(InputMove.Y, InputMove.X, 0.f).GetSafeNormal();
        Character->LaunchCharacter(DashDirection * DashForce, true, true);
    }
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

    // Remettre le timer à 0 lors de la sortie du dash
    DashTimer = 0.f;
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

    // Augmenter le timer du dash
    DashTimer += DeltaTime;

    // Si la durée du dash est atteinte, passer à l'état Idle ou un autre état
    if (DashTimer >= DashDuration)
    {
        StateMachine->ChangeState(ECouchCharacterStateID::Idle);  // Retourner à Idle après le dash
        return;
    }

    // Appliquer un mouvement continu pendant le dash (si souhaité)
    FVector2D InputMove = Character->GetInputMove();
    if (!InputMove.IsNearlyZero())
    {
        FVector DashDirection = FVector(InputMove.Y, InputMove.X, 0.f).GetSafeNormal();
        Character->AddMovementInput(DashDirection, DashForce * DeltaTime);
    }
}
