"""
`tcframe package`: make the package, then zip the source and distribution
packages into the build directory as `[slug]-source.zip` and `[slug].zip`.

Each archive holds a single top-level `[slug]/` folder. The source package is
the problem sources; the distribution package adds the generated `spec.yml`
and `tc/` from the build directory.
"""

import zipfile
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.build import DEFAULT_SOLUTION
from tcframe_cli.build import SPEC_YML
from tcframe_cli.make import make_package
from tcframe_cli.scaffold import SPEC_FILE
from tcframe_cli.validate import VALIDATOR_BINARY

# Source package entries from the RFC layout (plus the 1.x flat reference solution,
# until solutions/ref lands in Phase 3). Missing entries are skipped.
SOURCE_ENTRIES = (
    SPEC_FILE,
    'metadata.yml',
    'solutions',
    'solution',
    'scorer.cpp',
    'scorer',
    'manager',
    'languages',
)
# Generated entries that only the distribution package carries.
DIST_ENTRIES = (SPEC_YML, 'tc', VALIDATOR_BINARY)


def _collect(base: Path, entries: tuple[str, ...]) -> list[tuple[Path, str]]:
    """Return (file, path-relative-to-base) for each existing entry, recursing into dirs."""
    found = []
    for name in entries:
        path = base / name
        if path.is_file():
            found.append((path, name))
        elif path.is_dir():
            for sub in sorted(path.rglob('*')):
                if sub.is_file():
                    found.append((sub, sub.relative_to(base).as_posix()))
    return found


def _write_zip(zip_path: Path, slug: str, files: list[tuple[Path, str]]) -> None:
    with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zf:
        for path, rel in files:
            zf.write(path, arcname=f'{slug}/{rel}')


def write_archives(package_dir: Path, build_dir: Path, slug: str) -> tuple[Path, Path]:
    """Write `[slug]-source.zip` and `[slug].zip` into `build_dir`; return both paths."""
    source_files = _collect(package_dir, SOURCE_ENTRIES)
    dist_files = source_files + _collect(build_dir, DIST_ENTRIES)

    source_zip = build_dir / f'{slug}-source.zip'
    dist_zip = build_dir / f'{slug}.zip'
    _write_zip(source_zip, slug, source_files)
    _write_zip(dist_zip, slug, dist_files)
    return source_zip, dist_zip


def package_package(
    package_dir: Path,
    build_dir: Optional[Path] = None,
    solution: str = DEFAULT_SOLUTION,
    env: Optional[Mapping[str, str]] = None,
) -> tuple[Path, Path]:
    """Make the package, then write both archives. Returns (source_zip, dist_zip)."""
    build_dir = make_package(package_dir, build_dir, solution=solution, env=env)
    slug = package_dir.resolve().name
    return write_archives(package_dir, build_dir, slug)
