# Test 02: Documentation Generation (Low Risk)

## Objetivo
Evaluar si el modelo puede generar documentación de calidad para código UE4.27 existente.

## Prompt para el modelo local

```
Generate comprehensive Doxygen-style documentation for the following Unreal Engine 4.27.2 C++ ActorComponent. 

Include:
1. File-level documentation (brief description, copyright notice)
2. Class documentation with @brief, @details, @note
3. All UPROPERTY documentation with @brief, @see, @note where relevant
4. All UFUNCTION documentation with @brief, @param, @return, @note
5. Delegate documentation
6. Usage examples in @code blocks

The code:

```cpp
// HealthComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "1.0"))
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void ResetHealth();

protected:
    virtual void BeginPlay() override;
};
```

Generate the documentation in the header file format.
```

## Criterios de evaluación

### Correctitud (0-10)
- [ ] Usa formato Doxygen correcto (`/** */`, `@brief`, `@param`, etc.)
- [ ] Documenta TODOS los miembros públicos
- [ ] Los parámetros coinciden con los del código
- [ ] Los tipos son correctos

### Utilidad (0-10)
- [ ] Las descripciones son claras y concisas
- [ ] Incluye ejemplos de uso
- [ ] Menciona consideraciones de uso (ej: "call on server only")
- [ ] Referencia otros miembros relacionados (@see)

### Formato (0-10)
- [ ] Consistencia en el estilo
- [ ] Sin errores tipográficos
- [ ] Formato de código correcto en ejemplos
- [ ] Alineación y sangría apropiada

## Resultado esperado

El modelo debería generar documentación completa y profesional, fácil de copiar y pegar directamente al código.