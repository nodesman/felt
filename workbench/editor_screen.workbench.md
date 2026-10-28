# Screen Builder & Architectural Prompt: Felt Main Editor Screen (Unified Frameless Window & Dynamic Themes)

<screen_data>
# Screen Metadata
{
  "screen_id": "editor_screen",
  "screen_title": "Felt Main Editor Screen (Unified Frameless Window & Dynamic Themes)",
  "version": "1.1.0",
  "status": "active",
  "target_platforms": [
    "macOS",
    "Linux",
    "Windows"
  ],
  "author": "Helm Sovereign Workbench"
}

# Structured Screen Data (YAML) - editor_screen
screen_name: "editor_screen"
title: "Felt Main Writing Canvas"

window_architecture:
  chrome_type: "native_unified_frameless"
  macos_configuration:
    style_mask:
      - "NSWindowStyleMaskTitled"
      - "NSWindowStyleMaskClosable"
      - "NSWindowStyleMaskMiniaturizable"
      - "NSWindowStyleMaskResizable"
      - "NSWindowStyleMaskFullSizeContentView"
    titlebar_transparent: true
    title_visibility: "NSWindowTitleHidden"
    traffic_lights_offset:
      x: 18
      y: 18
  cross_platform_fallback:
    linux: "client_side_decorations_dark"
    windows: "dwm_extend_frame_into_client_area"

canvas_layout:
  column_measure:
    default_width_px: 720
    character_measure: "~68-72 characters"
    padding_top_px: 80
    padding_bottom_px: 80
    side_margins: "dynamically centered: (window_width - 720) / 2"
  scroll_policy:
    vertical_scrollbar: "hidden"
    horizontal_scrollbar: "hidden"

themes:
  default_theme: "obsidian_void"
  single_source_of_truth: "ThemeRegistry (theme.h / themeregistry.h)"
  wcag_contrast_reference: "bg_vignette_center"
  palettes:
    obsidian_void:
      name: "Obsidian Void"
      type: "dark"
      bg_outer: "#08090C"
      bg_vignette_center: "#0D0F14"
      background: "#08090C"
      text_main: "#ECEBE8"
      text_dimmed: "#464B53"
      selection_bg: "#1E3A8A"
      selection_text: "#FFFFFF"
      caret_color: "#5E81AC"
      accent: "#5E81AC"
      markdown_prefix: "#4B5563"
      hud_text: "#6B7280"
      description: "Default dark canvas. Deep obsidian radial vignette (#0D0F14 center tapering to #08090C outer edges) with optical bone white prose (#ECEBE8), WCAG-verified slate smoke focus dimming (#464B53, 2.15:1 contrast against #0D0F14 center and 2.29:1 against #08090C outer), and Nordic frost accent (#5E81AC)."

    warm_parchment:
      name: "Warm Parchment"
      type: "light"
      background: "#FBF8F2"
      text_main: "#2D2A26"
      text_dimmed: "#C4BDB3"
      selection_bg: "#E8DFD1"
      selection_text: "#2D2A26"
      caret_color: "#D9532F"
      markdown_prefix: "#A8A196"
      hud_text: "#8C857B"
      description: "Tactile editorial paper aesthetic. Natural warm cream with deep sepia ink and terracotta caret."

    slate_concrete:
      name: "Slate Concrete"
      type: "dark_muted"
      background: "#1A1D20"
      text_main: "#E6EDF3"
      text_dimmed: "#4A5157"
      selection_bg: "#2D333B"
      selection_text: "#FFFFFF"
      caret_color: "#F97316"
      markdown_prefix: "#636C76"
      hud_text: "#768390"
      description: "Industrial brutalist slate. Low eye-strain matte dark grey with frosty white typography and energetic amber caret."

    midnight_indigo:
      name: "Midnight Indigo"
      type: "dark_saturated"
      background: "#0A0E17"
      text_main: "#E2E8F0"
      text_dimmed: "#334155"
      selection_bg: "#1E293B"
      selection_text: "#FFFFFF"
      caret_color: "#38BDF8"
      markdown_prefix: "#475569"
      hud_text: "#64748B"
      description: "Deep nocturnal marine atmosphere. Obsidian blue-black with silver starlight prose and vivid sky cyan caret."

markdown_in_situ_rules:
  headings:
    prefix_color: "theme.markdown_prefix"
    text_weight: "Bold"
    scale: 1.15
  blockquotes:
    prefix_color: "theme.markdown_prefix"
    text_style: "Italic"
    color: "theme.text_main"
  emphasis:
    bold: "FontWeight::Bold"
    italic: "FontItalic"
  divider:
    character: "---"
    color: "theme.text_dimmed"

modes:
  focus_mode:
    shortcut: "Cmd+D"
    active_block_color: "theme.text_main"
    inactive_block_color: "theme.text_dimmed"
  typewriter_mode:
    shortcut: "Cmd+T"
    behavior: "Active line vertically centered upon typing and scrolling"
</screen_data>

## Role & Goal Definitions
You are the **Lead C++ / Qt 6 Systems Engineer & Principal Interface Designer** for **Felt** — an unbloated, distraction-free markdown writing application.
Your goal is to implement, refine, and verify the screen component described in `<screen_data>` according to the highest visual and architectural standards.

## Execution Constraints
- **Framework & Language**: Native C++17 with Qt 6.5.3 Widgets. No web wrappers, no Electron, no Tauri.
- **Window Chrome**: Unified, frameless native chrome. On macOS, use native Cocoa window attributes (`NSWindowStyleMaskFullSizeContentView`, `titlebarAppearsTransparent`, seamless traffic lights) or platform-appropriate frameless handling so windows look integrated without an ugly default titlebar chrome, while retaining native window management on macOS, Linux, and Windows.
- **Visual Aesthetic**:
  - Distraction-free minimalism inspired by iA Writer, Ulysses, and typography-first tools.
  - Centered measure column with optimal character measure (~65-72 characters).
  - High-acuity typography with configurable monospaced / proportional fonts.
- **Theme Substrate**: Must support dynamic theme switching across the 4 core palettes defined in the screen specification.
- **William Zinsser Test Mandate**: All behavior must be accompanied by clutter-free, active-verb unit tests in muscular, clutter-free prose.

## Required Output Format
- Clean C++ header (`.h`) and implementation (`.cpp`) files adhering to Object Calisthenics (small methods, single responsibility, no deep nesting).
- CMakeLists.txt updates if new files are introduced.
- Deterministic test cases verifying state changes, geometry calculations, and mode toggles.
