"""
`tcframe version`: the 1.x wrapper's git-derived version string, falling back
to the package version outside a tagged git checkout.
"""

import subprocess
from pathlib import Path

from tcframe_cli import __version__


def _git(cwd: Path, *args: str) -> str | None:
    try:
        result = subprocess.run(['git', *args], cwd=cwd, capture_output=True, text=True)
    except FileNotFoundError:
        return None
    return result.stdout.strip() if result.returncode == 0 else None


def version_string(cwd: Path) -> str:
    """`<tag>[-<commits-ahead>-g<hash>][-dirty]`, or the package version if no tag is found."""
    tag = _git(cwd, 'describe', '--tags', '--abbrev=0')
    if not tag:
        return __version__
    tag = tag.removeprefix('v')

    ahead = _git(cwd, 'rev-list', f'v{tag}..HEAD', '--count') or '0'
    commit = _git(cwd, 'rev-parse', '--short', 'HEAD') or ''
    dirty = '-dirty' if _git(cwd, 'status', '--porcelain') else ''

    if int(ahead) > 0:
        return f'{tag}-{ahead}-g{commit}{dirty}'
    return f'{tag}{dirty}'
