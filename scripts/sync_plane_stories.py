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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Zero default grey OS title bar band; traffic lights flush inside top canvas margin (human visual/screenshot check).
- [ ] All 8 `it()` scenarios in `describe("FeltEditor list continuation and indentation")` pass via `FeltAppDriver` keystroke simulation.
- [ ] Zero `else` keywords, <= 200 lines per file."""
    },
    {
        'id': '410d443e-2506-4628-b732-382eb3da52fe',
        'title': '[FELT-UI-2] Build Multi-Theme Engine & Ambient Tactile Sheen',
        'desc': """## Outcome Contract & Deliverables
- **Ambient Tactile Sheen**:
  - Replace flat `#080808` fill with a radial vignette (`bgVignetteCenter` `#0D0F14` tapering to `bgOuter` `#08090C`) eliminating the raw `<textarea>` look.
  - Enforce 160% proportional line height (`QTextBlockFormat::ProportionalHeight`) across all initial, newly inserted, and pasted text blocks.
  - Apply `ThemeRegistry::obsidianVoid().textMain` (`#ECEBE8`) body typography and `ThemeRegistry::obsidianVoid().textDimmed` (`#464B53`, WCAG 2.15:1 against `#0D0F14`) focus dimming without flipping `document()->isModified()`.
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Radial vignette and 160% proportional line spacing verified in live window and unit assertions.
- [ ] All assertions reference `ThemeRegistry` palette properties (zero duplicate hardcoded hex literals).
- [ ] Toggling Focus Mode (`Cmd+D`) or switching themes on an unmodified document never marks `isModified()` true.
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] File length <= 180 lines, zero `else` keywords.
- [ ] All 7 `it()` blocks pass under `FeltAppDriver` interaction.
- [ ] Visual layout matches `_meta/mockups/preferences_settings_modal.jpg`."""
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `preferences.cpp` via `xcrun llvm-cov`.
- [ ] All 5 `it()` blocks pass."""
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Drives the real `MainWindow` widget hierarchy with real Qt mouse, keyboard, and file I/O events.
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `grammar.cpp` via `xcrun llvm-cov`.
- [ ] Zero GUI includes, zero `else` keywords, <= 90 lines."""
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] All 8 `it()` blocks pass with concrete span offset assertions.
- [ ] 100.0% branch coverage on inline delimiter scanning."""
    },
    {
        'id': 'fde5d68e-ab7d-4b12-a729-c5625dc005fe',
        'title': '[FELT-CORE-1] Implement Immutable ThemePalette & ThemeRegistry',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `theme.h/cpp` and `themeregistry.h/cpp` (<= 100 lines each, zero `else`, immutable value objects) for *Obsidian Void*, *Warm Parchment*, *Slate Concrete*, and *Midnight Indigo*.
- **Single Source of Truth & WCAG Math**:
  - `ThemeRegistry` is the sole authority for hex color literals in the codebase. All tests outside `ThemeRegistry` read colors from the registry.
  - Raise *Obsidian Void* `textDimmed` from `#3E4249` (1.897:1) to `#464B53` (2.15:1 against `#0D0F14` center, 2.29:1 against `#08090C` outer).
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100.0% branch coverage on `theme.cpp` and `themeregistry.cpp` via `xcrun llvm-cov`.
- [ ] Zero `else` keywords, <= 100 lines per file."""
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Every `.h` and `.cpp` file in the repository is strictly <= 200 lines.
- [ ] Zero `else` keywords exist anywhere in the codebase."""
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

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `task gate` exits non-zero on any file > 200 lines, any `else`, any tautology, or any `describe()` with < 4 unique `it()` blocks."""
    },
    {
        'id': 'b7723a4d-0320-4fe5-935d-5ae4694cddad',
        'title': '[FELT-TEST-1] Build Zinsser BDD Harness (zinsser.h), 100% LLVM Branch Coverage & Legacy Test Refactor',
        'desc': """## Outcome Contract & Deliverables
- **Code**:
  - Build `tests/zinsser.h` (<= 90 lines) providing `describe()` and `it()` with runtime checks against bureaucratic fillers.
  - Enable `-fprofile-instr-generate -fcoverage-mapping` in `CMakeLists.txt` and enforce 100% branch coverage on pure domain modules (`stats`, `grammar`, `theme`, `themeregistry`, `measure`, `preferences`).
  - Refactor existing `tests/test_felt.cpp` to eliminate `|| true` (using real offscreen window activation) and replace hardcoded `#F0F0F0` / `#383838` strings with `ThemeRegistry::obsidianVoid()` references.

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Zero `|| true` tautologies and zero duplicate hardcoded palette hex literals in `tests/test_felt.cpp`.
- [ ] 100.0% branch coverage on core domain modules via `xcrun llvm-cov`."""
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
