# Felt Incremental Stories & Zinsser Test Suite Roadmap

**Project**: Felt Writer (`[FELT]` in `notionworks` Plane workspace)  
**Target Outcome**: Bridge the C++17 / Qt 6.5.3 editor to 100% visual, behavioral, and Object Calisthenics conformance, governed by William Zinsser `describe()` / `it()` specifications and 100% LLVM branch coverage on core domain modules.

---

## Explicit v1 Scope Decisions (5 Accepted Refinements & 2 Declined Non-Goals)

### Accepted Into v1 Specifications & Tests
1. **WCAG Contrast Math Fix (`#464B53` vs `#3E4249`)**: Raised *Obsidian Void* `textDimmed` from `#3E4249` (`1.897:1`) to **`#464B53`** (`2.15:1` against `#0D0F14` vignette center, `2.29:1` against `#08090C` outer edge). All WCAG contrast checks are measured against `bgVignetteCenter`.
2. **Single Source of Truth for Palette Hex Values**: `ThemeRegistry` (`theme.h` / `themeregistry.h`) is the only place hex strings live. All editor, highlighter, and E2E tests assert against `ThemeRegistry::obsidianVoid().textMain` and `textDimmed`.
3. **Atomic Single-Step `Cmd+Z` Undo on Smart Lists**: All smart list mutations (`Enter`, `Tab`, `Shift+Tab`) are wrapped in `QTextCursor::beginEditBlock()` and `endEditBlock()` so a single `Cmd+Z` reverts both the auto-inserted marker and newline.
4. **Escaped Asterisks (`\*`), Ordered Lists (`1. ` -> `2. `), Multibyte UTF-8 Word Count & Save Failure Safety**: Added explicit `it()` specifications for backslash-escaped asterisks, numbered list auto-increment, multibyte UTF-8 word counting, UTF-8/newline save fidelity, and read-only save failure preservation.
5. **Real Offscreen Window Activation & Auditor Rule**: `FeltAppDriver` calls `window.show()`, `window.activateWindow()`, and `QTest::qWaitForWindowActive(&window)` under `QT_QPA_PLATFORM=offscreen` so `editor->hasFocus()` is genuinely `true` without `|| true`. Multi-claim `it()` titles are unbundled, and the Auditor Rule enforces zero artificial padding.

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

---

## Epic 4: Verification Gate & 100% Branch Coverage

### Story 11: `[FELT-GATE-1]` Implement Automated Calisthenics, Zinsser & Auditor Verification Gate
- Enforces $\le 200$ lines per file, `0 else` keywords, zero bureaucratic filler words, zero `|| true` tautologies, $\ge 4$ unique `it()` blocks per `describe()`, and the Auditor Rule (every `it()` tests a single distinct failure mode with zero artificial padding).

### Story 12: `[FELT-TEST-1]` Build Zinsser BDD Harness (`zinsser.h`), 100% LLVM Branch Coverage & Legacy Test Refactor
- Builds `tests/zinsser.h` ($\le 90$ lines), wires `-fprofile-instr-generate -fcoverage-mapping` + `xcrun llvm-cov` 100% branch coverage on pure domain modules, and refactors `tests/test_felt.cpp` to use `ThemeRegistry` colors and real offscreen window focus activation.
