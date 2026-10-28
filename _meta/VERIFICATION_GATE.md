# Felt Verification Gate (v2.0)

**Project**: Felt Writer (`[FELT]`)  
**Objective**: Deterministic automated and LLM-audited quality gate guaranteeing 100% Object Calisthenics conformance, compiler-grade grammar correctness, 100% LLVM branch coverage on domain core modules, and clutter-free William Zinsser test prose before any story is marked **Done**.

---

## 1. Object Calisthenics Invariants (The 9 Rules)
- [ ] **Rule 1 (Single Level of Indentation)**: Max 1 level of indentation per method. Use early returns and guard clauses.
- [ ] **Rule 2 (No `else` Keyword)**: Zero occurrences of `else` across all C++ headers and implementations.
- [ ] **Rule 3 (Wrap Primitives)**: Colors, measures, and statistics wrapped in tiny domain value objects (`ThemePalette`, `DocumentMeasure`, `DocumentStats`).
- [ ] **Rule 4 (First-Class Collections)**: Theme registries and grammar rule sets held in dedicated collection objects (`ThemeRegistry`, `MarkdownGrammar`).
- [ ] **Rule 5 (One Dot Per Line)**: Strict Law of Demeter. No chaining through internal object graphs.
- [ ] **Rule 6 (No Abbreviations)**: Identifiers written out in full domain clarity (`highlighter`, not `hl`; `characterCount`, not `charCnt`).
- [ ] **Rule 7 (Small Entities)**: Every `.h` and `.cpp` file strictly $\le 200$ lines.
- [ ] **Rule 8 (Max 2 Instance Variables)**: Non-coordinator domain classes have at most 2 state variables.
- [ ] **Rule 9 (No Getters/Setters on Complex State)**: Tell, don't ask; push behavior into the value objects.

---

## 2. William Zinsser Test Suite & Auditor Standard
- [ ] **Prose Clarity**: Tests read as muscular human declarations of system truth using nested `describe()` and `it()` blocks (`tests/zinsser.h`).
- [ ] **Zero Bureaucratic Fillers**: Forbidden words: `should`, `properly`, `correctly`, `successfully`, `in order to`, `works as expected`.
- [ ] **Active Present-Tense Verbs**: Every `it()` title starts with an active present-tense verb (`highlights`, `illuminates`, `rejects`, `preserves`, `continues`, `increments`, `reverts`, `clears`, `indents`, `unindents`, `enforces`, `dims`, `traps`, `ignores`, `counts`, `retains`, `activates`).
- [ ] **Multi-Scenario `describe()` Rule**: Every `describe()` block in `tests/` must contain **at least 4 `it()` blocks** (`>= 4` scenarios per `describe()`).
- [ ] **Auditor Rule for `it()` Blocks (Single Failure Mode / Zero Padding)**: *"Every `it()` block must test a single distinct failure mode that would exist even without a minimum count rule (zero artificial padding)."* Multi-claim `it()` titles must be unbundled into focused assertions.
- [ ] **Zero Tautologies & Real Offscreen Focus Activation**: Forbidden patterns: `|| true`, `true ||`, `QVERIFY(true)`, `expect(true).toBe(true)`. Under `QT_QPA_PLATFORM=offscreen`, `FeltAppDriver` must call `window.show()`, `window.activateWindow()`, and `QTest::qWaitForWindowActive(&window)` so `editor->hasFocus()` evaluates to `true` genuinely.
- [ ] **Single Source of Truth for Palette Hex Values**: `ThemeRegistry` (`theme.h` / `themeregistry.h`) is the sole authority for palette hex literals. All other test suites (`editor`, `highlighter`, `FeltAppDriver`) must assert against `ThemeRegistry::obsidianVoid().textMain` / `ThemeRegistry::obsidianVoid().textDimmed` rather than hardcoding duplicate hex strings.

---

## 3. Visual Craft, WCAG Contrast & Physical Sheen Gate
- [ ] **Ambient Vignette**: Canvas renders a radial gradient (`bgVignetteCenter` `#0D0F14` softly tapering to `bgOuter` `#08090C` in *Obsidian Void*), eliminating flat `<textarea>` appearance.
- [ ] **WCAG Contrast Reference (`bgVignetteCenter`)**: All WCAG contrast ratios are strictly measured against `bgVignetteCenter`:
  - Body text (`textMain` `#ECEBE8` vs `#0D0F14`): $\ge 7.0:1$ (WCAG AAA).
  - Focus Mode dimmed text (`textDimmed` `#464B53` vs `#0D0F14`): strictly within $[2.0:1, 4.0:1]$ (`#464B53` yields $2.15:1$ against `#0D0F14` center and $2.29:1$ against `#08090C` outer, replacing `#3E4249` which failed at $1.897:1$).
- [ ] **Vertical Rhythm**: $1.6\times$ (160%) proportional line height enforced across initial, newly inserted, and pasted blocks without marking unmodified documents dirty.
- [ ] **Atomic Smart List Undo**: Smart list `Enter`, `Tab`, and `Shift+Tab` mutations wrapped in `QTextCursor::beginEditBlock()` / `endEditBlock()` so a single `Cmd+Z` reverts both the auto-inserted marker and newline.
- [ ] **Frameless Chrome**: Native Cocoa `NSWindowStyleMaskFullSizeContentView` on macOS with inset traffic lights at `(18, 18)`.
- [ ] **Transparent Icon**: 100% transparent squircle icon in macOS Dock (`alpha = 0` outside rounded perimeter).

---

## 4. Explicit v1 Non-Goals (Scope Boundaries Against Bloat)
1. **No Hard-Wrapped Multi-Line Paragraph Grouping in Focus Mode**: Felt uses soft-wrapping (`QPlainTextEdit::WidgetWidth` inside a `66ch` measure), so a paragraph is strictly a single `QTextBlock`.
2. **No ICU Dictionary Word Segmentation for CJK**: `DocumentStats` (`<= 80` lines, zero ICU dependencies) uses fast deterministic whitespace tokenization, verified for multibyte UTF-8 safety across non-Latin scripts (such as Tamil and accented UTF-8 prose).

---

## 5. Automated Conformance Check Command
Before completing any story, execute:
```bash
task gate
# Which runs: python3 scripts/verify_conformance.py && ctest --test-dir build --output-on-failure
```
All checks (Object Calisthenics, Zinsser prose, `>= 4` `it()` per `describe()`, zero tautologies, and 100.0% core LLVM branch coverage) must pass with exit code `0`.
