# Guía de GitHub para Pension Benedito y NarrativeActionKit

## 📋 Índice

1. [Conceptos Básicos de GitHub](#conceptos-básicos)
2. [Estructura del Repositorio](#estructura-del-repositorio)
3. [Configuración Inicial](#configuración-inicial)
4. [Flujo de Trabajo Diario](#flujo-de-trabajo-diario)
5. [Mejores Prácticas](#mejores-prácticas)
6. [Solución de Problemas](#solución-de-problemas)
7. [Scripts Automatizados](#scripts-automatizados)

---

## Conceptos Básicos de GitHub

### ¿Qué es GitHub?

GitHub es una plataforma para almacenar, versionar y colaborar en proyectos de código. Piensa en él como:

- **Google Drive para código** — Guarda tus archivos en la nube
- **Historial de cambios** — Puedes ver y revertir cualquier cambio
- **Trabajo en equipo** — Múltiples personas pueden trabajar simultáneamente
- **Backup automático** — Tu código siempre está seguro

### Conceptos Clave

| Término | Analogía | Descripción |
|---------|----------|-------------|
| **Repository (Repo)** | Carpeta del proyecto | Donde se guarda todo tu código |
| **Commit** | Foto del proyecto | Un snapshot de tus archivos en un momento dado |
| **Branch** | Versión paralela | Una copia del código para experimentar sin riesgo |
| **Push** | Subir a la nube | Envía tus cambios locales a GitHub |
| **Pull** | Bajar de la nube | Descarga cambios de GitHub a tu computadora |
| **Merge** | Combinar versiones | Une cambios de diferentes branches |

### Flujo Visual

```
Tu Computadora (Local)          GitHub (Nube)
┌─────────────────┐            ┌─────────────────┐
│  Archivos       │   push     │  Archivos       │
│  modificados    │ ─────────→ │  actualizados   │
│                 │            │                 │
│  Archivos       │ ←───────── │  Archivos       │
│  actualizados   │   pull     │  modificados    │
└─────────────────┘            └─────────────────┘
```

---

## Estructura del Repositorio

### Repositorio Principal: `PensionBenedito`

```
PensionBenedito/
├── .github/                    # Configuración de GitHub
│   ├── workflows/             # Acciones automáticas
│   │   └── build.yml         # Build automático
│   └── ISSUE_TEMPLATE/       # Plantillas para issues
│       ├── bug_report.md
│       └── feature_request.md
│
├── .gitignore                 # Archivos a ignorar
├── .gitattributes             # Configuración de archivos
├── README.md                  # Documentación principal
├── LICENSE                    # Licencia del proyecto
├── CHANGELOG.md               # Historial de cambios
│
├── Source/                    # Código fuente
│   ├── NarrativeActionKit/   # Framework reutilizable (submodule)
│   └── PensionBenedito/      # Código del juego
│
├── Content/                   # Assets de Unreal (archivos grandes)
│   ├── Blueprints/
│   ├── Maps/
│   ├── Textures/
│   └── ...
│
├── Docs/                      # Documentación adicional
│   ├── Setup.md
│   ├── Architecture.md
│   └── Contributing.md
│
├── Scripts/                   # Scripts de automatización
│   ├── setup.ps1             # Configuración inicial
│   ├── commit.ps1            # Commit rápido
│   ├── push.ps1              # Push a GitHub
│   └── backup.ps1            # Backup manual
│
└── Research/                  # Documentación de investigación
    ├── Historical-Context/
    └── Visual-References/
```

### ¿Por Qué Esta Estructura?

1. **Organización clara** — Cada cosa en su lugar
2. **Separación de concerns** — Código, assets, documentación
3. **Automatización** — Scripts para tareas repetitivas
4. **Colaboración** — Plantillas para issues y pull requests

---

## Configuración Inicial

### Paso 1: Configurar Git (Una sola vez)

Abre PowerShell y ejecuta:

```powershell
# Configurar tu identidad (cambia estos valores)
git config --global user.name "Tu Nombre"
git config --global user.email "tu.email@ejemplo.com"

# Configurar comportamiento por defecto
git config --global pull.rebase false
git config --global core.autocrlf true
```

### Paso 2: Autenticarse en GitHub

```powershell
# Iniciar sesión en GitHub (abre el navegador)
gh auth login

# Sigue las instrucciones en pantalla
# Selecciona: GitHub.com → HTTPS → Y → Login with browser
```

### Paso 3: Crear el Repositorio

```powershell
# Navegar a la carpeta del proyecto
cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"

# Inicializar repositorio Git
git init

# Crear repositorio en GitHub
gh repo create PensionBenedito --public --description "Horror mystery game set in 1930s Spain" --source=. --push
```

### Paso 4: Configurar Archivos Especiales

Los archivos `.gitignore` y `.gitattributes` ya están creados en el proyecto. Estos le dicen a Git qué archivos ignorar y cómo manejar archivos grandes.

---

## Flujo de Trabajo Diario

### Inicio del Día

```powershell
# 1. Navegar al proyecto
cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"

# 2. Bajar últimos cambios (si trabajaste en otro lugar)
git pull
```

### Durante el Día

```powershell
# 3. Hacer cambios en el código
# ... editar archivos ...

# 4. Ver qué archivos cambiaron
git status

# 5. Agregar cambios al "staging"
git add .  # Agrega todos los cambios
# O seleccionar archivos específicos:
# git add Source/PensionBenedito/Private/Core/MiArchivo.cpp

# 6. Crear un commit (guardar snapshot)
git commit -m "Añadir sistema de diálogos básico"

# 7. Subir a GitHub
git push
```

### Fin del Día

```powershell
# 8. Asegurarse de que todo está subido
git status
git push

# 9. (Opcional) Crear un backup etiquetado
git tag -a v0.1.0 -m "Primera versión funcional"
git push --tags
```

---

## Mejores Prácticas

### Commits Significativos

**❌ Malo:**
```powershell
git commit -m "cambios"
git commit -m "arreglando cosas"
git commit -m "asdfasdf"
```

**✅ Bueno:**
```powershell
git commit -m "Añadir componente de interacción con detección por raycast"
git commit -m "Corregir bug en sistema de diálogos al saltar nodos"
git commit -m "Documentar sistema de inventario con ejemplos"
```

### Convención de Commits

Usa este formato:

```
[Tipo]: Descripción corta

Tipos:
- feat:     Nueva característica
- fix:      Corrección de bug
- docs:     Documentación
- style:    Formato, espacios, etc.
- refactor: Reestructurar código
- test:     Añadir tests
- chore:    Mantenimiento
```

Ejemplos:
```powershell
git commit -m "feat: Añadir sistema de sanidad con 6 niveles"
git commit -m "fix: Corregir crash al interactuar con NPC muerto"
git commit -m "docs: Documentar API del sistema de diálogos"
```

### Ramas (Branches) para Trabajo en Equipo

```
main (rama principal - siempre estable)
├── develop (rama de desarrollo)
│   ├── feature/sistema-combate
│   ├── feature/dialogos-avanzados
│   └── bugfix/crash-inventario
└── release/v1.0 (rama de lanzamiento)
```

**Crear una nueva rama:**
```powershell
# Crear y cambiar a nueva rama
git checkout -b feature/nuevo-sistema

# Trabajar normalmente...
git add .
git commit -m "feat: Implementar nuevo sistema"

# Subir la rama a GitHub
git push -u origin feature/nuevo-sistema

# Cuando termine, fusionar con develop
git checkout develop
git merge feature/nuevo-sistema
git push
```

---

## Solución de Problemas

### "No puedo hacer push"

**Problema:** Rechaza el push porque hay cambios remotos.

**Solución:**
```powershell
# Bajar cambios remotos
git pull --rebase

# Si hay conflictos, resolverlos y continuar
git add .
git rebase --continue

# Intentar push de nuevo
git push
```

### "Hice un commit incorrecto"

**Problema:** Cometí un error en el último commit.

**Solución (si NO lo he subido):**
```powershell
# Deshacer último commit (mantiene cambios)
git reset --soft HEAD~1

# Corregir y volver a commitear
git add .
git commit -m "feat: Corregir implementación del sistema X"
```

**Solución (SÍ lo subí):**
```powershell
# Crear un nuevo commit que deshaga el error
git revert HEAD
git push
```

### "Mis archivos .uasset son muy grandes"

**Problema:** Git no puede manejar archivos grandes.

**Solución:**
```powershell
# Instalar Git LFS (Large File Storage)
git lfs install

# Configurar para archivos de Unreal
git lfs track "*.uasset"
git lfs track "*.umap"

# Agregar configuración
git add .gitattributes
git commit -m "chore: Configurar Git LFS para assets de Unreal"
```

### "No veo mis cambios"

**Problema:** `git status` no muestra cambios.

**Solución:**
```powershell
# Ver todos los cambios (incluyendo no rastreados)
git status -u

# Si el archivo está en .gitignore, verificar
cat .gitignore
```

---

## Scripts Automatizados

### Script de Configuración Inicial (`setup.ps1`)

Este script configura todo automáticamente la primera vez.

### Script de Commit Rápido (`commit.ps1`)

```powershell
# Uso: .\Scripts\commit.ps1 "Descripción del cambio"
```

### Script de Push (`push.ps1`)

```powershell
# Uso: .\Scripts\push.ps1
```

### Script de Backup (`backup.ps1`)

```powershell
# Uso: .\Scripts\backup.ps1 "v1.0.0"
```

---

## Comandos de Referencia Rápida

| Acción | Comando |
|--------|---------|
| Ver estado | `git status` |
| Ver historial | `git log --oneline` |
| Agregar todo | `git add .` |
| Commit | `git commit -m "mensaje"` |
| Push | `git push` |
| Pull | `git pull` |
| Crear rama | `git checkout -b nombre-rama` |
| Cambiar rama | `git checkout nombre-rama` |
| Ver ramas | `git branch` |
| Fusionar rama | `git merge nombre-rama` |
| Ver diferencias | `git diff` |

---

## Recursos Adicionales

- [GitHub Guides](https://guides.github.com/) — Guías oficiales
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf) — Referencia rápida
- [Unreal Engine Git](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/RevisionControl/) — Documentación oficial

---

## Checklist del Principiante

- [ ] Git instalado
- [ ] GitHub CLI instalado
- [ ] Identidad configurada (nombre y email)
- [ ] Sesión de GitHub iniciada
- [ ] Repositorio creado
- [ ] Primer commit hecho
- [ ] Primer push exitoso
- [ ] Scripts de automatización probados

---

*Última actualización: 2026-09-26*