"""
Language configs (`[slug].yml`): extensions plus build and run commands.

Lookup order per RFC (first match wins, see paths.py):
  [pkg]/languages/x.yml  ->  ~/.tcframe/languages/x.yml  ->  $TCFRAME_HOME/languages/x.yml

Command templates may use $FILENAME (source file name) and $BASE_FILENAME (name
without extension). Substitution runs per token after shlex splitting, so a
file name with spaces stays a single argument.

Example (cpp17.yml):
  name: C++17 (GCC)
  family: cpp
  extensions: [cc, cpp, c++]
  build: /usr/bin/g++ -std=c++17 -o $BASE_FILENAME $FILENAME
  run: ./$BASE_FILENAME
"""

import re
import shlex
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.paths import LANGUAGES, find_resource, search_roots
from tcframe_cli.simple_yaml import YamlError, parse

LANGUAGE_SUFFIX = '.yml'
REQUIRED_KEYS = ('name', 'family', 'extensions', 'build', 'run')
_SLUG = re.compile(r'^[A-Za-z0-9_-]+$')
_PLACEHOLDER = re.compile(r'\$(BASE_FILENAME|FILENAME)(?![A-Za-z0-9_])')


class LanguageError(Exception):
    """Raised for an invalid language config or slug; the message is user-facing."""


@dataclass(frozen=True)
class Language:
    slug: str
    name: str
    family: str
    extensions: tuple[str, ...]
    build: str  # may be empty for interpreted languages
    run: str
    source: Path

    def build_argv(self, filename: str) -> list[str]:
        """argv for building `filename`; empty when the language has no build step."""
        return _substitute(self.build, filename)

    def run_argv(self, filename: str) -> list[str]:
        """argv for running the artifact built from `filename`."""
        return _substitute(self.run, filename)


def _substitute(template: str, filename: str) -> list[str]:
    path = Path(filename)
    values = {'FILENAME': path.name, 'BASE_FILENAME': path.stem}
    return [_PLACEHOLDER.sub(lambda m: values[m.group(1)], token)
            for token in shlex.split(template)]


def check_slug(slug: str) -> str:
    """Return `slug` unchanged if it is a safe file stem, else raise LanguageError."""
    if not _SLUG.match(slug):
        raise LanguageError(f'invalid language slug {slug!r}; use letters, digits, "-" or "_"')
    return slug


def load_language(path: Path) -> Language:
    """Parse and validate one language file. The slug is the file stem."""
    try:
        data = parse(path.read_text(encoding='utf-8'))
    except (OSError, YamlError) as exc:
        raise LanguageError(f'{path}: {exc}')

    missing = [k for k in REQUIRED_KEYS if k not in data]
    if missing:
        raise LanguageError(f'{path}: missing key(s): {", ".join(missing)}')

    for key in ('name', 'family', 'build', 'run'):
        if not isinstance(data[key], str):
            raise LanguageError(f'{path}: {key} must be a single value')
    if not data['run']:
        raise LanguageError(f'{path}: run must not be empty')

    exts = data['extensions']
    if not isinstance(exts, list) or not exts or not all(isinstance(e, str) for e in exts):
        raise LanguageError(f'{path}: extensions must be a non-empty list of strings')

    return Language(
        slug=check_slug(path.stem),
        name=data['name'],
        family=data['family'],
        extensions=tuple(e.lstrip('.').lower() for e in exts),
        build=data['build'],
        run=data['run'],
        source=path,
    )


def find_language(
    slug: str,
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> Optional[Language]:
    """Return the highest-priority `<slug>.yml` language, or None if none exists."""
    check_slug(slug)
    path = find_resource(LANGUAGES, slug + LANGUAGE_SUFFIX, package_dir, env, home)
    return load_language(path) if path else None


def available_languages(
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> dict[str, Language]:
    """
    Every language visible from this package, keyed by slug. A slug found in a
    higher-priority root hides the same slug in lower ones.
    """
    found: dict[str, Language] = {}
    for base in search_roots(LANGUAGES, package_dir, env, home):
        folder = base / LANGUAGES
        if not folder.is_dir():
            continue
        for path in sorted(folder.glob('*' + LANGUAGE_SUFFIX)):
            if path.stem not in found:
                found[path.stem] = load_language(path)
    return found
