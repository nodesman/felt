# Felt Incremental Stories & Zinsser Test Suite Roadmap

**Project**: Felt Writer (`[FELT]` in `notionworks` Plane workspace)  
**Target Outcome**: Bridge the C++17 / Qt 6.5.3 editor to 100% visual, behavioral, and Object Calisthenics conformance, governed by William Zinsser `describe()` / `it()` specifications, 100% LLVM branch coverage on core domain modules, and context-specific LLM Semantic Code Audits.

---

## Explicit v1 Scope Decisions (5 Accepted Refinements & 2 Declined Non-Goals)

### Accepted Into v1 Specifications & Tests
1. **WCAG Contrast Math & Headroom Fix (`#4C525B` vs `#3E4249`)**: Raised *Obsidian Void* `textDimmed` from `#3E4249` (`1.897:1`) to **`#4C525B`** (**`2.43:1`** against `#0D0F14` vignette center, providing comfortable headroom above the `2.0:1` floor). All WCAG contrast checks are measured against `bgVignetteCenter`.
2. **Single Source of Truth for Palette Hex Values**: `ThemeRegistry` (`theme.h` / `themeregistry.h`) is the only place hex strings live. All editor, highlighter, and E2E tests assert against `ThemeRegistry::obsidianVoid().textMain` and `textDimmed`.
3. **Atomic Single-Step `Cmd+Z` Undo on Smart Lists**: All smart list mutations (`Enter`, `Tab`, `Shift+Tab`) are wrapped in `QTextCursor::beginEditBlock()` and `endEditBlock()` (early-return-safe) so a single `Cmd+Z` reverts both the auto-inserted marker and newline.
4. **Escaped Asterisks (`\*`), Ordered Lists (`1. ` -> `2. `), Multibyte UTF-8 Word Count & Save Failure Safety**: Added explicit `it()` specifications for backslash-escaped asterisks, numbered list auto-increment, multibyte UTF-8 word counting, UTF-8/newline save fidelity, and read-only save failure preservation.
5. **Real Offscreen Window Activation, Unbundled Claims & Context-Specific LLM Semantic Audits**: `FeltAppDriver` calls `window.show()`, `window.activateWindow()`, and `QTest::qWaitForWindowActive(&window)` under `QT_QPA_PLATFORM=offscreen` so `editor->hasFocus()` is genuinely `true` without `|| true`. Every story includes context-specific LLM Semantic Code Audit checks in its Brake Check.

### Explicitly Declined for v1 (Kept Out of Scope to Prevent Bloat)
1. **No Hard-Wrapped Multi-Line Paragraph Grouping in Focus Mode**: Felt is a soft-wrapping prose editor (`QPlainTextEdit::WidgetWidth` inside a `66ch` measure). A paragraph is strictly a single `QTextBlock`.
2. **No ICU Dictionary Word Segmentation for CJK**: `DocumentStats` (`<= 80` lines, zero ICU dependencies) uses fast deterministic whitespace tokenization, verified for multibyte UTF-8 safety.

---

## Epic 1: Unified Frameless Window, Multi-Theme Engine & Minimal Preferences

### Story 1: `[FELT-UI-1]` Implement Native Unified Frameless Window Chrome & Smart List Keystrokes
```cpp
describe("FeltEditor list continuation and indentation", [] {
    it("continues unordered hyphen, asterisk, and plus bullets on enter");
    it("increments numbered list markers on enter after non-empty item");
    it("splits list item text cleanly at the cursor when enter is pressed mid-line");
    it("preserves leading indentation when continuing nested list items");
    it("clears empty list marker and restores plain paragraph on second enter");
    it("indents active list item by four spaces when tab is pressed");
    it("unindents nested list item by four spaces when shift-tab is pressed");
    it("reverts auto-inserted list marker and newline in a single undo step");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Early-Return-Safe `beginEditBlock`)**: Read `FeltEditor::keyPressEvent` in `editor.cpp` and verify that every execution path after `cursor.beginEditBlock()` is guaranteed to reach `cursor.endEditBlock()` (or uses an RAII scope guard) so an early return never wedges the `QTextDocument` undo stack open.
  - [ ] **LLM Semantic Check 2 (Cocoa Hit-Testing vs. Viewport Selection)**: Read `windowchrome.mm` and `editor.cpp` to verify top-margin window dragging does not swallow mouse clicks inside the editor viewport or reintroduce recursive `sendEvent` forwarding.
  - [ ] **LLM Multimodal Visual Check**: Inspect live screenshot of `Felt.app` to confirm macOS traffic lights sit at `(18, 18)` against a unified canvas with zero grey title-bar band.

### Story 2: `[FELT-UI-2]` Build Multi-Theme Engine & Ambient Tactile Sheen
```cpp
describe("Canvas Tactile Aesthetics and Live Theming", [] {
    it("enforces one hundred sixty percent proportional line height across initial blocks");
    it("applies one hundred sixty percent line height automatically to newly inserted and pasted blocks");
    it("illuminates active block in palette main text color while dimming inactive blocks to palette dimmed color");
    it("updates canvas vignette and editor text colors live when switching palettes");
    it("updates highlighter syntax colors and bottom hud colors live when switching palettes");
    it("preserves cursor position and undo history across live theme transitions");
    it("preserves unmodified document clean state across theme and focus mode transitions");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Zero `Cmd+Z` Undo-Stack Pollution During Visual Dimming)**: Read `FeltEditor::updateFocusMode()` and theme-switching code in `editor.cpp` to verify that applying dimmed/active block formats does **not** record format-change entries onto the user's `QTextDocument` undo stack (`setModified(false)` alone is insufficient if `Cmd+Z` undoes Focus Mode colors instead of user text).
  - [ ] **LLM Semantic Check 2 (True Radial Vignette Painting)**: Verify viewport background paints a `QRadialGradient` from `palette.bgVignetteCenter` (`#0D0F14`) to `palette.bgOuter` (`#08090C`), not a flat CSS fill.
  - [ ] **LLM Semantic Check 3 (Zero Stray Color Values)**: Verify every color in `editor.cpp`, `highlighter.cpp`, and `documenthud.cpp` is read dynamically from the active `ThemePalette`.

### Story 3: `[FELT-UI-3]` Create Minimalist Preferences & Settings Sheet (`Cmd+,`)
```cpp
describe("Minimalist Preferences Sheet (Cmd+,)", [] {
    it("opens centered over the canvas when triggered by shortcut or menu");
    it("reuses single dialog instance when preferences shortcut is pressed repeatedly");
    it("synchronizes switch states with live keyboard-toggled focus and typewriter modes upon opening");
    it("emits selected palette key immediately when a theme swatch is clicked");
    it("emits clamped font size and column measure as sliders move");
    it("toggles focus mode, typewriter scrolling, and hud visibility switches");
    it("dismisses cleanly when escape key is pressed");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Loop-Free Two-Way UI Sync via `QSignalBlocker`)**: Read `settingssheet.cpp` to verify that syncing controls with live keyboard-toggled states (`Cmd+D` Focus Mode, `Cmd+T` Typewriter Mode) uses `QSignalBlocker` so opening the dialog does not re-emit change signals in a feedback loop.
  - [ ] **LLM Semantic Check 2 (Single-Instance Modal Lifecycle)**: Verify pressing `Cmd+,` repeatedly raises/reuses the existing `SettingsSheet` instance cleanly without leaking `QDialog` pointers.
  - [ ] **LLM Multimodal Visual Check**: Compare live screenshot of `Cmd+,` sheet against `_meta/mockups/preferences_settings_modal.jpg`.

### Story 4: `[FELT-UI-4]` Connect Live Canvas Reactive Sync & QSettings Persistence
```cpp
describe("Preferences State Machine and Storage Persistence", [] {
    it("roundtrips theme, typography, measure, and dynamics settings through storage");
    it("clamps out of range font sizes to thirteen through twenty six points");
    it("clamps out of range column widths to five hundred twenty through nine hundred sixty pixels");
    it("falls back to priority monospace family when stored font name is unavailable on system");
    it("restores default obsidian configuration when storage keys are missing or corrupted");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Domain Ownership of Clamping & Fallbacks)**: Read `preferences.h/cpp` and `mainwindow.cpp` to verify all clamping rules (`13–26pt` font size, `520–960px` column width) and corrupt-key fallbacks live inside the `Preferences` domain class, not scattered across UI sliders.
  - [ ] **LLM Semantic Check 2 (No Layout Thrashing on Slider Drag)**: Verify moving the column-width slider recalculates viewport margins directly without reconstructing `FeltHighlighter` or reloading fonts on every pixel tick.

### Story 5: `[FELT-UI-5]` Codify `FeltAppDriver` End-to-End Functional User Journeys
```cpp
describe("FeltAppDriver end-to-end writing journeys", [] {
    it("activates editor focus genuinely on startup under offscreen platform");
    it("formats headings, quotes, and emphasis in situ while updating reading stats");
    it("scaffolds, splits, indents, and terminates markdown lists from live keystrokes");
    it("isolates the active paragraph in focus mode and centers the cursor in typewriter mode");
    it("live-switches themes, font sizes, and column measures from the preferences sheet");
    it("preserves utf8 multibyte prose and trailing newlines across save and load");
    it("retains dirty state and buffer contents when saving to a read-only path fails");
    it("handles mouse clicks across viewport and side margins without event recursion");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Genuine User-Input Simulation vs. Backdoor Calls)**: Read `tests/felt_app_driver.h` and verify driver actions dispatch real Qt key, mouse, and QAction events through the widget hierarchy rather than calling internal helper methods directly.
  - [ ] **LLM Semantic Check 2 (Full-State Journey Assertions)**: Verify each of the 8 E2E journeys asserts the complete observable user outcome (editor text, block/char formatting, HUD string, dirty flag, and saved disk contents).

### Story 5b: `[FELT-UI-6]` Mount Invisible Top Titlebar Drag Strip & Restore High-Contrast Selection Highlight
```cpp
describe("Frameless Titlebar Drag Strip and Selection Highlight", [] {
    it("mounts a thirty-six pixel invisible drag strip across the top titlebar zone");
    it("moves the main window position when dragging across the top titlebar strip");
    it("restores solid canvas background and high-contrast selection highlight on editor");
    it("preserves text cursor selection below the drag strip without moving the window");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Top `0..36px` Drag Strip Separation)**: Read `titlebardragstrip.h/cpp` and `mainwindow.cpp` to verify a dedicated `36px`-high `TitleBarDragStrip` sits at the top of `MainWindow`'s vertical layout (`y = 0..36`, above `FeltEditor`) so clicking and dragging in the top 10–36px titlebar zone moves the window cleanly without `FeltEditor` intercepting mouse presses.
  - [ ] **LLM Semantic Check 2 (Solid Canvas & Selection Highlight Integrity)**: Read `editor.cpp` to verify `FeltEditor` uses a solid `background-color` and `selection-background-color` (`#1E3A8A` on Obsidian Void) rather than `background: transparent` (which suppresses Qt's `QStyleSheetStyle` selection fill).

---

## Epic 2: Compiler-Grade Lexical Grammar & In-Situ Conformance

### Story 6: `[FELT-LEX-1]` Build Pure Markdown Grammar Lexer & Invariant Rules
```cpp
describe("Markdown Block Grammar Lexer", [] {
    it("parses heading levels one through six into receding prefix and title spans");
    it("rejects hash markers lacking a trailing space as plain paragraphs");
    it("rejects seven or more leading hashes as plain paragraph text");
    it("identifies unordered hyphen, asterisk, and plus list bullets with leading indentation");
    it("parses blockquotes and three-character horizontal scene breaks");
    it("distinguishes three-character scene breaks from hyphenated prose and list items");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Zero-Allocation `QStringView` Linear Scan)**: Read `grammar.h/cpp` to verify block lexing scans `QStringView` directly in $O(N)$ linear time—zero `QRegularExpression` instantiations per call and zero heap-allocated `QString` substring copies.
  - [ ] **LLM Semantic Check 2 (Pure Domain Isolation)**: Verify `grammar.h/cpp` has zero GUI/widget dependencies and returns pure value structs (`BlockToken`, `InlineSpan`).

### Story 7: `[FELT-LEX-2]` Author Compiler-Grade Inline Delimiter Conformance Suite
```cpp
describe("Markdown Inline Emphasis Delimiter Lexer", [] {
    it("matches balanced double-asterisk bold spans within a paragraph");
    it("matches balanced single-asterisk italic spans without colliding with bold");
    it("isolates multiple bold and italic spans on the same line without greedy bridging");
    it("ignores backslash-escaped asterisks inside prose");
    it("ignores leading asterisk list markers when scanning inline italics");
    it("ignores space-padded multiplication asterisks in arithmetic expressions");
    it("traps and ignores unclosed or mismatched asterisk delimiters");
    it("preserves raw markdown characters byte-for-byte without html mutation");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Exact Span Offset & Length Assertions)**: Read `tests/test_grammar_conformance.cpp` to verify every `it()` block asserts the **exact start index, length, and token type** of both the opening/closing delimiter (`*`, `**`) and inner content—never a weak check like `!spans.isEmpty()`.
  - [ ] **LLM Semantic Check 2 (No Quadratic Backtracking on Unclosed/Escaped Asterisks)**: Verify in `grammar.cpp` that scanning a line with multiple unclosed `*` or escaped `\*` characters runs in a single linear pass without nested backtracking loops.

---

## Epic 3: Object Calisthenics Core & Architectural Conformance

### Story 8: `[FELT-CORE-1]` Implement Immutable `ThemePalette` & `ThemeRegistry`
```cpp
describe("ThemeRegistry and WCAG Contrast Invariants", [] {
    it("resolves obsidian, parchment, slate, and midnight palettes by key");
    it("falls back to obsidian void when queried with an unknown key");
    it("enforces wcag aaa contrast ratio above seven to one between body text and vignette center");
    it("enforces dimmed text contrast between two and four to one against vignette center for focus mode");
    it("enforces high-visibility contrast for active caret and text selection across all palettes");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Exact WCAG 2.1 Relative Luminance Formula)**: Read `theme.cpp` to verify the contrast calculation implements the true WCAG 2.1 piecewise sRGB transfer function (`0.04045` linear threshold and `2.4` gamma exponent) measured against `bgVignetteCenter`, rather than a simplified perceived-brightness shortcut.
  - [ ] **LLM Semantic Check 2 (True Value-Object Immutability & `#4C525B` Headroom)**: Verify `ThemePalette` cannot be mutated after construction and that Obsidian Void `textDimmed` (`#4C525B`) provides `2.43:1` contrast headroom against `#0D0F14`.

### Story 9: `[FELT-CORE-2]` Decouple Document Measure Calculus into Tiny Value Object
```cpp
describe("Document Measure and Viewport Margin Calculus", [] {
    it("centers the writing column symmetrically when window width exceeds measure");
    it("clamps side margins to forty pixels when window is narrower than measure");
    it("clamps configured column width within five hundred twenty and nine hundred sixty pixels");
    it("preserves eighty pixel top and bottom breathing margins across all window sizes");
    it("prevents negative viewport geometry when window width is smaller than minimum margins");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Tell, Don't Ask Delegation)**: Read `measure.h/cpp` and `editor.cpp` to verify `FeltEditor::updateMargins()` delegates 100% of margin and column-width clamping math to `DocumentMeasure` (zero raw geometry arithmetic left inside `editor.cpp`).

### Story 10: `[FELT-CORE-3]` Extract `DocumentHUD` and `MenuBuilder` & Eliminate All `else` Branches
```cpp
describe("DocumentStats word and reading-time counter", [] {
    it("returns zero words and zero minutes for empty or whitespace-only documents");
    it("computes word, character, and non-space counts across multi-paragraph prose");
    it("counts whitespace-delimited words accurately across multibyte utf8 scripts");
    it("rounds reading time at two hundred words per minute with one minute floor for non-empty text");
    it("formats summary string with middle-dot separator for the bottom hud");
});

describe("Declarative Menu Actions and Font Priority Resolution", [] {
    it("selects first available monospace family from ordered priority list without else branches");
    it("falls back to system fixed font when no priority family is installed");
    it("binds file, view, focus, typewriter, and preferences shortcuts declaratively");
    it("synchronizes checkable menu actions with editor focus and typewriter states");
});
```
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Genuine Guard Clauses vs. Disguised `else`)**: Read `editor.cpp` and `mainwindow.cpp` to verify that removing the 10 `else` branches used genuine early returns and data-driven iteration (the font priority list), not nested ternaries (`a ? (b ? c : d) : e`) or inverted `if (!cond)` blocks following a non-returning `if (cond)`.
  - [ ] **LLM Semantic Check 2 (Cohesive Single-Responsibility Extraction)**: Verify `DocumentHUD` and `MenuBuilder` are clean, cohesive components with $\le 2$ instance variables, not arbitrary slices of `MainWindow` created just to pass the 200-line limit.

---

## Epic 4: Verification Gate & 100% Branch Coverage

### Story 11: `[FELT-GATE-1]` Implement Automated Calisthenics, Zinsser & Auditor Verification Gate
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check (Mandatory Pre-Done Audit)**: Verify that no story is marked `Done` until both the automated `task gate` script exits `0` **and** an LLM Auditor has verified the story-specific semantic checks above on the git diff.

### Story 12: `[FELT-TEST-1]` Build Zinsser BDD Harness (`zinsser.h`), 100% LLVM Branch Coverage & Legacy Test Refactor
- **Context-Specific LLM Auditor Gate**:
  - [ ] **LLM Semantic Check 1 (Assertion Strength & Honesty Audit Across All 63 `it()` Blocks)**: Read every `it()` block body across the test suite and verify that:
    1. Every `it()` body asserts the exact behavioral claim in its title (`QCOMPARE` on exact strings, coordinates, and `ThemePalette` colors—zero weak truthy checks).
    2. Every `it()` tests a distinct failure mode that would deserve to exist even without the `>= 4` rule (zero padding).
    3. All legacy tests in `tests/test_felt.cpp` use `ThemeRegistry` colors and real offscreen focus activation (`show()`, `activateWindow()`, `qWaitForWindowActive()`).
