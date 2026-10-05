"""
Evaluator directories and their helpers (RFC 2.0 "Evaluator", SPEC.md T6.3).

An evaluator is `registry/evaluators/<slug>/` with an `evaluator.yml`:

  custom_solution_keys: false
  tc_output: not_required        required | not_required | optional (default)
  helpers:
  - { slug: communicator, type: program, optional: false }

Helper types:
- program: a package program named `<slug>` (a directory) or `<slug>.<ext>` (a file).
  Compiled once before grading. If it is missing and the helper is optional, the
  registry default is used: `helpers/<slug>/defaults.yml` names it (`slug: compare`),
  and the program is `helpers/<slug>/<name>/`.
- files: the package directory `<slug>/` (e.g. `manager/` for Functional). It has no
  registry default.

A required helper that is not found is an error; an optional one with no default is
left out.
"""

from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from .paths import find_resource
from .simple_yaml import Map, YamlError, parse

EVALUATOR_FILE = 'evaluator.yml'
TC_OUTPUT_VALUES = ('required', 'not_required', 'optional')
HELPER_TYPES = ('program', 'files')


class EvaluatorError(Exception):
    """Raised for a malformed evaluator.yml or an unresolvable helper."""


@dataclass(frozen=True)
class HelperSpec:
    slug: str
    type: str
    optional: bool


@dataclass(frozen=True)
class EvaluatorConfig:
    slug: str
    directory: Path
    custom_solution_keys: bool
    tc_output: str
    helpers: tuple[HelperSpec, ...]


@dataclass(frozen=True)
class ResolvedHelper:
    spec: HelperSpec
    path: Path
    source: str  # 'package' or 'registry'


def _bool(config: Map, key: str, default: bool) -> bool:
    if key not in config:
        return default
    value = config[key]
    if value not in ('true', 'false'):
        raise EvaluatorError(f'{key} must be true or false, got {value!r}')
    return value == 'true'


def _helper(item) -> HelperSpec:
    if not isinstance(item, dict):
        raise EvaluatorError(f'helper must be a flow map like {{ slug: x, type: program }}, got {item!r}')
    for key in ('slug', 'type'):
        if key not in item:
            raise EvaluatorError(f'helper is missing {key!r}: {item!r}')
    if item['type'] not in HELPER_TYPES:
        raise EvaluatorError(f'helper type must be program or files, got {item["type"]!r}')
    return HelperSpec(
        slug=item['slug'],
        type=item['type'],
        optional=_bool(item, 'optional', False),
    )


def load_evaluator(directory: Path) -> EvaluatorConfig:
    """Read `<directory>/evaluator.yml`. The directory name is the evaluator slug."""
    directory = Path(directory)
    path = directory / EVALUATOR_FILE
    try:
        config = parse(path.read_text())
    except OSError as err:
        raise EvaluatorError(f'cannot read {path}: {err}') from err
    except YamlError as err:
        raise EvaluatorError(f'{path}: {err}') from err

    tc_output = config.get('tc_output', 'optional')
    if tc_output not in TC_OUTPUT_VALUES:
        raise EvaluatorError(f'{path}: tc_output must be one of {", ".join(TC_OUTPUT_VALUES)}, got {tc_output!r}')

    helpers = config.get('helpers', [])
    if helpers == '':  # `helpers:` with no items
        helpers = []
    if not isinstance(helpers, list):
        raise EvaluatorError(f'{path}: helpers must be a list')

    return EvaluatorConfig(
        slug=directory.name,
        directory=directory,
        custom_solution_keys=_bool(config, 'custom_solution_keys', False),
        tc_output=tc_output,
        helpers=tuple(_helper(item) for item in helpers),
    )


def find_evaluator(slug: str, env: Optional[Mapping[str, str]] = None, home: Optional[Path] = None) -> Optional[EvaluatorConfig]:
    """Find `evaluators/<slug>/` in the registry roots ($TCFRAME_HOME, then ~/.tcframe)."""
    found = find_resource('registry', f'evaluators/{slug}/{EVALUATOR_FILE}', env=env, home=home)
    if found is None:
        return None
    return load_evaluator(found.parent)


def resolve_helpers(config: EvaluatorConfig, package_dir: Path) -> dict[str, ResolvedHelper]:
    """
    Map each helper slug to the package program/files, or to the registry default.
    Required helpers must resolve; optional ones may be missing from the result.
    """
    package_dir = Path(package_dir)
    resolved = {}
    for spec in config.helpers:
        found = _find_in_package(spec, package_dir)
        if found is not None:
            resolved[spec.slug] = ResolvedHelper(spec, found, 'package')
            continue

        if spec.type == 'program':
            default = _registry_default(spec, config)
            if default is not None:
                resolved[spec.slug] = ResolvedHelper(spec, default, 'registry')
                continue

        if not spec.optional:
            raise EvaluatorError(f'evaluator {config.slug!r} requires helper {spec.slug!r} ({spec.type}) in the package')
    return resolved


def _find_in_package(spec: HelperSpec, package_dir: Path) -> Optional[Path]:
    if spec.type == 'files':
        candidate = package_dir / spec.slug
        if candidate.exists() and not candidate.is_dir():
            raise EvaluatorError(f'helper {spec.slug!r} must be a directory, found a file')
        return candidate if candidate.is_dir() else None

    candidates = []
    directory = package_dir / spec.slug
    if directory.is_dir():
        candidates.append(directory)
    candidates.extend(sorted(p for p in package_dir.glob(f'{spec.slug}.*') if p.is_file()))
    if len(candidates) > 1:
        names = ', '.join(p.name for p in candidates)
        raise EvaluatorError(f'helper {spec.slug!r} is ambiguous in the package: {names}')
    return candidates[0] if candidates else None


def _registry_default(spec: HelperSpec, config: EvaluatorConfig) -> Optional[Path]:
    # The registry root is <root>/evaluators/<slug>/, so the helpers live at <root>/helpers/.
    registry_root = config.directory.parent.parent
    defaults = registry_root / 'helpers' / spec.slug / 'defaults.yml'
    if not defaults.is_file():
        return None
    try:
        name = parse(defaults.read_text()).get('slug', '')
    except YamlError as err:
        raise EvaluatorError(f'{defaults}: {err}') from err
    if not name:
        return None
    program = registry_root / 'helpers' / spec.slug / name
    return program if program.is_dir() else None
