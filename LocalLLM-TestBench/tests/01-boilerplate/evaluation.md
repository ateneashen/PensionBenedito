# Evaluación — Test 01: Health Component

## Código generado (resumen)

El modelo generó un componente de salud con delegates y funciones básicas. Sin embargo, tiene **varios errores críticos** que impiden su uso directo.

## Errores encontrados

### Errores de compilación (CRÍTICOS)

1. **Línea 15**: `#include "HealthComponent.h"` — Incluye el propio header (error circular)
2. **Línea 29**: `DECLARE_DYNAMIC_MULTICAST_DELEGABLE(FOnDeath)` — Typo: debería ser `DELEGATE` (sin parámetros)
3. **Línea 55-56**: `FDelegateWrapper<>` — No existe en UE4. Los delegates dinámicos no necesitan wrappers manuales
4. **Línea 69**: Comentario inválido — `PrimaryActorTick` no existe en componentes (es `PrimaryComponentTick`)
5. **Línea 110**: `LogHealthComponentWarning` — Categoría de log mal definida (debería ser `LogHealthComponent`)
6. **Línea 115**: `FMath::Clamp(HealAmount, 0.0f)` — Falta el segundo parámetro (MaxHealth)
7. **Línea 119**: `FMath::Clamp(CurrentHealth, CurrentHealth, MaxHealth)` — Lógica incorrecta (debería ser `0.0f, MaxHealth`)
8. **Línea 121**: `OnHealthChanged.Broadcast(Health, MaxHealth)` — Variable `Health` no existe (debería ser `CurrentHealth`)

### Errores de convención UE 4.27

1. **Línea 35**: `EditAnywhere, BlueprintReadWrite` para CurrentHealth — Según criterios, debería ser `VisibleAnywhere, BlueprintReadOnly` (el setter debería ser función, no propiedad directa)
2. **Constructor vacío**: No inicializa valores por defecto (MaxHealth = 100, CurrentHealth = MaxHealth, bIsDead = false)
3. **Falta `.generated.h`**: Aunque está incluido, el código no tiene `UCLASS()` ni `UFUNCTION()` macros
4. **No hay `UCLASS()`**: Falta la declaración de clase para que sea un UObject válido

### Errores de diseño

1. **No hay `BeginPlay`**: Debería inicializar CurrentHealth en BeginPlay, no en constructor
2. **No hay validación de daño negativo**: ApplyDamage no verifica DamageAmount > 0
3. **Heal revive al muerto**: La lógica de heal permite revivir, lo cual podría no ser deseable (debería ser configurable)

## Puntuación

| Criterio | Puntuación | Notas |
|----------|------------|-------|
| **Correctitud** | 3/10 | Múltiples errores de compilación, typos, lógica incorrecta |
| **Seguridad** | 4/10 | Usa delegates dinámicos correctamente, pero tiene bugs peligrosos |
| **Eficiencia** | 6/10 | Sin tick, código relativamente limpio a pesar de los errores |
| **Usabilidad** | 2/10 | No compila, requiere correcciones extensas |
| **TOTAL** | **3.75/10** | ❌ No apto para uso directo |

## Correcciones necesarias

### Header (.h) corregido

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
class YOURPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	// Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "1.0"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	// Functions
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

### Implementación (.cpp) corregida

```cpp
// HealthComponent.cpp
#include "HealthComponent.h"

DEFINE_LOG_CATEGORY(LogHealthComponent);

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100.0f;
	CurrentHealth = 100.0f;
	bIsDead = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsDead)
	{
		UE_LOG(LogHealthComponent, Warning, TEXT("ApplyDamage called on dead actor. Ignoring."));
		return;
	}

	if (DamageAmount <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast();
		UE_LOG(LogHealthComponent, Log, TEXT("Actor is dead."));
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (CurrentHealth > 0.0f)
	{
		bIsDead = false;
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
	bIsDead = false;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
```

## Conclusión

El modelo **no generó código usable** en este primer test. Los errores van desde typos simples hasta bugs de lógica y violaciones de convenciones UE4.27. 

**Recomendación**: Para boilerplate simple, el modelo requiere supervisión intensiva. Las tareas de documentación y refactorización menor podrían ser más adecuadas.