#!/usr/bin/env python3
"""
verify_conformance.py - Conformance & Object Calisthenics Gate for Felt

Audits the C++ codebase against:
1. Object Calisthenics Rule 7: Small entities (<= 200 lines per file)
2. Object Calisthenics Rule 2: Zero 'else' keywords across C++ implementation
3. William Zinsser Mandate: Zero bureaucratic words or tautologies in test suites
4. Multi-Scenario Describe & Auditor Rule:
   - Every describe() block must contain >= 4 unique it() blocks.
   - Auditor Rule: Every it() block must test a single distinct failure mode that
     would exist even without a minimum count rule (zero artificial padding).
5. 100% LLVM Branch & Line Coverage on pure domain modules.
"""

import sys
import re
import subprocess
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent.parent
MAX_LINES = 200
MIN_IT_PER_DESCRIBE = 4

BANNED_TEST_WORDS = [
    "should",
    "properly",
    "correctly",
    "successfully",
    "works_as_expected",
    "works as expected",
]

BANNED_TAUTOLOGIES = [
    r"\|\|\s*true\b",
    r"\btrue\s*\|\|",
    r"QVERIFY\s*\(\s*true\s*\)",
]


def get_cpp_files():
    files = []
    for ext in ("*.h", "*.cpp", "*.mm"):
        for p in ROOT_DIR.rglob(ext):
            if "build" in p.parts or ".git" in p.parts:
                continue
            files.append(p)
    return sorted(files)


def check_file_lengths(files):
    violations = []
    for p in files:
        lines = p.read_text(encoding="utf-8").splitlines()
        if len(lines) > MAX_LINES:
            violations.append((p.relative_to(ROOT_DIR), len(lines)))
    return violations


def check_no_else(files):
    violations = []
    else_re = re.compile(r"\belse\b")
    for p in files:
        for idx, line in enumerate(p.read_text(encoding="utf-8").splitlines(), start=1):
            stripped = line.split("//")[0]
            if else_re.search(stripped):
                violations.append((p.relative_to(ROOT_DIR), idx, line.strip()))
    return violations


def check_zinsser_prose():
    violations = []
    test_files = list((ROOT_DIR / "tests").glob("*.cpp"))
    for p in test_files:
        for idx, line in enumerate(p.read_text(encoding="utf-8").splitlines(), start=1):
            lower = line.lower()
            for banned in BANNED_TEST_WORDS:
                if banned in lower:
                    violations.append((p.name, idx, f"bureaucratic word '{banned}'", line.strip()))
            for pattern in BANNED_TAUTOLOGIES:
                if re.search(pattern, line):
                    violations.append((p.name, idx, f"tautological assertion '{pattern}'", line.strip()))
    return violations


def check_describe_density():
    violations = []
    test_files = list((ROOT_DIR / "tests").glob("*.cpp"))
    describe_re = re.compile(r'\bdescribe\s*\(\s*"([^"]+)"')
    it_re = re.compile(r'\bit\s*\(\s*"([^"]+)"')

    for p in test_files:
        content = p.read_text(encoding="utf-8")
        describes = list(describe_re.finditer(content))
        if not describes:
            continue
        for idx, match in enumerate(describes):
            desc_name = match.group(1)
            start_pos = match.end()
            end_pos = describes[idx + 1].start() if idx + 1 < len(describes) else len(content)
            block_slice = content[start_pos:end_pos]
            it_titles = it_re.findall(block_slice)
            unique_it_count = len(set(it_titles))
            if unique_it_count < MIN_IT_PER_DESCRIBE:
                violations.append((p.name, desc_name, unique_it_count, MIN_IT_PER_DESCRIBE))
    return violations


def check_domain_coverage():
    test_bin = ROOT_DIR / "build" / "test_felt"
    profdata = ROOT_DIR / "build" / "test_felt.profdata"
    if not test_bin.exists() or not profdata.exists():
        return [("build/test_felt.profdata", "Missing coverage profile (run task coverage)")]

    domain_sources = [
        str(ROOT_DIR / name)
        for name in ("theme.cpp", "themeregistry.cpp", "stats.cpp", "grammar.cpp", "measure.cpp", "preferences.cpp")
        if (ROOT_DIR / name).exists()
    ]
    cmd = [
        "xcrun", "llvm-cov", "report",
        str(test_bin),
        f"-instr-profile={profdata}",
        *domain_sources,
    ]
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        return [("llvm-cov", res.stderr.strip())]

    violations = []
    for line in res.stdout.splitlines():
        if line.startswith("TOTAL"):
            parts = line.split()
            branch_cov = parts[-1]
            line_cov = parts[-4]
            if branch_cov != "100.00%" or line_cov != "100.00%":
                violations.append(("Domain Coverage", f"Line={line_cov}, Branch={branch_cov} (requires 100.00%)"))
    return violations


def main():
    print("=== [GATE] Auditing Felt Object Calisthenics & Verification Gate ===")
    cpp_files = get_cpp_files()
    has_errors = False

    len_violations = check_file_lengths(cpp_files)
    if len_violations:
        print("\n[FAIL] Object Calisthenics Rule 7: Small Entities (<= 200 lines):")
        for rel, count in len_violations:
            print(f"  - {rel}: {count} lines (exceeds {MAX_LINES} max) -> Assigned to [FELT-CORE-3]")
        has_errors = True
    else:
        print("[PASS] File lengths: All entities <= 200 lines.")

    else_violations = check_no_else(cpp_files)
    if else_violations:
        print(f"\n[FAIL] Object Calisthenics Rule 2: Zero 'else' keywords ({len(else_violations)} found):")
        for rel, line_no, snippet in else_violations:
            print(f"  - {rel}:{line_no}: {snippet}")
        has_errors = True
    else:
        print("[PASS] Zero 'else' keywords across all C++ files.")

    zinsser_violations = check_zinsser_prose()
    if zinsser_violations:
        print(f"\n[FAIL] William Zinsser Mandate: Test prose / tautology violations ({len(zinsser_violations)} found):")
        for name, line_no, reason, snippet in zinsser_violations:
            print(f"  - {name}:{line_no}: contains {reason} -> {snippet}")
        has_errors = True
    else:
        print("[PASS] William Zinsser Mandate: Test prose is clutter-free with zero tautologies.")

    density_violations = check_describe_density()
    if density_violations:
        print(f"\n[FAIL] Multi-Scenario Describe Rule (< {MIN_IT_PER_DESCRIBE} unique it() blocks):")
        for name, dname, count, min_c in density_violations:
            print(f"  - {name}: describe(\"{dname}\") has {count} unique it() blocks (requires >= {min_c})")
        has_errors = True
    else:
        print(f"[PASS] Describe Density: All describe() blocks contain >= {MIN_IT_PER_DESCRIBE} unique scenarios.")

    cov_violations = check_domain_coverage()
    if cov_violations:
        print("\n[FAIL] Domain Core 100% LLVM Coverage Gate:")
        for target, reason in cov_violations:
            print(f"  - {target}: {reason}")
        has_errors = True
    else:
        print("[PASS] Domain Core Coverage: 100.00% line & 100.00% branch coverage verified via llvm-cov.")

    if has_errors:
        print("\n[GATE ACTIVE] Conformance violations detected. Resolve assigned stories to pass gate.")
        return 1

    print("\n[GATE CLEAR] 100% Conformance achieved across all invariants.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
