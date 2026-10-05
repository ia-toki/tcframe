"""
`tcframe new --template=[t]`: copy a template spec into a new problem package.
"""

import re
import shutil
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.paths import TEMPLATES, find_resource, search_roots, tcframe_home

SPEC_FILE = 'spec.cpp'
DEFAULT_TEMPLATE = 'batch'

_TEMPLATE_NAME = re.compile(r'[A-Za-z0-9_-]+')


class ScaffoldError(Exception):
    """Raised when a package cannot be scaffolded; the message is user-facing."""


def create_package(
    template: str,
    dest: Path,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> Path:
    """
    Write `dest/spec.cpp` from the named template and return its path.

    Refuses to overwrite an existing spec.cpp.
    """
    if not _TEMPLATE_NAME.fullmatch(template):
        raise ScaffoldError(f"invalid template name {template!r}; use letters, digits, '-' or '_'")

    src = find_resource(TEMPLATES, f'{template}.cpp', env=env, home=home)
    if src is None:
        searched = ', '.join(str(r / TEMPLATES / f'{template}.cpp')
                             for r in search_roots(TEMPLATES, env=env, home=home))
        msg = f"template '{template}' not found; searched: {searched}"
        if tcframe_home(env) is None:
            msg += '; TCFRAME_HOME is unset'
        raise ScaffoldError(msg)

    target = dest / SPEC_FILE
    if target.exists():
        raise ScaffoldError(f"'{target}' already exists; refusing to overwrite")

    dest.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src, target)
    return target
