# Instrucciones para Configurar GitHub

## Tu Situacion Actual

Tu proyecto ya tiene toda la infraestructura de GitHub configurada. Solo necesitas ejecutar **un comando** para activarlo.

## Paso 1: Ejecutar el Script de Configuracion

Abre **PowerShell** como administrador y ejecuta:

```powershell
cd "C:\Users\Admin\Documents\DSH Projects\UnrealEngineProjects"
.\Scripts\setup.ps1 -UserName "ateneashen" -UserEmail "atenea_nushen@outlook.com"
```

### ¿Que hara este script?

1. **Configurara tu identidad** en Git (nombre y email)
2. **Inicializara el repositorio** Git en tu carpeta del proyecto
3. **Configurara Git LFS** para manejar archivos grandes (.uasset, .umap)
4. **Creara el repositorio** en GitHub (abrira tu navegador para iniciar sesion)
5. **Hara el primer commit** con todo tu codigo
6. **Subira todo a GitHub**

### ¿Que necesitas hacer?

1. **Ejecutar el comando** de arriba
2. **Iniciar sesion en GitHub** cuando se abra el navegador
3. **Confirmar** algunas preguntas simples
4. **¡Esperar** a que termine (1-2 minutos)

## Paso 2: Verificar que Funciono

Despues de ejecutar el script, verifica:

```powershell
.\Scripts\verify.ps1
```

Deberias ver:
- [OK] Git instalado
- [OK] GitHub CLI instalado
- [OK] Git identity configurado
- [OK] Repositorio inicializado
- [OK] Git LFS instalado
- [OK] .gitignore existe
- [OK] .gitattributes existe
- [OK] Scripts encontrados

## Paso 3: Empezar a Usar

### Menu Interactivo (Recomendado para Principiantes)

```powershell
.\Scripts\quick-start.ps1`
```

Este menu te da opciones para:
- Ver estado de Git
- Hacer commits
- Subir a GitHub
- Crear backups
- Y mas...

### Comandos Basicos

| Accion | Comando |
|--------|---------|
| Ver estado | `git status` |
| Guardar cambios | `.\Scripts\commit.ps1 -Type "feat" -Message "descripcion"` |
| Subir a GitHub | `.\Scripts\push.ps1` |
| Crear backup | `.\Scripts\backup.ps1 -Version "v1.0.0"` |
| Ver ayuda | `.\Scripts\help.ps1` |

## Flujo de Trabajo Diario

### Al Empezar el Dia
```powershell
git pull
```

### Durante el Dia
```powershell
# Hacer cambios en el codigo...
# Cuando quieras guardar:
.\Scripts\commit.ps1 -Type "feat" -Message "Añadir nuevo sistema"
```

### Al Terminar el Dia
```powershell
.\Scripts\push.ps1
```

### Para Crear una Version
```powershell
.\Scripts\backup.ps1 -Version "v1.0.0" -Message "Primera version funcional" -Push
```

## Documentacion Completa

| Documento | Para Que Sirve |
|-----------|----------------|
| **INSTRUCCIONES.md** | Este documento (empezar aqui) |
| **GITHUB_QUICKSTART.md** | Guia rapida en 3 pasos |
| **GITHUB_SETUP.md** | Guia completa con todo el detalle |
| **GITHUB_READY.md** | Estado actual del proyecto |
| **README.md** | Documentacion del proyecto |

## Solucion de Problemas Comunes

### "El script tiene errores de sintaxis"
Asegurate de usar comillas consistentes:
```powershell
# Correcto
.\Scripts\setup.ps1 -UserName "ateneashen" -UserEmail "atenea_nushen@outlook.com"

# Tambien correcto
.\Scripts\setup.ps1 -UserName 'ateneashen' -UserEmail 'atenea_nushen@outlook.com'
```

### "No puedo hacer push"
```powershell
git pull --rebase
git push
```

### "Hice un commit incorrecto"
```powershell
# Si NO lo he subido:
git reset --soft HEAD~1

# Si SI lo he subido:
git revert HEAD
git push
```

### "Mis archivos .uasset son muy grandes"
Git LFS ya esta configurado. Si hay problemas:
```powershell
git lfs track "*.uasset"
git lfs track "*.umap"
git add .gitattributes
git commit -m "chore: Actualizar Git LFS"
```

## Preguntas Frecuentes

### ¿Que es Git?
Git es un sistema de control de versiones que guarda el historial de tus cambios. Es como "Google Drive" para codigo, pero mas potente.

### ¿Que es GitHub?
GitHub es una plataforma en la nube donde guardas tus repositorios Git. Es como un "backup" de tu codigo en internet.

### ¿Que es Git LFS?
Git LFS (Large File Storage) es una extension de Git para manejar archivos grandes como los de Unreal Engine (.uasset, .umap).

### ¿Necesito saber programar para usar Git?
No. Los scripts que hemos creado automatizan todo. Solo necesitas ejecutar comandos simples.

### ¿Puedo perder mi codigo?
No. Git guarda todo el historial. Puedes volver a cualquier version anterior en cualquier momento.

## Enlaces Utiles

- [GitHub Guides](https://guides.github.com/) - Guias oficiales
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf) - Referencia rapida
- [Unreal Engine Git](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/RevisionControl/) - Documentacion oficial

## Checklist

- [ ] Script de verificacion ejecutado
- [ ] Script de configuracion ejecutado
- [ ] Sesion de GitHub iniciada
- [ ] Repositorio creado en GitHub
- [ ] Primer commit hecho
- [ ] Primer push exitoso
- [ ] Menu rapido probado

---

*Ultima actualizacion: 2026-09-26*