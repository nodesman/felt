# Felt UI Incremental Stories Roadmap

**Module / Epic**: Unified Frameless Window, Multi-Theme Engine & Minimal Preferences  
**Project**: Felt Sovereign Writer (`[FELT]` in `notionworks` Plane workspace)  
**Target Outcome**: Bridge the existing C++ / Qt 6 editor to the exact visual and behavioral fidelity established in the mockups and screen specifications.

---

## The 5 Incremental Stories

### Story 1: Native Unified Frameless macOS Window & Cross-Platform Chrome
- **Plane Identifier**: `[FELT-UI-1]`
- **Scope**:
  - Eliminate the default OS title bar on macOS via Cocoa window flags:
    - `NSWindowStyleMaskFullSizeContentView`
    - `titlebarAppearsTransparent = YES`
    - `titleVisibility = NSWindowTitleHidden`
  - Position native window traffic lights (close, minimize, zoom) gracefully at `(18, 18)` within the top margin.
  - Implement smooth window dragging via upper padding without blocking text selection.
  - Provide clean dark client-side decoration fallbacks for Linux and Windows.
- **The Brake Check (Quality Vectors)**:
  - [ ] No grey or disjointed title bar header band.
  - [ ] macOS traffic lights remain crisp, interactive, and properly positioned.
  - [ ] Dragging the window by the top margin works seamlessly.
  - [ ] Fullscreen toggle preserves full canvas measure.

---

### Story 2: Sovereign Multi-Theme Palette Engine
- **Plane Identifier**: `[FELT-UI-2]`
- **Scope**:
  - Implement `theme.h` and `theme.cpp` defining the 4 core palettes:
    1. **Obsidian Void**: `#080808` canvas, `#F0F0F0` text, `#383838` dimmed, `#3B82F6` electric cyan caret.
    2. **Warm Parchment**: `#FBF8F2` paper, `#2D2A26` sepia ink, `#C4BDB3` dimmed, `#D9532F` terracotta caret.
    3. **Slate Concrete**: `#1A1D20` matte charcoal, `#E6EDF3` frosty white, `#4A5157` dimmed, `#F97316` amber caret.
    4. **Midnight Indigo**: `#0A0E17` marine dark, `#E2E8F0` silver text, `#334155` dimmed, `#38BDF8` sky caret.
  - Wire palette switching to `FeltEditor`, `FeltHighlighter`, and bottom HUD.
  - Update `Focus Mode` dimming logic to derive from `theme.textDimmed` instead of hardcoded hex colors.
- **The Brake Check (Quality Vectors)**:
  - [ ] Switching themes instantly updates canvas background, selection, text, prefixes, and HUD.
  - [ ] Focus mode dimming respects the active palette in all 4 themes.
  - [ ] Electric caret switches color dynamically without widget reconstruction.

---

### Story 3: Minimalist iA Writer-Style Preferences Dialog (`Cmd+,`)
- **Plane Identifier**: `[FELT-UI-3]`
- **Scope**:
  - Implement `settingsdialog.h` and `settingsdialog.cpp` as a 520×480 non-resizable floating sheet.
  - Components:
    1. **Theme Swatches**: 4 circular/rounded tactile buttons with active ring indicator.
    2. **Typography Controls**: Typeface dropdown (Menlo, Monaco, SF Mono, JetBrains Mono, Courier New), font size slider (13pt–26pt), column measure slider (520px–960px).
    3. **Writing Dynamics**: Toggle switches for Focus Mode (`Cmd+D`), Typewriter Mode (`Cmd+T`), in-situ markdown, and statistics HUD.
    4. **Caret Personality**: Segmented selector for caret width (`1px`, `2px`, `3px`).
  - Style dialog cleanly using dark stylesheet synced with the active theme background.
- **The Brake Check (Quality Vectors)**:
  - [ ] Triggered via standard shortcut `Cmd+,` and app menu `Preferences...`.
  - [ ] Dismissible via `Esc` key or close button.
  - [ ] Visual fidelity matches [`preferences_settings_modal.jpg`](file:///Users/rajsekharan/projects/foundry/tech/felt/_meta/mockups/preferences_settings_modal.jpg).

---

### Story 4: Live Canvas Reactive Synchronization & Persistence
- **Plane Identifier**: `[FELT-UI-4]`
- **Scope**:
  - Connect `SettingsDialog` signals (`themeChanged`, `fontSizeChanged`, `columnWidthChanged`, `caretWidthChanged`) directly to `MainWindow` and `FeltEditor`.
  - Ensure column width slider dynamically triggers `updateMargins()` for instantaneous visual feedback.
  - Persist all configuration state via `QSettings` (`com.nodesman.felt`) on macOS plist / Linux config.
  - Load saved preferences cleanly on application startup.
- **The Brake Check (Quality Vectors)**:
  - [ ] Adjusting column width slider recalculates margins smoothly in real-time.
  - [ ] Changing font size updates typography and line height immediately.
  - [ ] Restarting `Felt.app` completely restores the user's chosen theme, font, and measure.

---

### Story 5: William Zinsser Test Suite & Quality Gate Verification
- **Plane Identifier**: `[FELT-UI-5]`
- **Scope**:
  - Extend `tests/test_felt.cpp` with muscular, clutter-free unit tests adhering to William Zinsser standards:
    - `switches_palettes_dynamically()`
    - `calculates_margins_for_custom_measures()`
    - `persists_and_restores_user_preferences()`
    - `clamps_column_width_within_bounds()`
    - `formats_all_four_themes_consistently()`
  - Verify 100% test pass rate via `ctest --test-dir build --output-on-failure`.
- **The Brake Check (Quality Vectors)**:
  - [ ] Zero bureaucratic filler words in test names (`should`, `properly`, `works as expected`).
  - [ ] 100% tests passing on automated runner.
  - [ ] Zero memory leaks or dangling QObject pointers.

---

## Epic 2: Compiler-Grade Lexical Grammar & In-Situ Conformance
- **Plane Module**: `Compiler-Grade Lexical Grammar & In-Situ Conformance`
- **Objective**: Create a deterministic, compiler-grade lexical test matrix verifying all markdown grammar productions, boundary conditions, and negative syntax cases.

### Story 6: Markdown Grammar Lexer & Invariant Rules
- **Plane Identifier**: `[FELT-LEX-1]`
- **Scope**:
  - Extract pure markdown matching rules from `highlighter.cpp` into `grammar.h` and `grammar.cpp` (< 90 lines).
  - Implement zero-allocation token scanning using `QStringView`.
- **The Brake Check (Quality Vectors)**:
  - [ ] Zero Qt GUI dependencies in `grammar.h/cpp` (pure string/token logic).
  - [ ] Rejects invalid markdown gracefully (e.g. `#nospace`, `**unclosed`).
  - [ ] File length strictly $\le 100$ lines.

### Story 7: Clutter-Free Grammar Conformance Test Suite
- **Plane Identifier**: `[FELT-LEX-2]`
- **Scope**:
  - Create `tests/test_grammar_conformance.cpp` testing H1–H6, blockquotes, symmetric delimiters, and scene breaks.
  - Use active-verb William Zinsser declarations.
- **The Brake Check (Quality Vectors)**:
  - [ ] Active verb test cases: `rejects_unspaced_heading_marker()`, `parses_nested_blockquote()`, `traps_unbalanced_asterisks()`.
  - [ ] 100% test pass rate via `ctest`.

---

## Epic 3: Object Calisthenics Core & Architectural Conformance
- **Plane Module**: `Object Calisthenics Core & Architectural Conformance`
- **Objective**: Refactor all Felt components into small, single-responsibility entities $\le 200$ lines, 1 level of indentation, zero `else` keywords, and tiny value objects.

### Story 8: Immutable `ThemePalette` & `ThemeRegistry` Value Objects
- **Plane Identifier**: `[FELT-CORE-1]`
- **Scope**:
  - Create `theme.h/cpp` and `themeregistry.h/cpp` ($\le 100$ lines, zero `else`, value objects for colors).
  - Enforce WCAG AAA contrast ratio ($\ge 7:1$) on all 4 palettes.
- **The Brake Check (Quality Vectors)**:
  - [ ] Value objects immutable after construction.
  - [ ] Automated test asserts contrast ratio $\ge 7:1$ for all 4 themes.

### Story 9: Decouple Document Measure Calculus into Tiny Value Object
- **Plane Identifier**: `[FELT-CORE-2]`
- **Scope**:
  - Create `measure.h/cpp` calculating dynamic margins, line widths, and clamping.
  - Implement with 1 level of indentation and zero `else`.
- **The Brake Check (Quality Vectors)**:
  - [ ] Pure mathematical value object with zero Qt widget coupling.
  - [ ] 100% line & branch coverage via unit tests.

### Story 10: Extract `DocumentHUD` and `MenuBuilder` from MainWindow
- **Plane Identifier**: `[FELT-CORE-3]`
- **Scope**:
  - Shrink `mainwindow.cpp` from 279 lines down to $< 140$ lines.
  - Delegate HUD to `documenthud.h/cpp` and menu bar to `menubuilder.h/cpp`.
- **The Brake Check (Quality Vectors)**:
  - [ ] Every single C++ file in the repository strictly $\le 200$ lines.
  - [ ] Clean separation of concerns with zero logic duplication.

---

## Epic 4: The Sovereign Verification Gate (100% Conformance)
- **Plane Module**: `The Sovereign Verification Gate (100% Conformance)`
- **Objective**: Automated gate script auditing code metrics, Object Calisthenics, and test pass rate before declaring any milestone complete.

### Story 11: Automated Calisthenics & Verification Gate Script
- **Plane Identifier**: `[FELT-GATE-1]`
- **Scope**:
  - Implement `scripts/verify_conformance.py` verifying:
    1. Line counts ($\le 200$ lines per file).
    2. Zero `else` keywords.
    3. Indentation depth ($\le 1$ level per method).
    4. 100% CTest pass rate.
  - Wire to `task gate` in `Taskfile.yml`.
- **The Brake Check (Quality Vectors)**:
  - [ ] Fails build if any line exceeds 200 lines or contains an `else` keyword.
  - [ ] Returns exit code 0 only when all invariants and tests pass.

### Story 12: Zinsser BDD Harness (`describe`/`it`), 100% Branch Coverage & Object Calisthenics Cage
- **Plane Identifier**: `[FELT-TEST-1]` (Issue #17)
- **Scope**:
  - Build header-only C++17 Zinsser DSL (`tests/zinsser.h`, $< 100$ lines) providing `describe()` / `it()` BDD blocks with runtime enforcement of William Zinsser *On Writing Well* active-verb standards.
  - Configure `-DENABLE_COVERAGE=ON` in CMake with Apple Clang `-fprofile-instr-generate -fcoverage-mapping`.
  - Wire `xcrun llvm-cov` and `xcrun llvm-profdata` into `task coverage` enforcing 100% branch and line coverage on core modules.
- **The Brake Check (Quality Vectors)**:
  - [ ] `task test` displays hierarchical, clutter-free Zinsser prose output.
  - [ ] `task coverage` mathematically proves 100% branch coverage across all domain modules.
  - [ ] `task gate` returns exit code 0 only when Calisthenics, Zinsser prose, and 100% branch coverage pass simultaneously.

