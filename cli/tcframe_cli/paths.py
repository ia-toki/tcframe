"""
TCFRAME_HOME / ~/.tcframe resolution and config lookup precedence.

Search order per resource follows the 2.0 RFC:
  templates  ~/.tcframe/templates/[t].cpp  ->  $TCFRAME_HOME/templates/[t].cpp
  languages  [pkg]/languages/x.yml         ->  ~/.tcframe/languages/x.yml  ->  $TCFRAME_HOME/languages/x.yml
  registry   $TCFRAME_HOME/registry        ->  ~/.tcframe/registry
"""

import os
from pathlib import Path
from typing import Mapping, Optional

USER_DIR_NAME = '.tcframe'
ENV_TCFRAME_HOME = 'TCFRAME_HOME'

TEMPLATES = 'templates'
LANGUAGES = 'languages'
REGISTRY = 'registry'

# Base roots per resource, in priority order. 'package' is the problem package
# directory (only meaningful for languages); 'user' is ~/.tcframe; 'home' is
# $TCFRAME_HOME.
_ORDER = {
    TEMPLATES: ('user', 'home'),
    LANGUAGES: ('package', 'user', 'home'),
    REGISTRY: ('home', 'user'),
}


def tcframe_home(env: Optional[Mapping[str, str]] = None) -> Optional[Path]:
    """Return $TCFRAME_HOME as a Path, or None when unset or empty."""
    if env is None:
        env = os.environ
    value = env.get(ENV_TCFRAME_HOME, '')
    return Path(value) if value else None


def user_home(home: Optional[Path] = None) -> Path:
    """Return ~/.tcframe (the per-user tcframe directory)."""
    return (home if home is not None else Path.home()) / USER_DIR_NAME


def search_roots(
    resource: str,
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> list[Path]:
    """
    Base directories to search for `resource`, highest priority first.

    Each entry is a base directory; the resource lives at `<base>/<resource>/...`
    (for the package root, `<package>/languages/...`). Unset roots (no package
    dir, no TCFRAME_HOME) are skipped.
    """
    if resource not in _ORDER:
        raise ValueError(f"unknown resource {resource!r}")

    roots = []
    for kind in _ORDER[resource]:
        if kind == 'package':
            if package_dir is not None:
                roots.append(Path(package_dir))
        elif kind == 'user':
            roots.append(user_home(home))
        elif kind == 'home':
            base = tcframe_home(env)
            if base is not None:
                roots.append(base)
    return roots


def find_resource(
    resource: str,
    relpath: str,
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> Optional[Path]:
    """
    Return the first existing `<base>/<resource>/<relpath>` (or
    `<package>/languages/<relpath>` for languages), or None if nothing matches.
    """
    for base in search_roots(resource, package_dir, env, home):
        candidate = base / resource / relpath
        if candidate.exists():
            return candidate
    return None
