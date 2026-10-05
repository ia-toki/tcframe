"""
Multi-file solutions (RFC "Functional", SPEC.md T6.4).

A multi-file solution is a directory whose source files are named by solution key:

  solutions/ref/fushar/encoder.cpp
  solutions/ref/fushar/decoder.cpp

The keys come from spec.yml (`evaluator.solution_keys`). Other files in the directory,
such as headers, are ignored. Every key file must belong to one language family, which
selects the evaluator's build_<family> and run_<family> scripts (SPEC.md T6.3). The
directory is only accepted when its evaluator takes custom solution keys
(`custom_solution_keys: true` in evaluator.yml); otherwise it is an output-only
submission or an error.

The runner gets the key files as --solution-file=<key>=<path>, plus the evaluator
directory, the family and the manager helper directory.
"""

from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.evaluators import EvaluatorConfig, EvaluatorError, find_evaluator
from tcframe_cli.languages import Language

MANAGER_DIR = 'manager'


class MultifileError(Exception):
    """Raised when a multi-file solution cannot be run; the message is user-facing."""


@dataclass(frozen=True)
class SpecInfo:
    evaluator_slug: Optional[str]
    solution_keys: tuple[str, ...]


def read_spec_info(spec_yml: Path) -> SpecInfo:
    """Read `evaluator.slug` and `evaluator.solution_keys` from a spec.yml written by the C++ spec."""
    slug: Optional[str] = None
    keys: tuple[str, ...] = ()
    in_evaluator = False
    try:
        lines = spec_yml.read_text().splitlines()
    except OSError as exc:
        raise MultifileError(f"cannot read '{spec_yml}': {exc}") from exc
    for line in lines:
        if not line.startswith(' '):
            in_evaluator = line.rstrip() == 'evaluator:'
            continue
        if not in_evaluator:
            continue
        key, _, value = line.strip().partition(':')
        value = value.strip()
        if key == 'slug':
            slug = value or None
        elif key == 'solution_keys':
            inner = value.strip('[]')
            keys = tuple(k.strip() for k in inner.split(',') if k.strip())
    if not keys:
        raise MultifileError(f"'{spec_yml}' has no evaluator.solution_keys; rebuild the package")
    return SpecInfo(slug, keys)


def evaluator_for(spec: SpecInfo, env: Optional[Mapping[str, str]] = None) -> EvaluatorConfig:
    """The evaluator named in spec.yml, looked up in the registry."""
    if spec.evaluator_slug is None:
        raise MultifileError('the spec uses a custom evaluator; multi-file solutions need a registry evaluator')
    try:
        config = find_evaluator(spec.evaluator_slug, env=env)
    except EvaluatorError as exc:
        raise MultifileError(str(exc)) from exc
    if config is None:
        raise MultifileError(f"evaluator {spec.evaluator_slug!r} not found in the registry")
    return config


def is_multifile(solution_dir: Path, spec_yml: Path, env: Optional[Mapping[str, str]] = None) -> bool:
    """True when `solution_dir` is a multi-file solution (a directory whose evaluator takes custom keys)."""
    if not solution_dir.is_dir():
        return False
    return evaluator_for(read_spec_info(spec_yml), env).custom_solution_keys


def _source_extensions(languages: Mapping[str, Language]) -> set[str]:
    return {ext for language in languages.values() for ext in language.extensions}


def key_files(solution_dir: Path, keys: tuple[str, ...], languages: Mapping[str, Language]) -> dict[str, Path]:
    """
    The one source file per key: a file whose stem is the key and whose extension a
    language claims (encoder.cpp for key encoder). Headers such as encoder.h are skipped.
    """
    sources = _source_extensions(languages)
    files: dict[str, Path] = {}
    entries = [p for p in sorted(solution_dir.iterdir())
               if p.is_file() and not p.name.startswith('.') and p.suffix.lstrip('.').lower() in sources]
    for key in keys:
        matches = [p for p in entries if p.stem == key]
        if not matches:
            raise MultifileError(f"'{solution_dir}' has no file for solution key '{key}' (expected {key}.<ext>)")
        if len(matches) > 1:
            names = ', '.join(p.name for p in matches)
            raise MultifileError(f"'{solution_dir}' has more than one file for key '{key}': {names}")
        files[key] = matches[0]
    return files


def family_of(files: Mapping[str, Path], languages: Mapping[str, Language]) -> str:
    """The single language family of the key files."""
    families: dict[str, list[str]] = {}
    for key, path in files.items():
        ext = path.suffix.lstrip('.').lower()
        language = next((languages[s] for s in sorted(languages) if ext in languages[s].extensions), None)
        if language is None:
            raise MultifileError(f"no language for '{path.name}' (key '{key}'); add a languages/*.yml that lists .{ext}")
        families.setdefault(language.family, []).append(path.name)
    if len(families) != 1:
        detail = '; '.join(f'{fam}: {", ".join(names)}' for fam, names in sorted(families.items()))
        raise MultifileError(f'key files must share one language family, found {detail}')
    return next(iter(families))


def multifile_args(
    solution_dir: Path,
    package_dir: Path,
    spec_yml: Path,
    languages: Mapping[str, Language],
    env: Optional[Mapping[str, str]] = None,
) -> list[str]:
    """
    Runner arguments for a multi-file solution: one --solution-file per key, plus the
    evaluator directory, the language family and the manager directory (all absolute).
    """
    spec = read_spec_info(spec_yml)
    evaluator = evaluator_for(spec, env)
    if not evaluator.custom_solution_keys:
        raise MultifileError(
            f"'{solution_dir}' is a directory, but evaluator '{evaluator.slug}' takes one solution file; "
            f"multi-file solutions need a functional evaluator")

    files = key_files(solution_dir, spec.solution_keys, languages)
    family = family_of(files, languages)
    args = [f'--solution-file={key}={path.resolve()}' for key, path in files.items()]
    args.append(f'--evaluator-dir={evaluator.directory.resolve()}')
    args.append(f'--solution-family={family}')
    args.append(f'--manager={(package_dir / MANAGER_DIR).resolve()}')
    return args
