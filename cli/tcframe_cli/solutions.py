"""
Parse a source package's `solutions/` tree (RFC "solutions/" section).

  solutions/
    ref/         exactly one entry: the reference solution (expected AC)
    ac/ wa/ tle/ rte/ ok/ failed/
    ok-75/ tle-75/ ...     verdict with an optional expected score after "-"

Each entry is a solution: a file (a single source file) or a directory (a
multi-file solution, required for functional problems). Hidden entries
(starting with ".") are ignored.

The expected verdict of a directory:
  ac -> AC, wa -> WA, tle -> TLE, rte -> RTE, ok -> OK
  failed -> WA, TLE or RTE (any of them)
  ref -> AC
The `-N` suffix sets the expected score; without it the score is not checked.
"""

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

SOLUTIONS_DIR = 'solutions'
REF_GROUP = 'ref'
_GROUP = re.compile(r'^(ac|wa|tle|rte|ok|failed)(?:-(\d+))?$')

# Verdict codes as printed by evaluators (see registry/README.md).
AC, OK, WA, RTE, TLE = 'AC', 'OK', 'WA', 'RTE', 'TLE'
_VERDICT_OF = {'ac': frozenset({AC}), 'wa': frozenset({WA}), 'tle': frozenset({TLE}),
               'rte': frozenset({RTE}), 'ok': frozenset({OK}),
               'failed': frozenset({WA, TLE, RTE})}


class SolutionsError(Exception):
    """Raised when the solutions tree is missing or malformed; the message is user-facing."""


@dataclass(frozen=True)
class Expected:
    verdicts: frozenset[str]  # any of these verdicts is a match
    score: Optional[int]      # expected score for `ok-N`/`tle-N`...; None = unchecked


@dataclass(frozen=True)
class Solution:
    group: str     # directory name under solutions/, e.g. "ref", "ok-75"
    name: str      # entry name, e.g. "fushar.cpp" or a solution directory name
    path: Path
    expected: Expected

    @property
    def is_dir(self) -> bool:
        return self.path.is_dir()

    @property
    def label(self) -> str:
        return f'{self.group}/{self.name}'


def parse_group(group: str) -> Expected:
    """Expected verdict/score for a solutions/ subdirectory name."""
    if group == REF_GROUP:
        return Expected(frozenset({AC}), None)
    match = _GROUP.match(group)
    if match is None:
        raise SolutionsError(
            f"unknown solutions directory '{group}'; expected ref, ac, wa, tle, rte, ok, "
            f"failed, or one of these with a score suffix, e.g. ok-75")
    verdict, score = match.groups()
    return Expected(_VERDICT_OF[verdict], int(score) if score is not None else None)


def parse_solutions(package_dir: Path) -> list[Solution]:
    """
    Every solution in `package_dir/solutions/`. `ref` is listed first, then the other
    groups in name order, and entries are sorted by name within a group.
    """
    root = package_dir / SOLUTIONS_DIR
    if not root.is_dir():
        raise SolutionsError(f"'{root}' does not exist; a source package needs solutions/ref/")

    groups = sorted(p for p in root.iterdir() if p.is_dir() and not p.name.startswith('.'))
    names = [p.name for p in groups]
    if REF_GROUP not in names:
        raise SolutionsError(f"'{root}' has no '{REF_GROUP}/' directory with the reference solution")

    solutions: list[Solution] = []
    for group_dir in sorted(groups, key=lambda p: (p.name != REF_GROUP, p.name)):
        expected = parse_group(group_dir.name)
        entries = sorted(e for e in group_dir.iterdir() if not e.name.startswith('.'))
        if group_dir.name == REF_GROUP and len(entries) != 1:
            raise SolutionsError(
                f"'{group_dir}' must contain exactly one reference solution, found {len(entries)}")
        for entry in entries:
            solutions.append(Solution(group_dir.name, entry.name, entry, expected))
    return solutions
