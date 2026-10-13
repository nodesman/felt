# Felt

> A featherweight, distraction-free markdown writing instrument.
> The unbloated replacement for iA Writer. A quiet place to type what you feel.

Live Site: [felt-5w3.pages.dev](https://felt-5w3.pages.dev)

---

## Philosophy

Felt is built on three unshakeable principles:

1. **The Obsidian Void:** A matte `#080808` canvas with zero toolbars, sidebars, buttons, or ribbons. Pure plain text with generous breathing margins and optimal ~70-character measure.
2. **True Focus Mode:** The active sentence or paragraph stays bright and crisp (`#F0F0F0`). Everything before and after gently dims into deep slate (`#383838`), letting the world fall away.
3. **Native & Sovereign:** Built in native C++ and Qt 6. Instant launch (<50ms), raw local `.md` file sovereignty, zero subscriptions, zero proprietary lock-in.

---

## The Essential 5 In-Situ Markdown Rules

Felt deliberately rejects complex Markdown tables, math formulas, and HTML conversion. It styles the Essential 5 in-place:

1. **Headings (`#`, `##`, `###`):** High-contrast bold styling with subtle slate prefixes.
2. **Quotes (`>`):** Gentle italic slate styling for inner monologues.
3. **Emphasis (`**bold**`, `*italic*`):** Natural typographic emphasis without hiding syntax.
4. **Lists (`-`, `*`, `1.`):** Clean bullet accents.
5. **Scene Breaks (`---`, `***`):** Quiet horizontal pauses for narrative pacing.

---

## Build Requirements

- C++17 compiler (Clang / GCC / MSVC)
- CMake 3.16+
- Qt 6.5+ (Widgets, Test)

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./Felt
```
