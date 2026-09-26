# 🎮 Pension Benedito — Horror Mystery in 1930s Spain

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-4.27-black?style=flat&logo=unrealengine)](https://www.unrealengine.com/)
[![GitHub](https://img.shields.io/badge/GitHub-Repository-blue?style=flat&logo=github)](https://github.com)
[![License](https://img.shields.io/badge/License-Proprietary-red?style=flat)](LICENSE)

> *Two pensions. Two cities. One terrifying connection.*

**Pension Benedito** is a horror mystery game set in the tumultuous years before the Spanish Civil War (1930-1936). Players explore two seemingly unconnected pensions in Madrid and Barcelona, uncovering a dark supernatural link that defies explanation.

---

## 🎭 About the Game

### Setting
- **Time Period**: 1930-1936 (Spanish Second Republic)
- **Locations**: Madrid & Barcelona, Spain
- **Atmosphere**: Pre-war tension, political unrest, supernatural horror

### Gameplay
- **Exploration**: Investigate atmospheric pensions and their secrets
- **Dialogue**: Branching conversations with NPCs
- **Puzzles**: Environmental and logic puzzles
- **Combat**: Action sequences (Bloodborne/DMC inspired)
- **Narrative**: Deep mystery with multiple endings

### Inspirations
- **Bloodborne**: Atmosphere, sanity system, challenging combat
- **DMC (Ninja Theory)**: Stylish action, cinematic storytelling
- **Classic Adventure Games**: Puzzle-solving, exploration, narrative depth

---

## 🏗️ Project Structure

This repository contains two main components:

### 1. NarrativeActionKit (Reusable Framework)
A generic framework for narrative action games, designed for reusability.

```
NarrativeActionKit/
├── Core/                    # Character, Components, Systems
├── Interaction/             # Object interaction system
├── Dialogue/                # Branching dialogue system
├── Inventory/               # Item management
├── Narrative/               # Quest and flag system
└── Terror/                  # Sanity and horror mechanics
```

**Features:**
- ✅ Project-agnostic design
- ✅ Component-based architecture
- ✅ GAS (Gameplay Ability System) integration
- ✅ Data-driven configuration
- ✅ UE4.27 compliant

### 2. Pension Benedito (Game Project)
The specific game built using NarrativeActionKit.

```
PensionBenedito/
├── Source/                  # Game-specific C++ code
├── Content/                 # Unreal assets (Blueprints, Maps, etc.)
├── Research/                # Historical research and references
└── Documentation/           # Game design documents
```

---

## 🚀 Getting Started

### Prerequisites

- **Unreal Engine 4.27.2** — [Download](https://www.unrealengine.com/download)
- **Visual Studio 2019/2022** — With C++ workload
- **Git** — [Download](https://git-scm.com/)
- **GitHub CLI** (optional) — [Download](https://cli.github.com/)

### Quick Start

1. **Clone the repository**
   ```powershell
   git clone https://github.com/YOUR_USERNAME/PensionBenedito.git
   cd PensionBenedito
   ```

2. **Run setup script**
   ```powershell
   .\Scripts\setup.ps1 -UserName "Your Name" -UserEmail "your@email.com"
   ```

3. **Open in Unreal Engine**
   - Double-click `PensionBenedito.uproject`
   - Wait for compilation
   - Start exploring!

For detailed setup instructions, see [GITHUB_SETUP.md](GITHUB_SETUP.md).

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [GITHUB_SETUP.md](GITHUB_SETUP.md) | Complete GitHub guide for beginners |
| [NarrativeActionKit/README.md](NarrativeActionKit/README.md) | Framework documentation |
| [NarrativeActionKit/Documentation/ARCHITECTURE.md](NarrativeActionKit/Documentation/ARCHITECTURE.md) | Architecture guide |
| [PensionBenedito/README.md](PensionBenedito/README.md) | Game-specific documentation |
| [PensionBenedito/Research/README.md](PensionBenedito/Research/README.md) | Research methodology |

---

## 🛠️ Development

### Daily Workflow

```powershell
# Start of day
git pull

# Make changes...
# ...

# End of day
.\Scripts\commit.ps1 -Type "feat" -Message "Add new feature"
.\Scripts\push.ps1
```

### Creating Backups

```powershell
# Create versioned backup
.\Scripts\backup.ps1 -Version "v1.0.0" -Message "First playable version" -Push
```

### Branch Strategy

```
main (stable)
├── develop (active development)
│   ├── feature/new-system
│   ├── feature/dialogue-improvements
│   └── bugfix/crash-fix
└── release/v1.0
```

---

## 🎨 Art Direction

### Visual Style
- **Period**: 1930s Spain (Art Deco, early modernism)
- **Lighting**: Gas lamps, early electricity, candlelight
- **Color Palette**: Muted, desaturated with occasional warm accents
- **Architecture**: Madrid neoclassical, Barcelona modernist

### Key Visual Elements
- Pension interiors with period-accurate furniture
- Street scenes with vintage vehicles and signage
- Character costumes reflecting social class
- Supernatural visual effects for horror sequences

---

## 🔬 Research

Extensive historical research ensures period authenticity:

- **Historical Context**: Political climate, daily life, social classes
- **Visual References**: Architecture, costumes, props, documents
- **Terror Elements**: Period fears, superstitions, unsolved mysteries
- **Local LLM Integration**: AI-assisted research for efficiency

See [Research Documentation](PensionBenedito/Research/README.md) for details.

---

## 🤝 Contributing

This is currently a solo project, but contributions are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'feat: Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## 📋 Roadmap

### Phase 1: Foundation ✅
- [x] NarrativeActionKit framework
- [x] Core systems (Character, Components, Dialogue)
- [x] Historical research
- [x] Project structure

### Phase 2: Core Gameplay (In Progress)
- [ ] First playable level
- [ ] Basic interaction system
- [ ] Dialogue system integration
- [ ] Inventory implementation

### Phase 3: Content Creation
- [ ] Madrid pension level
- [ ] Barcelona pension level
- [ ] NPC characters
- [ ] Puzzle design

### Phase 4: Polish
- [ ] Visual effects
- [ ] Audio design
- [ ] Performance optimization
- [ ] Bug fixing

### Phase 5: Release
- [ ] Playtesting
- [ ] Marketing materials
- [ ] Distribution
- [ ] Post-launch support

---

## 📄 License

This project is proprietary. All rights reserved.

For licensing inquiries, contact: [your-email@example.com]

---

## 🙏 Acknowledgments

- **Epic Games** — For Unreal Engine and GAS
- **Historical Archives** — For period references
- **Community** — For feedback and support

---

## 📞 Contact

- **GitHub**: [github.com/YOUR_USERNAME](https://github.com/YOUR_USERNAME)
- **Email**: your-email@example.com
- **Twitter**: [@YourHandle](https://twitter.com/YourHandle)

---

<p align="center">
  <i>Made with ❤️ and Unreal Engine 4.27</i>
</p>