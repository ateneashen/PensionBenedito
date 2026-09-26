# External Assets and Resources

## Overview

This document catalogs external assets, libraries, and resources that can be used in **Pension Benedito** and the **NarrativeActionKit** framework. These resources are collected from various sources and are either free or have permissive licenses.

---

## Animation Libraries

### Quaternius - Universal Animation Library 2

- **URL**: https://quaternius.com/packs/universalanimationlibrary2.html
- **Description**: Comprehensive animation library with hundreds of animations for humanoid characters
- **License**: Free for commercial and non-commercial use
- **Format**: FBX, compatible with UE4.27
- **Key Features**:
  - Walk, run, idle animations
  - Combat animations (attacks, dodges, blocks)
  - Interaction animations (pick up, open door, etc.)
  - Death and hit reactions
  - Emotes and gestures
- **How to Use**:
  1. Download the pack from the website
  2. Import FBX files into UE4.27
  3. Retarget to our character skeleton if needed
  4. Create Animation Blueprints
- **Application in Project**:
  - Player character movement
  - NPC idle and dialogue animations
  - Combat system animations
  - Interaction animations (reading documents, opening doors)

### Other Quaternius Packs

- **URL**: https://quaternius.com/
- **Available Packs**:
  - Ultimate Animated Characters
  - Low Poly Animated Characters
  - Fantasy RTS Characters
  - And many more...
- **License**: All free for commercial use

---

## 3D Models

### Kenney Assets

- **URL**: https://kenney.nl/
- **Description**: Thousands of free game assets (2D, 3D, audio)
- **License**: CC0 (public domain)
- **Notable Packs**:
  - 3D Furniture
  - Building Kit
  - Nature Kit
  - UI Pack

### Sketchfab (Free Models)

- **URL**: https://sketchfab.com/
- **Description**: 3D model marketplace with many free models
- **License**: Varies (check each model)
- **Filter**: Use "Downloadable" and "Free" filters

---

## Audio

### Freesound

- **URL**: https://freesound.org/
- **Description**: Collaborative database of Creative Commons Licensed sounds
- **License**: CC0, CC-BY, CC-BY-NC
- **Application**: Sound effects, ambient audio

### Incompetech (Kevin MacLeod)

- **URL**: https://incompetech.com/
- **Description**: Royalty-free music
- **License**: CC-BY (attribution required)
- **Application**: Background music, menu music

---

## Textures and Materials

### Poly Haven

- **URL**: https://polyhaven.com/
- **Description**: High-quality PBR textures, models, and HDRIs
- **License**: CC0 (public domain)
- **Application**: Environment textures, materials

### AmbientCG

- **URL**: https://ambientcg.com/
- **Description**: Free PBR materials and textures
- **License**: CC0
- **Application**: Architectural materials, period-appropriate textures

---

## Fonts

### Google Fonts

- **URL**: https://fonts.google.com/
- **Description**: Free fonts for commercial use
- **License**: Apache License 2.0, SIL Open Font License
- **Application**: UI text, documents in-game, period-appropriate typography

### Font Squirrel

- **URL**: https://www.fontsquirrel.com/
- **Description**: Free fonts for commercial use
- **License**: Varies (check each font)
- **Application**: Decorative fonts, period-appropriate typography

---

## Historical References

### Biblioteca Nacional de España (BNE)

- **URL**: https://www.bne.es/
- **Description**: Spanish National Library with extensive digital archives
- **Application**: Historical photographs, documents, maps

### Archivo Regional de la Comunidad de Madrid

- **URL**: https://www.archivomadrid.es/
- **Description**: Regional archive of Madrid
- **Application**: Historical photographs, municipal records

---

## Integration Guidelines

### Before Using External Assets

1. **Check License**: Always verify the license allows commercial use
2. **Document Source**: Record where the asset came from
3. **Attribute if Required**: Add attribution to credits
4. **Test Compatibility**: Ensure it works with UE4.27
5. **Optimize for Performance**: Reduce poly count, compress textures

### Naming Convention

When importing external assets:
`
[Source]_[Type]_[Description]
`

Examples:
- Quaternius_Anim_Walk_Forward
- Kenney_Mesh_Chair_Wooden
- AmbientCG_Tex_Wood_Floor

### Folder Structure

`
Content/
├── External/
│   ├── Quaternius/
│   │   ├── Animations/
│   │   └── Characters/
│   ├── Kenney/
│   │   ├── Furniture/
│   │   └── Props/
│   └── AmbientCG/
│       ├── Textures/
│       └── Materials/
`

---

## Checklist for New Assets

- [ ] License verified (commercial use allowed)
- [ ] Source documented
- [ ] Attribution added (if required)
- [ ] Tested in UE4.27
- [ ] Optimized for performance
- [ ] Named correctly
- [ ] Placed in correct folder
- [ ] Added to this document

---

*Last updated: 2026-09-26*
