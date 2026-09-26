# 🚀 Guía Rápida de GitHub para Pension Benedito

## ⚡ Inicio en 3 Pasos

### Paso 1: Configurar (Una sola vez)

```powershell
# Abre PowerShell y ejecuta:
cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"
.\Scripts\setup.ps1 -UserName "Tu Nombre" -UserEmail "tu@email.com"
```

Este script:
- ✅ Configura tu identidad en Git
- ✅ Inicializa el repositorio
- ✅ Configura Git LFS para archivos grandes
- ✅ Crea el repositorio en GitHub
- ✅ Hace el primer commit y push

### Paso 2: Trabajar (Cada día)

```powershell
# Al empezar el día:
git pull

# Durante el día, cuando quieras guardar cambios:
.\Scripts\commit.ps1 -Type "feat" -Message "Descripción del cambio"

# Al terminar el día:
.\Scripts\push.ps1
```

### Paso 3: Versionar (Cuando tengas algo listo)

```powershell
# Crear una versión etiquetada:
.\Scripts\backup.ps1 -Version "v1.0.0" -Message "Primera versión funcional" -Push
```

---

## 📋 Scripts Disponibles

| Script | Comando | Descripción |
|--------|---------|-------------|
| **quick-start.ps1** | `.\Scripts\quick-start.ps1` | Menú interactivo con opciones comunes |
| **setup.ps1** | `.\Scripts\setup.ps1 -UserName "..." -UserEmail "..."` | Configuración inicial |
| **commit.ps1** | `.\Scripts\commit.ps1 -Type "..." -Message "..."` | Commit rápido |
| **push.ps1** | `.\Scripts\push.ps1` | Push a GitHub |
| **backup.ps1** | `.\Scripts\backup.ps1 -Version "..."` | Crear backup etiquetado |

---

## 🎯 Tipos de Commit

Cuando hagas un commit, usa estos tipos:

| Tipo | Cuándo usarlo | Ejemplo |
|------|---------------|---------|
| **feat** | Nueva característica | `feat: Añadir sistema de diálogos` |
| **fix** | Corregir un bug | `fix: Corregir crash al interactuar` |
| **docs** | Documentación | `docs: Documentar API del inventario` |
| **style** | Formato, espacios | `style: Corregir indentación` |
| **refactor** | Reestructurar código | `refactor: Reorganizar componentes` |
| **test** | Añadir tests | `test: Añadir tests de diálogos` |
| **chore** | Mantenimiento | `chore: Actualizar .gitignore` |

---

## 📊 Flujo de Trabajo Visual

```
┌─────────────────────────────────────────────────────────────┐
│                    FLUJO DIARIO                              │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ☀️  INICIO DEL DÍA                                         │
│      │                                                      │
│      ▼                                                      │
│  ┌─────────┐                                                │
│  │git pull │ ← Bajar cambios (si trabajaste en otro lugar) │
│  └─────────┘                                                │
│      │                                                      │
│      ▼                                                      │
│  ┌─────────────────┐                                        │
│  │ Editar código   │ ← Trabajar normalmente                │
│  │ Crear assets    │                                        │
│  │ Probar cambios  │                                        │
│  └─────────────────┘                                        │
│      │                                                      │
│      ▼                                                      │
│  ┌──────────────────────────────┐                           │
│  │ .\Scripts\commit.ps1        │ ← Guardar cambios         │
│  │ -Type "feat" -Message "..." │                           │
│  └──────────────────────────────┘                           │
│      │                                                      │
│      ▼                                                      │
│  ┌─────────────────┐                                        │
│  │ .\Scripts\push  │ ← Subir a GitHub                      │
│  └─────────────────┘                                        │
│      │                                                      │
│      ▼                                                      │
│  🌙 FIN DEL DÍA                                             │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔍 Comandos Útiles

### Ver Estado
```powershell
git status                    # ¿Qué archivos cambiaron?
git status --short            # Versión compacta
git diff                      # ¿Qué líneas cambiaron?
git log --oneline             # Historial resumido
```

### Gestionar Cambios
```powershell
git add .                     # Agregar todos los cambios
git add archivo.cpp           # Agregar un archivo específico
git commit -m "mensaje"       # Crear commit
git push                      # Subir a GitHub
git pull                      # Bajar de GitHub
```

### Gestionar Ramas
```powershell
git branch                    # Ver ramas locales
git branch -a                 # Ver todas las ramas
git checkout -b nueva-rama    # Crear y cambiar a nueva rama
git checkout main             # Cambiar a rama principal
git merge nueva-rama          # Fusionar rama
```

### Deshacer Errores
```powershell
# Deshacer último commit (sin perder cambios)
git reset --soft HEAD~1

# Descartar todos los cambios locales
git checkout .

# Deshacer cambios en un archivo específico
git checkout -- archivo.cpp
```

---

## ❓ Solución de Problemas Comunes

### "No puedo hacer push"
```powershell
# Bajar cambios y fusionar
git pull --rebase
# Si hay conflictos, resolverlos y luego:
git push
```

### "Hice un commit incorrecto"
```powershell
# Si NO lo he subido:
git reset --soft HEAD~1

# Si SÍ lo he subido:
git revert HEAD
git push
```

### "Mis archivos .uasset son muy grandes"
```powershell
# Git LFS ya está configurado, pero si hay problemas:
git lfs track "*.uasset"
git lfs track "*.umap"
git add .gitattributes
git commit -m "chore: Actualizar Git LFS tracking"
```

### "No veo mis cambios"
```powershell
# Ver todos los cambios (incluyendo no rastreados)
git status -u

# Si el archivo está en .gitignore:
cat .gitignore
```

---

## 📚 Documentación Completa

Para más detalles, consulta:

| Documento | Contenido |
|-----------|-----------|
| [GITHUB_SETUP.md](GITHUB_SETUP.md) | Guía completa de GitHub |
| [README.md](README.md) | Documentación principal del proyecto |
| [CHANGELOG.md](CHANGELOG.md) | Historial de cambios |
| [NarrativeActionKit/README.md](NarrativeActionKit/README.md) | Documentación del framework |

---

## 🎓 Consejos para Principiantes

### ✅ Hacer
- **Commit frecuente** — Mejor muchos commits pequeños que uno grande
- **Mensajes claros** — Describe QUÉ y POR QUÉ, no CÓMO
- **Pull antes de push** — Siempre baja los cambios más recientes
- **Usar ramas** — Para features grandes, crea una rama

### ❌ Evitar
- **Commits genéricos** — "cambios", "asdf", "wip"
- **Archivos grandes** — Git LFS ya está configurado para .uasset/.umap
- **Force push** — Solo si sabes lo que haces
- **Ignorar conflictios** — Resuélvelos siempre

---

## 🆘 ¿Necesitas Ayuda?

1. **Consulta la documentación** — Los archivos .md tienen toda la info
2. **Usa el menú rápido** — `.\Scripts\quick-start.ps1`
3. **Busca en GitHub** — [GitHub Guides](https://guides.github.com/)
4. **Pregunta** — Crea un issue en el repositorio

---

*Última actualización: 2026-09-26*