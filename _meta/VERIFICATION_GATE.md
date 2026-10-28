# Felt Sovereign Verification Gate (v1.0)

**Project**: Felt Sovereign Writer (`[FELT]`)  
**Objective**: Deterministic automated and LLM-audited quality gate guaranteeing 100% Object Calisthenics conformance, compiler-grade grammar correctness, and clutter-free William Zinsser test prose before any story is marked **Done**.

---

## 1. Object Calisthenics Invariants (The 9 Rules)
- [ ] **Rule 1 (Single Level of Indentation)**: Max 1 level of indentation per method. Use early returns and guard clauses.
- [ ] **Rule 2 (No `else` Keyword)**: Zero occurrences of `else` across all C++ headers and implementations.
- [ ] **Rule 3 (Wrap Primitives)**: Colors, measures, and statistics wrapped in tiny domain value objects (`ThemePalette`, `Measure`, `DocumentStats`).
- [ ] **Rule 4 (First-Class Collections)**: Theme registries and grammar rule sets held in dedicated collection objects.
- [ ] **Rule 5 (One Dot Per Line)**: Strict Law of Demeter. No chaining through internal object graphs.
- [ ] **Rule 6 (No Abbreviations)**: Identifiers written out in full domain clarity (`highlighter`, not `hl`; `characterCount`, not `charCnt`).
- [ ] **Rule 7 (Small Entities)**: Every `.h` and `.cpp` file strictly $\le 200$ lines.
- [ ] **Rule 8 (Max 2 Instance Variables)**: Non-coordinator domain classes have at most 2 state variables.
- [ ] **Rule 9 (No Getters/Setters on Complex State)**: Tell, don't ask; push behavior into the value objects.

---

## 2. William Zinsser Test Suite Standard
- [ ] **Prose Clarity**: Tests read as muscular human declarations of system truth.
- [ ] **Zero Bureaucratic Fillers**: Forbidden words: `should`, `properly`, `correctly`, `successfully`, `in order to`, `works as expected`.
- [ ] **Active Present-Tense Verbs**: Every test title starts with an active verb (`highlights`, `illuminates`, `rejects`, `preserves`, `continues`, `clears`, `indents`, `enforces`, `dims`, `verifies`, `traps`).

---

## 3. Visual Craft & Physical Sheen Gate
- [ ] **Ambient Vignette**: Canvas renders a radial gradient (`#0D0F14` center softly fading to `#060709` edges), eliminating flat `<textarea>` appearance.
- [ ] **Vertical Rhythm**: $1.6\times$ (160%) proportional line height enforced across all blocks.
- [ ] **Optical Palette**: Alabaster / Bone White (`#ECEBE8`) body text and Slate Smoke (`#3E4249`) dimming under Focus Mode.
- [ ] **Frameless Chrome**: Native Cocoa `NSWindowStyleMaskFullSizeContentView` on macOS with inset traffic lights at `(18, 18)`.
- [ ] **Transparent Icon**: 100% transparent squircle icon in macOS Dock (`alpha = 0` outside rounded perimeter).

---

## 4. Automated Conformance Check Command
Before completing any story, execute:
```bash
task gate
# Which runs: python3 scripts/verify_conformance.py && ctest --test-dir build --output-on-failure
```
All checks must pass with exit code `0`.
