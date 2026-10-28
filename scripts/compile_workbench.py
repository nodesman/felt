#!/usr/bin/env python3
"""
compile_workbench.py - Screen-Based Prompt Pipeline Assembler for Felt

Reads structured screen data from prompts/screens/<screen_name>/, injects it
at the top-level preamble into prompts/templates/, and generates the active
agent execution files in workbench/<screen_name>.workbench.md.
"""

import os
import sys
import json
import argparse
from pathlib import Path

def load_file_content(path: Path) -> str:
    if not path.is_file():
        return ""
    with open(path, "r", encoding="utf-8") as f:
        return f.read().strip()

def compile_screen(screen_dir: Path, template_path: Path, output_dir: Path, verbose: bool = False):
    screen_name = screen_dir.name
    metadata_file = screen_dir / "metadata.json"
    yaml_file = screen_dir / "screen_data.yaml"
    json_file = screen_dir / "screen_data.json"
    md_file = screen_dir / "screen_data.md"

    metadata = {}
    if metadata_file.exists():
        try:
            with open(metadata_file, "r", encoding="utf-8") as f:
                metadata = json.load(f)
        except Exception as e:
            print(f"[WARN] Failed to parse metadata.json in {screen_name}: {e}")

    screen_title = metadata.get("screen_title", screen_name.replace("_", " ").title())

    # Collect structured data payload
    data_payload_parts = []
    
    if metadata:
        data_payload_parts.append("# Screen Metadata")
        data_payload_parts.append(json.dumps(metadata, indent=2))
        data_payload_parts.append("")

    if yaml_file.exists():
        data_payload_parts.append(f"# Structured Screen Data (YAML) - {screen_name}")
        data_payload_parts.append(load_file_content(yaml_file))
        data_payload_parts.append("")
    elif json_file.exists():
        data_payload_parts.append(f"# Structured Screen Data (JSON) - {screen_name}")
        data_payload_parts.append(load_file_content(json_file))
        data_payload_parts.append("")
    elif md_file.exists():
        data_payload_parts.append(f"# Screen Documentation - {screen_name}")
        data_payload_parts.append(load_file_content(md_file))
        data_payload_parts.append("")

    compiled_data = "\n".join(data_payload_parts).strip()

    # Load base prompt template
    template_content = load_file_content(template_path)
    if not template_content:
        raise ValueError(f"Template at {template_path} is empty or missing.")

    # Top-level injection
    if "{{DATA}}" in template_content:
        compiled_prompt = template_content.replace("{{DATA}}", compiled_data)
    else:
        compiled_prompt = f"<screen_data>\n{compiled_data}\n</screen_data>\n\n{template_content}"

    compiled_prompt = compiled_prompt.replace("{{SCREEN_TITLE}}", screen_title)
    compiled_prompt = compiled_prompt.replace("{{SCREEN_NAME}}", screen_name)

    # Output file path
    output_dir.mkdir(parents=True, exist_ok=True)
    out_file = output_dir / f"{screen_name}.workbench.md"

    with open(out_file, "w", encoding="utf-8") as f:
        f.write(compiled_prompt + "\n")

    if verbose:
        print(f"[OK] Compiled: {screen_name} -> {out_file.relative_to(output_dir.parent)}")
    return out_file

def main():
    parser = argparse.ArgumentParser(description="Assemble workbench files from prompts/screens/")
    parser.add_argument("--screen", help="Name of a specific screen directory to compile")
    parser.add_argument("--template", default="screen_builder.prompt.md", help="Base template name in prompts/templates/")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    args = parser.parse_args()

    base_dir = Path(__file__).resolve().parent.parent
    prompts_dir = base_dir / "prompts"
    screens_dir = prompts_dir / "screens"
    templates_dir = prompts_dir / "templates"
    workbench_dir = base_dir / "workbench"

    template_path = templates_dir / args.template
    if not template_path.exists():
        # Fallback to any .prompt.md in templates
        candidates = list(templates_dir.glob("*.prompt.md"))
        if candidates:
            template_path = candidates[0]
        else:
            print(f"[ERROR] No template found in {templates_dir}", file=sys.stderr)
            sys.exit(1)

    if not screens_dir.exists():
        print(f"[ERROR] Screens directory {screens_dir} does not exist", file=sys.stderr)
        sys.exit(1)

    target_screens = []
    if args.screen:
        target_path = screens_dir / args.screen
        if not target_path.is_dir():
            print(f"[ERROR] Screen directory not found: {target_path}", file=sys.stderr)
            sys.exit(1)
        target_screens.append(target_path)
    else:
        target_screens = [d for d in screens_dir.iterdir() if d.is_dir() and not d.name.startswith(".")]

    if not target_screens:
        print("[WARN] No screen directories found to compile.")
        sys.exit(0)

    print(f"=== Compiling {len(target_screens)} Screen Workbench Prompt(s) ===")
    compiled_files = []
    for screen_dir in sorted(target_screens):
        out_file = compile_screen(screen_dir, template_path, workbench_dir, verbose=True)
        compiled_files.append(out_file)

    print(f"=== Successfully Compiled {len(compiled_files)} Workbench Target(s) ===")

if __name__ == "__main__":
    main()
