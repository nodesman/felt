# Felt Verification Gate (v3.0)

**Project**: Felt Writer (`[FELT]`)  
**Objective**: Deterministic automated and LLM-audited quality gate guaranteeing 100% Object Calisthenics conformance, compiler-grade grammar correctness, 100% LLVM branch coverage on domain core modules, clutter-free William Zinsser test prose, and context-specific LLM Semantic Code Audits before any story is marked **Done**.

---

## 1. Object Calisthenics Invariants (The 9 Rules)
- [ ] **Rule 1 (Single Level of Indentation)**: Max 1 level of indentation per method. Use early returns and guard clauses.
- [ ] **Rule 2 (No `else` Keyword)**: Zero occurrences of `else` across all C++ headers and implementations (and zero disguised `else` branches via nested ternaries `? : ? :`).
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
- [ ] **Multi-Scenario `describe()` Rule**: Every `describe()` block in `tests/` must contain **at least 4 unique `it()` blocks** (`>= 4` scenarios per `describe()`).
- [ ] **Auditor Rule for `it()` Blocks (Single Failure Mode / Zero Padding)**: *"Every `it()` block must test a single distinct failure mode that would exist even without a minimum count rule (zero artificial padding)."* Multi-claim `it()` titles must be unbundled into focused assertions.
- [ ] **Zero Tautologies & Real Offscreen Focus Activation**: Forbidden patterns: `|| true`, `true ||`, `QVERIFY(true)`, `expect(true).toBe(true)`. Under `QT_QPA_PLATFORM=offscreen`, `FeltAppDriver` must call `window.show()`, `window.activateWindow()`, and `QTest::qWaitForWindowActive(&window)` so `editor->hasFocus()` evaluates to `true` genuinely.
- [ ] **Single Source of Truth for Palette Hex Values**: `ThemeRegistry` (`theme.h` / `themeregistry.h`) is the sole authority for palette hex literals. All other test suites (`editor`, `highlighter`, `FeltAppDriver`) must assert against `ThemeRegistry::obsidianVoid().textMain` / `ThemeRegistry::obsidianVoid().textDimmed` rather than hardcoding duplicate hex strings.

---

## 3. Visual Craft, WCAG Contrast & Physical Sheen Gate
- [ ] **Ambient Vignette**: Canvas renders a `QRadialGradient` (`bgVignetteCenter` `#0D0F14` softly tapering to `bgOuter` `#08090C` in *Obsidian Void*), eliminating flat `<textarea>` appearance.
- [ ] **WCAG Contrast Reference (`bgVignetteCenter`)**: All WCAG contrast ratios are strictly measured against `bgVignetteCenter` using the exact WCAG 2.1 piecewise sRGB transfer function (`0.04045` threshold, `2.4` gamma):
  - Body text (`textMain` `#ECEBE8` vs `#0D0F14`): $\ge 7.0:1$ (WCAG AAA, $15.2:1$).
  - Focus Mode dimmed text (`textDimmed` `#4C525B` vs `#0D0F14`): strictly within $[2.0:1, 4.0:1]$ (`#4C525B` yields **$2.43:1$** against `#0D0F14` vignette center, providing comfortable headroom above the $2.0:1$ floor).
- [ ] **Vertical Rhythm**: $1.6\times$ (160%) proportional line height enforced across initial, newly inserted, and pasted blocks without marking unmodified documents dirty.
- [ ] **Atomic Smart List Undo**: Smart list `Enter`, `Tab`, and `Shift+Tab` mutations wrapped in `QTextCursor::beginEditBlock()` / `endEditBlock()` (guaranteed across early returns) so a single `Cmd+Z` reverts both the auto-inserted marker and newline.
- [ ] **Frameless Chrome**: Native Cocoa `NSWindowStyleMaskFullSizeContentView` on macOS with inset traffic lights at `(18, 18)`.
- [ ] **Transparent Icon**: 100% transparent squircle icon in macOS Dock (`alpha = 0` outside rounded perimeter).

---

## 4. Context-Specific LLM Semantic Code Audit Gates (Per-Story Brake Checks)
Before any story is marked **Done**, an LLM Auditor must inspect the C++ implementation and test bodies against that story's context-specific semantic checks:

1. **`[FELT-UI-1]` (Frameless Chrome & Smart Lists)**:
   - **Early-Return-Safe `beginEditBlock()`**: Verify in `editor.cpp` that every path after `cursor.beginEditBlock()` is guaranteed to reach `cursor.endEditBlock()` (or uses an RAII scope guard) so an early return never leaves the `QTextDocument` undo stack wedged open.
   - **Cocoa Hit-Testing vs. Viewport Selection**: Verify in `windowchrome.mm` and `editor.cpp` that top-margin window dragging never swallows mouse clicks inside the editor viewport or reintroduces recursive `sendEvent` forwarding.
   - **Multimodal Screenshot Check**: Verify macOS traffic lights sit at `(18, 18)` with zero grey title-bar band.
2. **`[FELT-UI-2]` (Multi-Theme Engine & Ambient Tactile Sheen)**:
   - **Zero `Cmd+Z` Undo-Stack Pollution During Visual Dimming**: Verify in `FeltEditor::updateFocusMode()` and theme switching that applying dimmed/active block formats does **not** record format changes onto the user's `QTextDocument` undo stack (`setModified(false)` alone is insufficient if `Cmd+Z` undoes Focus Mode colors instead of user text).
   - **True Radial Vignette Painting**: Verify the viewport paints a `QRadialGradient` from `palette.bgVignetteCenter` to `palette.bgOuter`, not a flat CSS fill.
   - **Zero Stray Color Literals**: Verify every color in `editor.cpp`, `highlighter.cpp`, and `documenthud.cpp` comes from `ThemePalette`.
3. **`[FELT-UI-3]` (Preferences & Settings Sheet `Cmd+,`)**:
   - **Loop-Free Two-Way Sync via `QSignalBlocker`**: Verify in `settingssheet.cpp` that syncing switch controls with live keyboard-toggled states (`Cmd+D`, `Cmd+T`) uses `QSignalBlocker` so opening the dialog does not re-emit change signals in a feedback loop.
   - **Single-Instance Modal Lifecycle**: Verify pressing `Cmd+,` repeatedly reuses the dialog instance without leaking `QDialog` pointers.
   - **Multimodal Screenshot Check**: Compare live `Cmd+,` sheet screenshot against `_meta/mockups/preferences_settings_modal.jpg`.
4. **`[FELT-UI-4]` (Live Canvas Reactive Sync & `QSettings` Persistence)**:
   - **Domain Ownership of Clamping & Fallbacks**: Verify clamping (`13–26pt` font size, `520–960px` measure) and corrupt-key fallbacks live inside `Preferences`, not inside UI slider widgets.
   - **Zero Layout Thrashing on Slider Drag**: Verify dragging the column-width slider updates viewport margins directly without reconstructing `FeltHighlighter` or reloading fonts on every tick.
5. **`[FELT-UI-5]` (`FeltAppDriver` End-to-End User Journeys)**:
   - **Genuine User-Input Simulation vs. Backdoor Calls**: Verify `FeltAppDriver` dispatches real Qt keyboard, mouse, and QAction events through the widget hierarchy rather than calling private/internal helper methods directly.
   - **Full-State Journey Assertions**: Verify each of the 8 E2E journeys asserts the complete observable outcome (buffer text, character/block formats, HUD string, dirty flag, and on-disk file contents).
6. **`[FELT-LEX-1]` (Pure Markdown Grammar Lexer)**:
   - **Zero-Allocation `QStringView` Linear Scan**: Verify `grammar.h/cpp` scans `QStringView` directly in $O(N)$ linear time with zero per-call `QRegularExpression` constructions, zero heap `QString` substring copies, and zero GUI/widget headers.
7. **`[FELT-LEX-2]` (Compiler-Grade Inline Delimiter Conformance Suite)**:
   - **Exact Span Offset & Length Assertions**: Verify every `it()` in `tests/test_grammar_conformance.cpp` asserts the exact start index, length, and token kind of both the delimiter and inner styled text—never weak `!spans.isEmpty()` checks.
   - **No Quadratic Backtracking**: Verify pathological lines with unclosed `*` or escaped `\*` scan in a single linear pass.
8. **`[FELT-CORE-1]` (Immutable `ThemePalette` & `ThemeRegistry`)**:
   - **Exact WCAG 2.1 Relative Luminance Formula**: Verify `theme.cpp` implements the true WCAG 2.1 piecewise sRGB transfer function (`0.04045` linear threshold, `2.4` gamma exponent) measured against `bgVignetteCenter`.
   - **True Immutability & `#4C525B` Headroom**: Verify `ThemePalette` fields cannot be mutated after construction and Obsidian Void `textDimmed` (`#4C525B`) provides `2.43:1` contrast headroom against `#0D0F14`.
9. **`[FELT-CORE-2]` (Document Measure Calculus)**:
   - **Tell, Don't Ask Delegation**: Verify `FeltEditor::updateMargins()` delegates 100% of geometry and clamping math to `DocumentMeasure` with zero raw margin arithmetic left inside `editor.cpp`.
10. **`[FELT-CORE-3]` (Extract `DocumentHUD` & `MenuBuilder` & Eliminate `else`)**:
    - **Genuine Guard Clauses vs. Disguised `else`**: Verify all 10 eliminated `else` branches use genuine early returns and data-driven iteration (font priority list), with zero nested ternaries (`? : ? :`) or inverted `if (!cond)` blocks following a non-returning `if (cond)`.
    - **Cohesive Single-Responsibility Extraction**: Verify `DocumentHUD` and `MenuBuilder` are cohesive components ($\le 2$ instance variables), not arbitrary line-count slices.
11. **`[FELT-GATE-1]` (Automated Gate & Pre-Done Audit)**:
    - **Mandatory Pre-Done Audit**: No story is marked `Done` until `task gate` exits `0` AND the LLM Auditor verifies the story's semantic checks on the git diff.
12. **`[FELT-TEST-1]` (Zinsser BDD Harness & 100% Branch Coverage)**:
    - **Assertion Strength & Honesty Audit Across All 63 `it()` Blocks**: Verify every `it()` body asserts the exact behavioral claim in its title (`QCOMPARE` on exact strings, offsets, and `ThemePalette` colors), has zero artificial padding, and uses real offscreen window focus activation (`show()`, `activateWindow()`, `qWaitForWindowActive()`).

---

## 5. Explicit v1 Non-Goals (Scope Boundaries Against Bloat)
1. **No Hard-Wrapped Multi-Line Paragraph Grouping in Focus Mode**: Felt uses soft-wrapping (`QPlainTextEdit::WidgetWidth` inside a `66ch` measure), so a paragraph is strictly a single `QTextBlock`.
2. **No ICU Dictionary Word Segmentation for CJK**: `DocumentStats` (`<= 80` lines, zero ICU dependencies) uses fast deterministic whitespace tokenization, verified for multibyte UTF-8 safety across non-Latin scripts (such as Tamil and accented UTF-8 prose).

---

## 6. Automated Conformance Check Command
Before completing any story, execute:
```bash
task gate
# Which runs: python3 scripts/verify_conformance.py && ctest --test-dir build --output-on-failure
```
All checks (Object Calisthenics, Zinsser prose, `>= 4` unique `it()` per `describe()`, zero tautologies, and 100.0% core LLVM branch coverage) must pass with exit code `0`.
