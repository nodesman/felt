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
  - Pressing `Enter` on a list line (`- item` or `* item`) automatically scaffolds the next bullet (`- ` or `* `) preserving indentation.
  - Pressing `Enter` on an empty bullet (`- `) clears the bullet and restores a standard paragraph line.
  - Pressing `Tab` indents by 4 spaces without moving focus.

## Zinsser Functional Spec (>= 4 `it()` per `describe`)
```cpp
describe("Smart In-Situ List Editing", [] {
    it("continues unordered hyphen and asterisk bullets on enter");
    it("preserves leading indentation when continuing nested list items");
    it("clears empty bullet marker and restores plain paragraph on second enter");
    it("indents active list item by four spaces when tab is pressed");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Zero default grey OS title bar band; traffic lights flush inside top canvas margin.
- [ ] All 4 `it()` scenarios in `describe("Smart In-Situ List Editing")` pass via `FeltAppDriver` keystroke simulation.
- [ ] Zero `else` keywords, <= 200 lines per file."""
    },
    {
        'id': '410d443e-2506-4628-b732-382eb3da52fe',
        'title': '[FELT-UI-2] Build Sovereign Multi-Theme Engine & Ambient Tactile Sheen',
        'desc': """## Outcome Contract & Deliverables
- **Ambient Tactile Sheen**:
  - Replace flat `#080808` fill with a radial vignette (`#0D0F14` center tapering to `#060709` edges) eliminating the raw `<textarea>` look.
  - Enforce 160% proportional line height (`QTextBlockFormat::ProportionalHeight`) across all text blocks.
  - Apply Optical Bone White (`#ECEBE8`) body typography and Slate Smoke (`#3E4249`) focus dimming.
- **Live Theme Application**:
  - Dynamically apply any of the 4 `ThemePalette` configurations (*Obsidian Void*, *Warm Parchment*, *Slate Concrete*, *Midnight Indigo*) to `FeltEditor`, `FeltHighlighter`, caret, and `DocumentHUD` without widget reconstruction.

## Zinsser Functional Spec (>= 4 `it()` per `describe`)
```cpp
describe("Canvas Tactile Aesthetics and Live Theming", [] {
    it("enforces one hundred sixty percent proportional line height across all blocks");
    it("illuminates active block in optical bone white while dimming inactive blocks to slate smoke");
    it("recolors canvas vignette, text, caret, and hud live when switching palettes");
    it("preserves cursor position and undo history across live theme transitions");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Radial vignette and 160% proportional line spacing verified in live window and unit assertions.
- [ ] All 4 `it()` blocks in `describe("Canvas Tactile Aesthetics and Live Theming")` pass."""
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

## Zinsser Functional Spec (>= 4 `it()` per `describe`)
```cpp
describe("Minimalist Preferences Sheet (Cmd+,)", [] {
    it("opens centered over the canvas when triggered by shortcut or menu");
    it("emits selected palette key immediately when a theme swatch is clicked");
    it("emits clamped font size and column measure as sliders move");
    it("toggles focus mode, typewriter scrolling, and hud visibility switches");
    it("dismisses cleanly when escape key is pressed");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] File length <= 180 lines, zero `else` keywords.
- [ ] All 5 `it()` blocks pass under `FeltAppDriver` interaction.
- [ ] Visual layout matches `_meta/mockups/preferences_settings_modal.jpg`."""
    },
    {
        'id': '347ebe23-181e-4cb9-9000-78fd7e921b7d',
        'title': '[FELT-UI-4] Connect Live Canvas Reactive Sync & QSettings Persistence',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `preferences.h` / `preferences.cpp` (<= 110 lines, zero `else`, 100% branch-covered) managing configuration state and `QSettings` (`com.nodesman.felt`) serialization.
- **Live Sync**: Wire `SettingsSheet` signals to `MainWindow` and `FeltEditor` so theme, font family, font size, column width, caret width, and HUD visibility update instantaneously and survive restarts.

## Zinsser Functional Spec (>= 4 `it()` per `describe`)
```cpp
describe("Preferences State Machine and Storage Persistence", [] {
    it("roundtrips theme, typography, measure, and dynamics settings through storage");
    it("clamps out of range font sizes to thirteen through twenty six points");
    it("clamps out of range column widths to five hundred twenty through nine hundred sixty pixels");
    it("restores default obsidian configuration when storage keys are missing or corrupted");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `preferences.cpp` via `xcrun llvm-cov`.
- [ ] All 4 `it()` blocks pass."""
    },
    {
        'id': 'ad498b62-2f6d-4693-8465-82ff26a802aa',
        'title': '[FELT-UI-5] Codify FeltAppDriver End-to-End Functional User Journeys',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `tests/felt_app_driver.h` (<= 90 lines) wrapping `MainWindow`, `FeltEditor`, `SettingsSheet`, and `DocumentHUD` with high-level user actions (`type`, `pressEnter`, `pressShortcut`, `clickThemeSwatch`, `dragMeasureSlider`, `clickMargin`).
- **Why**: Gives Cucumber-level functional readability inside C++ `describe()` / `it()` blocks with zero `.feature` regex glue, testing the real integrated widget tree and event loop.

## Zinsser End-to-End Journey Spec (>= 4 `it()` per `describe`)
```cpp
describe("End-to-End User Journey: Authoring and Customizing a Manuscript", [] {
    it("formats headings, quotes, and emphasis in situ while updating reading stats");
    it("scaffolds and terminates multi-item markdown lists from live keystrokes");
    it("isolates the active paragraph in focus mode and centers the cursor in typewriter mode");
    it("live-switches themes, font sizes, and column measures from the preferences sheet");
    it("handles mouse clicks across viewport and side margins without event recursion");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Drives the real `MainWindow` widget hierarchy with real Qt mouse and keyboard events.
- [ ] Zero `|| true` tautologies; all 5 end-to-end journeys pass 100%."""
    },
    {
        'id': '3af21322-41ad-4e73-83d3-6fcde411bfdb',
        'title': '[FELT-LEX-1] Build Pure Markdown Grammar Lexer & Invariant Rules',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Extract pure markdown matching rules into `grammar.h` / `grammar.cpp` (< 90 lines, zero Qt GUI includes, accepts `QStringView` for zero-allocation lexing).
- **Rules**:
  - Headings (`#` to `######` followed by space): returns prefix span (receding `#4B5563`) and content span (bold bone-white, scaled).
  - Unordered lists (`- ` or `* ` with optional leading indentation): returns jewel-cyan bullet span (`#3B82F6`) and item span.
  - Blockquotes (`> `), thematic dividers (`---`, `***`), and symmetric inline emphasis (`**bold**`, `*italic*`).

## Zinsser Core Spec (>= 4 `it()` per `describe`)
```cpp
describe("Markdown Block Grammar Lexer", [] {
    it("parses heading levels one through six into receding prefix and title spans");
    it("rejects hash markers lacking a trailing space as plain paragraphs");
    it("identifies unordered hyphen and asterisk list bullets with leading indentation");
    it("parses blockquotes and three-character horizontal scene breaks");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `grammar.cpp` via `xcrun llvm-cov`.
- [ ] Zero GUI includes, zero `else` keywords, <= 90 lines."""
    },
    {
        'id': 'b5eeb9a3-483f-436f-a257-b45412351369',
        'title': '[FELT-LEX-2] Author Compiler-Grade Inline Delimiter Conformance Suite',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Implement `tests/test_grammar_conformance.cpp` testing all inline symmetric lookaround rules and boundary conditions with compiler-grade rigor.

## Zinsser Core Spec (>= 4 `it()` per `describe`)
```cpp
describe("Markdown Inline Emphasis Delimiter Lexer", [] {
    it("matches balanced double-asterisk bold spans within a paragraph");
    it("matches balanced single-asterisk italic spans without colliding with bold");
    it("traps and ignores unclosed or mismatched asterisk delimiters");
    it("preserves raw markdown characters byte-for-byte without html mutation");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] All 4 `it()` blocks pass with concrete span offset assertions.
- [ ] 100.0% branch coverage on inline delimiter scanning."""
    },
    {
        'id': 'fde5d68e-ab7d-4b12-a729-c5625dc005fe',
        'title': '[FELT-CORE-1] Implement Immutable ThemePalette & ThemeRegistry',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `theme.h/cpp` and `themeregistry.h/cpp` (<= 100 lines each, zero `else`, immutable value objects) for *Obsidian Void*, *Warm Parchment*, *Slate Concrete*, and *Midnight Indigo*.

## Zinsser Core Spec (>= 4 `it()` per `describe`)
```cpp
describe("Sovereign Theme Registry and Contrast Invariants", [] {
    it("resolves obsidian, parchment, slate, and midnight palettes by key");
    it("falls back to obsidian void when queried with an unknown key");
    it("enforces wcag aaa contrast ratio above seven to one between body text and background");
    it("maintains distinct dimmed text contrast between two and four to one for focus mode");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `theme.cpp` and `themeregistry.cpp` via `xcrun llvm-cov`.
- [ ] Zero `else` keywords, <= 100 lines per file."""
    },
    {
        'id': '06c049ae-f813-4846-8c46-5cd26db30b2a',
        'title': '[FELT-CORE-2] Decouple Document Measure Calculus into Tiny Value Object',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `measure.h/cpp` (<= 60 lines, zero `else`, 1 level of indentation, zero Qt widget coupling) computing centered horizontal margins and vertical breathing space.

## Zinsser Core Spec (>= 4 `it()` per `describe`)
```cpp
describe("Document Measure and Viewport Margin Calculus", [] {
    it("centers the writing column symmetrically when window width exceeds measure");
    it("clamps side margins to forty pixels when window is narrower than measure");
    it("clamps configured column width within five hundred twenty and nine hundred sixty pixels");
    it("preserves eighty pixel top and bottom breathing margins across all window sizes");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `measure.cpp` via `xcrun llvm-cov`.
- [ ] Zero GUI includes, zero `else` keywords."""
    },
    {
        'id': '3847e032-6622-48a2-b793-a8dccabe2ee3',
        'title': '[FELT-CORE-3] Extract DocumentHUD and MenuBuilder & Eliminate All Else Branches',
        'desc': """## Outcome Contract & Deliverables
- **Code**:
  - Split `mainwindow.cpp` (279 lines -> < 130 lines) by extracting `documenthud.h/cpp` (<= 80 lines) and `menubuilder.h/cpp` (<= 100 lines).
  - Replace the 11-line `if / else if / else` font ladder in `editor.cpp` with a 4-line priority-list loop (`{"Menlo", "Monaco", "SF Mono", "Courier New"}`) and early return.
  - Eliminate all remaining `else` branches in `mainwindow.cpp` using guard clauses.

## Zinsser Core Spec (>= 4 `it()` per `describe`)
```cpp
describe("Document Statistics and Whisper HUD Formatting", [] {
    it("computes word, character, and non-space counts across multi-paragraph prose");
    it("rounds reading time at two hundred words per minute with one minute minimum");
    it("formats summary string with middle-dot separator for the bottom hud");
    it("resolves preferred monospace font family from ordered priority list without else branches");
});
```

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Every `.h` and `.cpp` file in the repository is strictly <= 200 lines.
- [ ] Zero `else` keywords exist anywhere in the codebase."""
    },
    {
        'id': 'd8ba3870-6df4-4c42-abab-f025bfa095bc',
        'title': '[FELT-GATE-1] Automated Calisthenics, Multi-Scenario Describe & Coverage Gate Script',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Upgrade `scripts/verify_conformance.py` and attach to `task gate`.
- **Enforced Invariants**:
  1. **Object Calisthenics**: Every file <= 200 lines, zero `else` keywords, max 1 indentation level.
  2. **Multi-Scenario `describe()` Rule**: Every `describe()` block in `tests/` must contain **at least 4 `it()` blocks** (no 1-item `describe()` clutter).
  3. **Zinsser & Anti-Tautology Rule**: Zero bureaucratic words (`should`, `properly`, `correctly`), zero tautologies (`|| true`, `QVERIFY(true)`), >= 1 concrete assertion per `it()`.
  4. **100% Core Branch Coverage**: Runs `xcrun llvm-profdata` + `xcrun llvm-cov` and asserts 100.0% branch coverage on all domain core modules.

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Fails build with non-zero exit code if any `describe()` has < 4 `it()` blocks, any file > 200 lines, any `else` keyword, or core branch coverage < 100%."""
    },
    {
        'id': 'b7723a4d-0320-4fe5-935d-5ae4694cddad',
        'title': '[FELT-TEST-1] Implement Zinsser BDD Harness (describe/it) & 100% LLVM Branch Coverage Cage',
        'desc': """## Outcome Contract & Deliverables
1. **Header-Only C++17 Zinsser BDD Harness (`tests/zinsser.h`, <= 90 lines)**:
   - Provides `describe("Feature", [] { ... })`, `it("active verb...", [] { ... })`, and `expect(a).toBe(b)`.
   - Prints clean hierarchical spec output and tracks assertion counts per `it()` block (failing any `it()` block that executes 0 assertions).
   - Reusable across all C++17 / Qt 6 Foundry projects (`Ark`, `Folio`, `Markd`, `WorkflowyQt`, `Howitzer`).
2. **CMake Apple Clang Coverage Integration**:
   - Add `-DENABLE_COVERAGE=ON` (`-fprofile-instr-generate -fcoverage-mapping`) to `CMakeLists.txt`.
   - Fix line 162 (`|| true`) in `tests/test_felt.cpp`.

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `tests/zinsser.h` is <= 90 lines, zero external dependencies, zero `else` keywords.
- [ ] `task gate` runs `zinsser.h` suites, verifies >= 4 `it()` per `describe()`, and confirms 100% core branch coverage via `xcrun llvm-cov`."""
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
