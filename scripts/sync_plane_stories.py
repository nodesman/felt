#!/usr/bin/env python3
import subprocess

cli = '/Users/rajsekharan/projects/foundry/kernel/skills/plane-ops/scripts/plane_client.py'
workspace = 'notionworks'
project = '704df24c-5db5-4578-86e2-67448b83ab78'

updates = [
    {
        'id': '3af21322-41ad-4e73-83d3-6fcde411bfdb',
        'desc': """## Outcome Contract & Code Deliverable
- **Code**: Extract pure markdown matching rules into `grammar.h` / `grammar.cpp` (< 90 lines, zero GUI code, accepts `QStringView` for zero-allocation parsing).
- **Styling Rules**:
  - In-situ receding heading prefix (`#`) in muted slate `#4B5563`. Heading text in bold bone-white `#FFFFFF` (1.25x scale).
  - Jewel-tone electric cyan bullets (`- `, `* `) in `#3B82F6` with bone-white list item text.
  - Zero HTML conversion; raw markdown syntax preserved.

## William Zinsser Test Suite
- `highlights_heading_with_receding_prefix()`
- `illuminates_list_bullet_in_jewel_cyan()`
- `rejects_unspaced_heading_marker()`
- `preserves_raw_markdown_characters_in_situ()`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] `grammar.h/cpp` contains zero Qt GUI includes (pure token logic).
- [ ] File length strictly <= 100 lines, 0 'else' keywords.
- [ ] 100% test pass via `ctest`."""
    },
    {
        'id': 'b5eeb9a3-483f-436f-a257-b45412351369',
        'desc': """## Outcome Contract & Code Deliverable
- **Code**: Create `tests/test_grammar_conformance.cpp` validating all markdown lexical grammar tokens against positive and negative test cases.

## William Zinsser Test Suite
- `rejects_unspaced_heading_marker()`
- `parses_nested_blockquote()`
- `traps_unbalanced_asterisks()`
- `parses_multi_level_unordered_lists()`
- `enforces_three_character_minimum_on_scene_break()`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Zero bureaucratic filler words ('should', 'properly', 'correctly', 'works as expected') in test names.
- [ ] Every test name begins with an active, present-tense verb.
- [ ] 100% test pass via `ctest --output-on-failure`."""
    },
    {
        'id': '3c902805-5803-48f1-98a3-8630eca86f95',
        'desc': """## Outcome Contract & Code Deliverable
- **Code**:
  - Implement native unified frameless window chrome via Cocoa attributes (`NSWindowStyleMaskFullSizeContentView`, transparent titlebar, traffic lights at 18, 18).
  - Implement smart list continuation on `Enter` (scaffolds `- ` on next line).
  - Implement empty bullet dismissal on consecutive `Enter` (clears `- ` and restores paragraph).
  - Implement 4-space tab indentation for nested list items.

## William Zinsser Test Suite
- `continues_list_bullet_on_enter()`
- `clears_empty_list_bullet_on_consecutive_enter()`
- `indents_list_item_by_four_spaces_on_tab()`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] No grey OS title bar band; canvas spans entire window.
- [ ] Window drag operational on top margin without interfering with text selection.
- [ ] All 3 list interaction tests passing 100%."""
    },
    {
        'id': '410d443e-2506-4628-b732-382eb3da52fe',
        'desc': """## Outcome Contract & Code Deliverable
- **Code**:
  - Implement Ambient Radial Vignette background on `FeltEditor` (center `#0D0F14` gently fading to `#060709` edges).
  - Enforce 160% proportional line height rhythm (`QTextBlockFormat::setLineHeight(160, QTextBlockFormat::ProportionalHeight)`).
  - Optical Bone White (`#ECEBE8`) body typography and Slate Smoke (`#3E4249`) focus dimming.
  - Implement `ThemePalette` and `ThemeRegistry` for all 4 themes (Obsidian, Parchment, Slate, Midnight).

## William Zinsser Test Suite
- `enforces_proportional_line_height_at_one_hundred_sixty_percent()`
- `renders_radial_ambient_vignette_palette()`
- `dims_inactive_blocks_to_slate_smoke()`
- `verifies_wcag_aaa_contrast_on_all_four_palettes()`

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Editor canvas displays visible ambient sheen, zero flat textarea feel.
- [ ] Contrast ratio >= 7:1 confirmed mathematically.
- [ ] 100% test pass via `ctest`."""
    },
    {
        'id': 'd8ba3870-6df4-4c42-abab-f025bfa095bc',
        'desc': """## Outcome Contract & Code Deliverable
- **Code**: Implement `scripts/verify_conformance.py` and attach to `task gate` in `Taskfile.yml`.

## Automated Checks Enforced
1. **Object Calisthenics Gate**: Max 200 lines per file, zero 'else' keywords, max 1 indentation level per method.
2. **William Zinsser Gate**: Zero bureaucratic filler words in test names, active-verb leading words.
3. **Compiler Conformance Gate**: 100% CTest pass, zero warnings.
4. **Visual Gate**: Ambient radial vignette and 160% line height confirmed in codebase.

## The Brake Check (LLM Auditor Verification Gate)
- [ ] Script returns exit code 0 when all invariants hold.
- [ ] Script fails with non-zero exit code if any line exceeds 200 lines or contains 'else'."""
    }
]

for u in updates:
    cmd = [
        'python3', cli, '--workspace', workspace, 'update-issue',
        '--project', project,
        '--issue', u['id'],
        '--description', u['desc']
    ]
    res = subprocess.run(cmd, capture_output=True, text=True)
    print(res.stdout.strip())
