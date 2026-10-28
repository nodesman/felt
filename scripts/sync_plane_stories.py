#!/usr/bin/env python3
import subprocess

cli = '/Users/rajsekharan/projects/foundry/kernel/skills/plane-ops/scripts/plane_client.py'
workspace = 'notionworks'
project = '704df24c-5db5-4578-86e2-67448b83ab78'

updates = [
    {
        'id': 'b7723a4d-0320-4fe5-935d-5ae4694cddad',
        'title': '[FELT-TEST-1] Implement Zinsser BDD Harness (describe/it), 100% Branch Coverage & Object Calisthenics Cage',
        'desc': """## Outcome Contract & Deliverables
1. **Zinsser BDD C++17 Harness (`tests/zinsser.h`, <= 90 lines)**:
   - Zero-dependency header-only C++17 DSL providing nested `describe("...", [] { ... })`, `it("...", [] { ... })`, and `expect(actual).toBe(expected)` blocks with hierarchical console output.
   - Enforces William Zinsser *On Writing Well* doctrine: rejects bureaucratic fillers (`should`, `properly`, `correctly`, `successfully`, `works as expected`) and requires active present-tense verbs.
   - Requires every `it()` block to execute at least one concrete value assertion (`expect(...)`) and bans tautologies (`|| true`, `QVERIFY(true)`).
   - Fixes line 162 in `tests/test_felt.cpp` by replacing `|| true` with proper widget activation and concrete state assertions.
2. **100% LLVM Branch Coverage on Pure Domain Core**:
   - Add `-DENABLE_COVERAGE=ON` in `CMakeLists.txt` using Apple Clang `-fprofile-instr-generate -fcoverage-mapping`.
   - Enforce **100% branch and line coverage** via `xcrun llvm-profdata` + `xcrun llvm-cov` on all pure domain modules (`stats`, `grammar`, `theme`, `measure`, `preferences`).
   - Keep platform OS glue (`windowchrome.mm`) as a thin (< 60 line) adapter exempt from branch coverage so tests never mock OS window managers.
3. **Reusable Foundry Asset**:
   - Design `tests/zinsser.h` so it can be dropped directly into other C++17 / Qt 6 Foundry projects (`Ark`, `Folio`, `Markd`, `WorkflowyQt`, `Howitzer`).

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `tests/zinsser.h` is <= 90 lines with zero `else` keywords.
- [ ] Every `it()` block executes >= 1 concrete assertion; zero `|| true` tautologies exist in `tests/`.
- [ ] `xcrun llvm-cov report` confirms 100.0% branch coverage on all domain core modules.
- [ ] `task gate` returns exit code 0."""
    },
    {
        'id': 'd8ba3870-6df4-4c42-abab-f025bfa095bc',
        'title': '[FELT-GATE-1] Automated Calisthenics & Verification Gate Script',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Upgrade `scripts/verify_conformance.py` and wire to `task gate` in `Taskfile.yml`.

## Automated Checks Enforced by `verify_conformance.py`
1. **Object Calisthenics Gate**:
   - Rule 7: Every `.h` and `.cpp` file strictly <= 200 lines.
   - Rule 2: Zero `else` keywords across all C++ headers and sources (enforcing data-driven lookup tables and guard clauses).
   - Rule 1: Max 1 level of indentation per method.
2. **Anti-Tautology & Zinsser Prose Gate**:
   - Zero bureaucratic filler words (`should`, `properly`, `correctly`, `successfully`, `works as expected`) in test titles.
   - Bans tautological test assertions (`|| true`, `&& true`, `QVERIFY(true)`, empty `it()` blocks without `expect` or `QCOMPARE`).
3. **100% Domain Branch Coverage Gate**:
   - Runs `xcrun llvm-profdata merge` and `xcrun llvm-cov export` to verify 100% branch and line coverage on `stats`, `grammar`, `theme`, `measure`, and `preferences`.

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Fails immediately with non-zero exit code on any file > 200 lines, any `else` token, any `|| true` tautology, or < 100% core branch coverage.
- [ ] Returns exit code 0 when all checks pass."""
    },
    {
        'id': 'fde5d68e-ab7d-4b12-a729-c5625dc005fe',
        'title': '[FELT-CORE-1] Implement Immutable ThemePalette & ThemeRegistry',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `theme.h` / `theme.cpp` and `themeregistry.h` / `themeregistry.cpp` (<= 100 lines each, zero `else`, immutable value objects).
- **4 Core Palettes**:
  1. `ObsidianVoid`: `#0D0F14` -> `#060709` radial vignette, `#ECEBE8` optical bone white text, `#3E4249` slate smoke dimmed, `#38BDF8` electric cyan caret.
  2. `WarmParchment`: `#FBF8F2` cream paper, `#2D2A26` espresso sepia ink, `#C4BDB3` dimmed, `#D9532F` terracotta caret.
  3. `SlateConcrete`: `#1A1D20` matte charcoal, `#E6EDF3` frosty white, `#4A5157` dimmed, `#F97316` amber caret.
  4. `MidnightIndigo`: `#0A0E17` deep marine, `#E2E8F0` silver text, `#334155` dimmed, `#38BDF8` sky caret.
- **Data-Driven Lookup**: Replace procedural `if/else` chains with first-class registry lookup.

## Zinsser `describe()` / `it()` Test Suite
- `describe("ThemeRegistry")`:
  - `it("resolves all four sovereign palettes by identifier")`
  - `it("falls back to obsidian void for unknown theme keys")`
  - `it("enforces wcag aaa contrast ratio above seven to one on all palettes")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100% branch coverage on `theme.cpp` and `themeregistry.cpp` via `llvm-cov`.
- [ ] Zero `else` keywords; <= 100 lines per file."""
    },
    {
        'id': '06c049ae-f813-4846-8c46-5cd26db30b2a',
        'title': '[FELT-CORE-2] Decouple Document Measure Calculus into Tiny Value Object',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `measure.h` / `measure.cpp` (<= 60 lines, zero `else`, 1 level of indentation, zero Qt widget dependencies).
- **Behavior**:
  - Computes horizontal viewport margins `std::max(40, (windowWidth - columnWidth) / 2)`.
  - Clamps configurable column width strictly within `[520, 960]` px (default `720` px).
  - Computes vertical breathing margins (`80` px top and bottom).

## Zinsser `describe()` / `it()` Test Suite
- `describe("DocumentMeasure")`:
  - `it("centers column horizontally when window exceeds measure")`
  - `it("clamps side margins to forty pixels on narrow windows")`
  - `it("clamps configured column width to valid bounds")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100% branch and line coverage on `measure.cpp` via `llvm-cov`.
- [ ] Zero GUI includes; zero `else` keywords."""
    },
    {
        'id': '3847e032-6622-48a2-b793-a8dccabe2ee3',
        'title': '[FELT-CORE-3] Extract DocumentHUD and MenuBuilder from MainWindow',
        'desc': """## Outcome Contract & Deliverables
- **Code**:
  - Decompose `mainwindow.cpp` (currently 279 lines) into:
    1. `mainwindow.h/cpp` (< 130 lines): Lean coordinator wiring editor, HUD, menus, and settings.
    2. `documenthud.h/cpp` (<= 80 lines): Whisper-quiet bottom statistics bar widget.
    3. `menubuilder.h/cpp` (<= 100 lines): Declarative menu bar and shortcut construction.
  - Refactor the `if / else if / else` font fallback ladder in `editor.cpp` (lines 58-68) into a 4-line data-driven loop over `{"Menlo", "Monaco", "SF Mono", "Courier New"}` with early return.
  - Eliminate all remaining `else` keywords in `mainwindow.cpp` using guard clauses.

## Zinsser `describe()` / `it()` Test Suite
- `describe("DocumentHUD")`:
  - `it("formats live word count and reading time into status label")`
  - `it("toggles visibility cleanly from preferences")`
- `describe("EditorTypography")`:
  - `it("selects first available monospace family from priority list")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `python3 scripts/verify_conformance.py` reports 0 file-length violations and 0 `else` violations across the entire repository."""
    },
    {
        'id': 'a01e7c3f-7015-4643-af14-7db578dae616',
        'title': '[FELT-UI-3] Create Minimalist Preferences & Settings Dialog (Cmd+,)',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `settingssheet.h` / `settingssheet.cpp` (<= 180 lines, zero `else`) implementing the 520x480 iA Writer-style floating preferences sheet triggered by `Cmd+,`.
- **Sections**:
  1. **Appearance & Themes**: 4 tactile swatch buttons (Obsidian, Parchment, Slate, Midnight).
  2. **Typography & Measure**: Typeface dropdown, Font Size slider (`13-26pt`), Column Width slider (`520-960px`).
  3. **Writing Dynamics**: Toggles for Focus Mode (`Cmd+D`), Typewriter Scrolling (`Cmd+T`), In-Situ Markdown, and Bottom HUD.
  4. **Caret Personality**: Segmented control for Caret Width (`1px`, `2px`, `3px`).

## Zinsser `describe()` / `it()` Test Suite
- `describe("SettingsSheet")`:
  - `it("emits theme selection when swatch is clicked")`
  - `it("emits clamped font size and measure on slider movement")`
  - `it("dismisses cleanly on escape key")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] File length <= 180 lines, zero `else` keywords.
- [ ] Visual layout matches `_meta/mockups/preferences_settings_modal.jpg`."""
    },
    {
        'id': '347ebe23-181e-4cb9-9000-78fd7e921b7d',
        'title': '[FELT-UI-4] Connect Live Canvas Reactive Sync & QSettings Persistence',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Create `preferences.h` / `preferences.cpp` (<= 110 lines, zero `else`, 100% branch-covered) managing configuration state and `QSettings` (`com.nodesman.felt`) serialization.
- **Live Sync**: Connect `SettingsSheet` signals to `MainWindow` and `FeltEditor` so theme, font family, font size, column width, caret width, and HUD visibility update instantaneously without restart.

## Zinsser `describe()` / `it()` Test Suite
- `describe("PreferencesContract")`:
  - `it("roundtrips all user settings through storage")`
  - `it("clamps out of range font sizes and column widths to safe bounds")`
  - `it("restores default obsidian configuration when storage is empty")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] 100% branch coverage on `preferences.cpp` via `llvm-cov`.
- [ ] Settings persist across application restarts."""
    },
    {
        'id': 'ad498b62-2f6d-4693-8465-82ff26a802aa',
        'title': '[FELT-UI-5] Codify William Zinsser Test Suite for Themes, Chrome & Preferences',
        'desc': """## Outcome Contract & Deliverables
- **Code**: Migrate all tests in `tests/` to the `zinsser.h` `describe()` / `it()` harness and verify full end-to-end integration across `grammar`, `theme`, `measure`, `preferences`, `editor`, and `mainwindow`.

## Zinsser `describe()` / `it()` Test Suite
- `describe("FeltEndToEnd")`:
  - `it("switches all canvas and hud colors when theme changes")`
  - `it("recalculates viewport margins immediately when measure changes")`
  - `it("handles mouse clicks across viewport and margins without recursion")`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `task gate` passes with exit code 0 (0 Calisthenics violations, 0 Zinsser violations, 100% core branch coverage, 100% test pass)."""
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
