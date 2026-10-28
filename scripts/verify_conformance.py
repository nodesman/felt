#!/usr/bin/env python3
"""
verify_conformance.py - Sovereign Conformance & Object Calisthenics Gate for Felt

Audits the C++ codebase against:
1. Object Calisthenics Rule 7: Small entities (<= 200 lines per file)
2. Object Calisthenics Rule 2: Zero 'else' keywords
3. William Zinsser Mandate: Zero bureaucratic words in test suites
"""

import sys
import re
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent.parent

BANNED_TEST_WORDS = [
    "should", "properly", "correctly", "successfully", "works_as_expected"
]

BANNED_TAUTOLOGIES = [
    r"\|\|\s*true\b",
    r"true\s*\|\|",
    r"QVERIFY\(\s*true\s*\)",
    r"expect\(\s*true\s*\)\.toBe\(\s*true\s*\)",
]

def check_file_lengths():
    violations = []
    cpp_files = list(ROOT_DIR.glob("*.h")) + list(ROOT_DIR.glob("*.cpp"))
    for p in sorted(cpp_files):
        with open(p, "r", encoding="utf-8") as f:
            lines = f.readlines()
        count = len(lines)
        if count > 200:
            violations.append((p.name, count, 200))
    return violations

def check_no_else():
    violations = []
    cpp_files = list(ROOT_DIR.glob("*.h")) + list(ROOT_DIR.glob("*.cpp"))
    else_regex = re.compile(r'\belse\b')
    for p in sorted(cpp_files):
        with open(p, "r", encoding="utf-8") as f:
            for idx, line in enumerate(f, 1):
                # Ignore comments
                stripped = line.strip()
                if stripped.startswith("//") or stripped.startswith("*"):
                    continue
                if else_regex.search(line):
                    violations.append((p.name, idx, stripped))
    return violations

def check_zinsser_tests():
    violations = []
    test_files = list((ROOT_DIR / "tests").glob("*.cpp")) + list((ROOT_DIR / "tests").glob("*.h"))
    for p in test_files:
        with open(p, "r", encoding="utf-8") as f:
            for idx, line in enumerate(f, 1):
                lower = line.lower()
                for banned in BANNED_TEST_WORDS:
                    if banned in lower:
                        violations.append((p.name, idx, f"bureaucratic word '{banned}'", line.strip()))
                for pattern in BANNED_TAUTOLOGIES:
                    if re.search(pattern, line):
                        violations.append((p.name, idx, f"tautological assertion '{pattern}'", line.strip()))
    return violations

def main():
    print("=== [GATE] Auditing Felt Object Calisthenics & Verification Gate ===")
    has_errors = False

    # 1. File Lengths
    len_violations = check_file_lengths()
    if len_violations:
        print("\n[FAIL] Object Calisthenics Rule 7: Small Entities (<= 200 lines):")
        for name, count, max_l in len_violations:
            print(f"  - {name}: {count} lines (exceeds {max_l} max) -> Assigned to [FELT-CORE-3]")
        has_errors = True
    else:
        print("[PASS] File lengths: All entities <= 200 lines.")

    # 2. No Else
    else_violations = check_no_else()
    if else_violations:
        print(f"\n[FAIL] Object Calisthenics Rule 2: Zero 'else' keywords ({len(else_violations)} found):")
        for name, line_num, code in else_violations[:5]:
            print(f"  - {name}:{line_num}: {code}")
        has_errors = True
    else:
        print("[PASS] Control Flow: Zero 'else' keywords detected.")

    # 3. William Zinsser Test Suite
    zinsser_violations = check_zinsser_tests()
    if zinsser_violations:
        print(f"\n[FAIL] William Zinsser Mandate: Bureaucratic test names ({len(zinsser_violations)} found):")
        for name, line_num, word, code in zinsser_violations:
            print(f"  - {name}:{line_num}: contains '{word}' -> {code}")
        has_errors = True
    else:
        print("[PASS] Test Suite: 100% William Zinsser clutter-free prose compliance.")

    if has_errors:
        print("\n[GATE ACTIVE] Conformance violations detected. Resolve assigned stories to pass gate.")
        return 1
    
    print("\n[GATE CLEAR] 100% Conformance achieved across all invariants.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
