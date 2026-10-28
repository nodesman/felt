#!/usr/bin/env python3
import subprocess

cli = '/Users/rajsekharan/projects/foundry/kernel/skills/plane-ops/scripts/plane_client.py'
workspace = 'notionworks'
project = '704df24c-5db5-4578-86e2-67448b83ab78'

updates = [
    {
        'id': '3c902805-5803-48f1-98a3-8630eca86f95',
        'title': '[FELT-UI-1] Implement Native Unified Frameless Window Chrome & Smart List Keystrokes',
        'desc': """## Outcome Contract & Deliverables
- **Native Unified Frameless Window (`windowchrome.h/mm`, <= 60 lines)**:
  - Strip default OS title bar on macOS via `NSWindowStyleMaskFullSizeContentView`, `titlebarAppearsTransparent = YES`, and `NSWindowTitleHidden`.
  - Position native traffic lights at `(18, 18)` inside the top margin and support smooth window dragging on upper padding without interfering with text selection.
- **Smart Markdown List Keystrokes (`editor.cpp`)**:
  - `Enter` at end or mid-line of an unordered (`- `, `* `, `+ `) or ordered (`1. `) list item continues or splits the bullet cleanly while preserving leading indentation.
  - `Enter` on an empty list marker (`- `, `* `, `+ `, `1. `) clears the marker and restores a standard paragraph.
  - `Tab` indents by 4 spaces; `Shift+Tab` (`Backtab`) un-indents by up to 4 spaces.
  - Wrap all smart list mutations in `QTextCursor::beginEditBlock()` and `endEditBlock()` so a single `Cmd+Z` reverts both the auto-inserted marker and newline atomically.

## Zinsser Functional Spec (8 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Early-Return-Safe `beginEditBlock`)**: Read `FeltEditor::keyPressEvent` in `editor.cpp` and verify that every execution path after `cursor.beginEditBlock()` is guaranteed to reach `cursor.endEditBlock()` (or uses an RAII scope guard) so an early return never leaves the `QTextDocument` undo stack wedged open.
- [ ] **LLM Semantic Check 2 (Cocoa Hit-Testing vs. Viewport Selection)**: Read `windowchrome.mm` and `editor.cpp` to verify top-margin window dragging does not swallow mouse clicks inside the editor viewport or reintroduce recursive `sendEvent` forwarding.
- [ ] **LLM Multimodal Visual Check**: Inspect a live screenshot of `Felt.app` on macOS to confirm traffic lights sit at `(18, 18)` against a unified canvas with zero grey title-bar band.
- [ ] All 8 `it()` scenarios pass via `FeltAppDriver` keystroke simulation; zero `else` keywords, <= 200 lines per file."""
    },
    {
        'id': '410d443e-2506-4628-b732-382eb3da52fe',
        'title': '[FELT-UI-2] Build Multi-Theme Engine & Ambient Tactile Sheen',
        'desc': """## Outcome Contract & Deliverables
- **Ambient Tactile Sheen**:
  - Replace flat `#080808` fill with a radial vignette (`bgVignetteCenter` `#0D0F14` tapering to `bgOuter` `#08090C`) eliminating the raw `<textarea>` look.
  - Enforce 160% proportional line height (`QTextBlockFormat::ProportionalHeight`) across all initial, newly inserted, and pasted text blocks.
  - Apply `ThemeRegistry::obsidianVoid().textMain` (`#ECEBE8`) body typography and `ThemeRegistry::obsidianVoid().textDimmed` (`#4C525B`, WCAG 2.43:1 against `#0D0F14`) focus dimming without flipping `document()->isModified()`.
  - **Explicit v1 Scope Boundary**: Focus Mode illuminates the active `QTextBlock` (soft-wrapped within the `66ch` measure via `QPlainTextEdit::WidgetWidth`); multi-line hard-wrapped blocks are separate paragraphs.
- **Live Theme Application**:
  - Dynamically apply any of the 4 `ThemePalette` configurations (*Obsidian Void*, *Warm Parchment*, *Slate Concrete*, *Midnight Indigo*) to `FeltEditor`, `FeltHighlighter`, caret, and `DocumentHUD` without widget reconstruction.

## Zinsser Functional Spec (7 `it()` Scenarios — Unbundled Single-Mode Claims)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Zero `Cmd+Z` Undo-Stack Pollution During Visual Dimming)**: Read `FeltEditor::updateFocusMode()` and theme-switching code in `editor.cpp` to verify that applying dimmed/active block formats does **not** record format-change entries onto the user's `QTextDocument` undo stack (`setModified(false)` alone is insufficient if `Cmd+Z` undoes Focus Mode colors instead of user text).
- [ ] **LLM Semantic Check 2 (True Radial Vignette Painting)**: Verify viewport background paints a `QRadialGradient` from `palette.bgVignetteCenter` (`#0D0F14`) to `palette.bgOuter` (`#08090C`), not a flat CSS fill.
- [ ] **LLM Semantic Check 3 (Zero Stray Color Values)**: Verify every color in `editor.cpp`, `highlighter.cpp`, and `documenthud.cpp` is read dynamically from the active `ThemePalette`.
- [ ] All 7 `it()` blocks pass."""
    },
    {
        'id': 'a01e7c3f-7015-4643-af14-7db578dae616',
        'title': '[FELT-UI-3] Create Minimalist Preferences & Settings Sheet (Cmd+,)',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `settingssheet.h` / `settingssheet.cpp` (<= 180 lines, zero `else`) implementing the 520x480 iA Writer-style floating preferences dialog triggered by `Cmd+,`.
- **Sections**:
  1. **Appearance & Themes**: 4 tactile swatch buttons (*Obsidian*, *Parchment*, *Slate*, *Midnight*).
  2. **Typography & Measure**: Typeface selector, Font Size slider (`13-26pt`), Column Width slider (`520-960px`).
  3. **Writing Dynamics**: Toggles for Focus Mode (`Cmd+D`), Typewriter Scrolling (`Cmd+T`), In-Situ Markdown, and Bottom HUD.
  4. **Caret Personality**: Segmented control for Caret Width (`1px`, `2px`, `3px`).

## Zinsser Functional Spec (7 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Loop-Free Two-Way UI Sync via `QSignalBlocker`)**: Read `settingssheet.cpp` to verify that syncing controls with live keyboard-toggled states (`Cmd+D` Focus Mode, `Cmd+T` Typewriter Mode) uses `QSignalBlocker` so opening the dialog does not re-emit change signals in a feedback loop.
- [ ] **LLM Semantic Check 2 (Single-Instance Modal Lifecycle)**: Verify pressing `Cmd+,` repeatedly raises/reuses the existing `SettingsSheet` instance cleanly without leaking `QDialog` pointers.
- [ ] **LLM Multimodal Visual Check**: Compare live screenshot of `Cmd+,` sheet against `_meta/mockups/preferences_settings_modal.jpg`.
- [ ] File length <= 180 lines, zero `else` keywords, all 7 `it()` blocks pass."""
    },
    {
        'id': '347ebe23-181e-4cb9-9000-78fd7e921b7d',
        'title': '[FELT-UI-4] Connect Live Canvas Reactive Sync & QSettings Persistence',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `preferences.h` / `preferences.cpp` (<= 110 lines, zero `else`, 100% branch-covered) managing configuration state and `QSettings` (`com.nodesman.felt`) serialization.
- **Live Sync**: Wire `SettingsSheet` signals to `MainWindow` and `FeltEditor` so theme, font family, font size, column width, caret width, and HUD visibility update instantaneously and survive restarts.

## Zinsser Functional Spec (5 `it()` Scenarios)
```cpp
describe("Preferences State Machine and Storage Persistence", [] {
    it("roundtrips theme, typography, measure, and dynamics settings through storage");
    it("clamps out of range font sizes to thirteen through twenty six points");
    it("clamps out of range column widths to five hundred twenty through nine hundred sixty pixels");
    it("falls back to priority monospace family when stored font name is unavailable on system");
    it("restores default obsidian configuration when storage keys are missing or corrupted");
});
```

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Domain Ownership of Clamping & Fallbacks)**: Read `preferences.h/cpp` and `mainwindow.cpp` to verify all clamping rules (`13–26pt` font size, `520–960px` column width) and corrupt-key fallbacks live inside the `Preferences` domain class, not scattered across UI sliders.
- [ ] **LLM Semantic Check 2 (No Layout Thrashing on Slider Drag)**: Verify moving the column-width slider recalculates viewport margins directly without reconstructing `FeltHighlighter` or reloading fonts on every pixel tick.
- [ ] 100.0% branch coverage on `preferences.cpp` via `xcrun llvm-cov`; all 5 `it()` blocks pass."""
    },
    {
        'id': 'ad498b62-2f6d-4693-8465-82ff26a802aa',
        'title': '[FELT-UI-5] Codify FeltAppDriver End-to-End Functional User Journeys',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `tests/felt_app_driver.h` (<= 90 lines) wrapping `MainWindow`, `FeltEditor`, `SettingsSheet`, and `DocumentHUD` with high-level user actions (`type`, `pressEnter`, `pressShortcut`, `clickThemeSwatch`, `dragMeasureSlider`, `clickMargin`, `saveToFile`, `openFile`).
- **Real Offscreen Window Activation**: `FeltAppDriver` constructor must invoke `window.show()`, `window.activateWindow()`, and `QTest::qWaitForWindowActive(&window)` under `QT_QPA_PLATFORM=offscreen` so `editor->hasFocus()` evaluates to `true` genuinely without `|| true`.

## Zinsser End-to-End Journey Spec (8 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Genuine User-Input Simulation vs. Backdoor Calls)**: Read `tests/felt_app_driver.h` and verify driver actions dispatch real Qt key, mouse, and QAction events through the widget hierarchy rather than calling internal helper functions directly.
- [ ] **LLM Semantic Check 2 (Full-State Journey Assertions)**: Verify each of the 8 E2E journeys asserts the complete observable user outcome (editor text, block/char formatting, HUD string, dirty flag, and saved disk contents).
- [ ] Zero `|| true` tautologies; all 8 end-to-end journeys pass 100%."""
    },
    {
        'id': '3af21322-41ad-4e73-83d3-6fcde411bfdb',
        'title': '[FELT-LEX-1] Build Pure Markdown Grammar Lexer & Invariant Rules',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Extract pure markdown matching rules into `grammar.h` / `grammar.cpp` (< 90 lines, zero Qt GUI includes, accepts `QStringView` for zero-allocation lexing).

## Zinsser Core Spec (6 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Zero-Allocation `QStringView` Linear Scan)**: Read `grammar.h/cpp` to verify block lexing scans `QStringView` directly in $O(N)$ linear time—zero `QRegularExpression` instantiations per call and zero heap-allocated `QString` substring copies.
- [ ] **LLM Semantic Check 2 (Pure Domain Isolation)**: Verify `grammar.h/cpp` has zero GUI/widget dependencies and returns pure value structs (`BlockToken`, `InlineSpan`).
- [ ] 100.0% branch coverage on `grammar.cpp` via `xcrun llvm-cov`; zero `else` keywords, <= 90 lines."""
    },
    {
        'id': 'b5eeb9a3-483f-436f-a257-b45412351369',
        'title': '[FELT-LEX-2] Author Compiler-Grade Inline Delimiter Conformance Suite',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Implement `tests/test_grammar_conformance.cpp` testing all inline symmetric lookaround rules, backslash-escaped delimiters (`\\*`), and delimiter collisions with compiler-grade rigor.

## Zinsser Core Spec (8 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Exact Span Offset & Length Assertions)**: Read `tests/test_grammar_conformance.cpp` to verify every `it()` block asserts the **exact start index, length, and token type** of both the opening/closing delimiter (`*`, `**`) and inner content—never a weak check like `!spans.isEmpty()`.
- [ ] **LLM Semantic Check 2 (No Quadratic Backtracking on Unclosed/Escaped Asterisks)**: Verify in `grammar.cpp` that scanning a line with multiple unclosed `*` or escaped `\\*` characters runs in a single linear pass without nested backtracking loops.
- [ ] 100.0% branch coverage on inline delimiter scanning."""
    },
    {
        'id': 'fde5d68e-ab7d-4b12-a729-c5625dc005fe',
        'title': '[FELT-CORE-1] Implement Immutable ThemePalette & ThemeRegistry',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `theme.h/cpp` and `themeregistry.h/cpp` (<= 100 lines each, zero `else`, immutable value objects) for *Obsidian Void*, *Warm Parchment*, *Slate Concrete*, and *Midnight Indigo*.
- **Single Source of Truth & WCAG Math**:
  - `ThemeRegistry` is the sole authority for hex color literals in the codebase. All tests outside `ThemeRegistry` read colors from the registry.
  - Set *Obsidian Void* `textDimmed` to `#4C525B` (**`2.43:1`** against `#0D0F14` vignette center, providing comfortable headroom above the `2.0:1` floor).
  - Measure all WCAG contrast ratios against `bgVignetteCenter`.

## Zinsser Core Spec (5 `it()` Scenarios)
```cpp
describe("ThemeRegistry and WCAG Contrast Invariants", [] {
    it("resolves obsidian, parchment, slate, and midnight palettes by key");
    it("falls back to obsidian void when queried with an unknown key");
    it("enforces wcag aaa contrast ratio above seven to one between body text and vignette center");
    it("enforces dimmed text contrast between two and four to one against vignette center for focus mode");
    it("enforces high-visibility contrast for active caret and text selection across all palettes");
});
```

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Exact WCAG 2.1 Relative Luminance Formula)**: Read `theme.cpp` to verify the contrast calculation implements the true WCAG 2.1 piecewise sRGB transfer function (`0.04045` linear threshold and `2.4` gamma exponent) measured against `bgVignetteCenter`, rather than a simplified perceived-brightness shortcut.
- [ ] **LLM Semantic Check 2 (True Value-Object Immutability & `#4C525B` Headroom)**: Verify `ThemePalette` cannot be mutated after construction and that Obsidian Void `textDimmed` (`#4C525B`) provides `2.43:1` contrast headroom against `#0D0F14`.
- [ ] 100.0% branch coverage on `theme.cpp` and `themeregistry.cpp` via `xcrun llvm-cov`; zero `else` keywords, <= 100 lines per file."""
    },
    {
        'id': '06c049ae-f813-4846-8c46-5cd26db30b2a',
        'title': '[FELT-CORE-2] Decouple Document Measure Calculus into Tiny Value Object',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `measure.h/cpp` (<= 60 lines, zero `else`, 1 level of indentation, zero Qt widget coupling) computing centered horizontal margins and vertical breathing space.

## Zinsser Core Spec (5 `it()` Scenarios)
```cpp
describe("Document Measure and Viewport Margin Calculus", [] {
    it("centers the writing column symmetrically when window width exceeds measure");
    it("clamps side margins to forty pixels when window is narrower than measure");
    it("clamps configured column width within five hundred twenty and nine hundred sixty pixels");
    it("preserves eighty pixel top and bottom breathing margins across all window sizes");
    it("prevents negative viewport geometry when window width is smaller than minimum margins");
});
```

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Tell, Don't Ask Delegation)**: Read `measure.h/cpp` and `editor.cpp` to verify `FeltEditor::updateMargins()` delegates 100% of margin and column-width clamping math to `DocumentMeasure` (zero raw geometry arithmetic left inside `editor.cpp`).
- [ ] 100.0% branch coverage on `measure.cpp` via `xcrun llvm-cov`; zero GUI includes, zero `else` keywords."""
    },
    {
        'id': '3847e032-6622-48a2-b793-a8dccabe2ee3',
        'title': '[FELT-CORE-3] Extract DocumentHUD and MenuBuilder & Eliminate All Else Branches',
        'desc': """## Outcome Contract & Deliverables
- **Code**:
  - Split `mainwindow.cpp` (279 lines -> < 130 lines) by extracting `documenthud.h/cpp` (<= 80 lines) and `menubuilder.h/cpp` (<= 100 lines).
  - Replace the 11-line `if / else if / else` font ladder in `editor.cpp` with a 4-line priority-list loop (`{"Menlo", "Monaco", "SF Mono", "Courier New"}`) and early return.
  - Eliminate all remaining `else` branches in `mainwindow.cpp` using guard clauses.
- **Explicit v1 Scope Boundary**: `DocumentStats` uses fast deterministic whitespace tokenization (`<= 80` lines, zero ICU dependencies), verified for multibyte UTF-8 safety across non-Latin scripts.

## Zinsser Core Spec (2 Cohesive `describe()` Blocks, 5 and 4 `it()` Scenarios)
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

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Genuine Guard Clauses vs. Disguised `else`)**: Read `editor.cpp` and `mainwindow.cpp` to verify that removing the 10 `else` branches used genuine early returns and data-driven iteration (the font priority list), not nested ternaries (`a ? (b ? c : d) : e`) or inverted `if (!cond)` blocks following a non-returning `if (cond)`.
- [ ] **LLM Semantic Check 2 (Cohesive Single-Responsibility Extraction)**: Verify `DocumentHUD` and `MenuBuilder` are clean, cohesive components with <= 2 instance variables, not arbitrary slices of `MainWindow` created just to pass the 200-line limit.
- [ ] Every `.h` and `.cpp` file in the repository is strictly <= 200 lines; zero `else` keywords exist anywhere in the codebase."""
    },
    {
        'id': 'd8ba3870-6df4-4c42-abab-f025bfa095bc',
        'title': '[FELT-GATE-1] Implement Automated Calisthenics, Zinsser & Auditor Verification Gate',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Implement `scripts/verify_conformance.py` and wire to `task gate`:
  1. Object Calisthenics Rule 7: Every `.h`/`.cpp` file <= 200 lines.
  2. Object Calisthenics Rule 2: Zero `else` keywords across all C++ files.
  3. William Zinsser Mandate: Zero bureaucratic filler words (`should`, `properly`, `correctly`, `successfully`, `works as expected`) and zero tautologies (`|| true`).
  4. Multi-Scenario & Auditor Rule: Every `describe()` block contains >= 4 unique `it()` blocks, and every `it()` block tests a single distinct failure mode that would exist even without a minimum count rule (zero artificial padding).

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check (Mandatory Pre-Done Audit)**: Verify that no story is marked `Done` until both the automated `task gate` script exits `0` **and** an LLM Auditor has verified the story-specific semantic checks on the git diff."""
    },
    {
        'id': 'b7723a4d-0320-4fe5-935d-5ae4694cddad',
        'title': '[FELT-TEST-1] Build Zinsser BDD Harness (zinsser.h), 100% LLVM Branch Coverage & Legacy Test Refactor',
        'desc': """## Outcome Contract & Deliverables
- **Code**:
  - Build `tests/zinsser.h` (<= 90 lines) providing `describe()` and `it()` with runtime checks against bureaucratic fillers.
  - Enable `-fprofile-instr-generate -fcoverage-mapping` in `CMakeLists.txt` and enforce 100% branch coverage on pure domain modules (`stats`, `grammar`, `theme`, `themeregistry`, `measure`, `preferences`).
  - Refactor existing `tests/test_felt.cpp` to eliminate `|| true` (using real offscreen window activation) and replace hardcoded `#F0F0F0` / `#383838` strings with `ThemeRegistry::obsidianVoid()` references.

## The Brake Check (Context-Specific LLM Auditor Verification Gate)
- [ ] **LLM Semantic Check 1 (Assertion Strength & Honesty Audit Across All 63 `it()` Blocks)**: Read every `it()` block body across the test suite and verify that:
  1. Every `it()` body asserts the exact behavioral claim in its title (`QCOMPARE` on exact strings, coordinates, and `ThemePalette` colors—zero weak truthy checks).
  2. Every `it()` tests a distinct failure mode that would deserve to exist even without the `>= 4` rule (zero padding).
  3. All legacy tests in `tests/test_felt.cpp` use `ThemeRegistry` colors and real offscreen focus activation (`show()`, `activateWindow()`, `qWaitForWindowActive()`)."""
    }
]

for u in updates:
    cmd = [
        'python3', cli, '--workspace', workspace, 'update-issue',
        '--project', project,
        '--issue', u['id'],
        '--title', u['title'],
        '--description', u['desc']
    ]
    res = subprocess.run(cmd, capture_output=True, text=True)
    print(res.stdout.strip())
