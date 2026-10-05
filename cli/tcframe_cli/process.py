"""
Cross-platform command resolution for programs the CLI runs (solutions, scorers,
communicators, compiled spec runner). Ported from the removed tcframe-python/ layer (see git history).
"""

import os
import shlex
import shutil
import subprocess
import sys
from typing import Union

_IS_WINDOWS = sys.platform == 'win32'

# Characters that require a real shell to interpret the command
_SHELL_METACHARS = frozenset('|&;<>$`')


class CommandError(Exception):
    """Raised when a command's executable cannot be found or run."""


def _needs_shell(command: str) -> bool:
    return any(c in _SHELL_METACHARS for c in command)


def _split_command(command: str) -> list[str]:
    """Split a command line into argv, keeping Windows backslash paths intact."""
    if _IS_WINDOWS:
        parts = shlex.split(command, posix=False)
        return [p[1:-1] if len(p) >= 2 and p[0] == p[-1] and p[0] in '"\'' else p
                for p in parts]
    return shlex.split(command)


def _is_path(prog: str) -> bool:
    return '/' in prog or os.sep in prog or bool(os.altsep and os.altsep in prog)


def _executable_candidates(prog: str) -> list[str]:
    """Paths to try for a program path; on Windows './solution' also matches solution.exe."""
    candidates = [prog]
    if _IS_WINDOWS and not os.path.splitext(prog)[1]:
        exts = os.environ.get('PATHEXT', '.COM;.EXE;.BAT;.CMD').split(os.pathsep)
        candidates += [prog + ext.lower() for ext in exts if ext]
    return candidates


def _quote_arg(arg: str) -> str:
    return subprocess.list2cmdline([arg]) if _IS_WINDOWS else shlex.quote(arg)


def _strip_first_token(command: str) -> str:
    """Return `command` without its leading (possibly quoted) program token."""
    s = command.lstrip()
    if s[:1] in ('"', "'"):
        end = s.find(s[0], 1)
        return s[end + 1:].lstrip() if end != -1 else ''
    parts = s.split(None, 1)
    return parts[1] if len(parts) > 1 else ''


def resolve_command(command: str, what: str = 'solution') -> Union[list[str], str]:
    """
    Resolve `command` into something subprocess can run without a shell.

    Returns an argv list whose first element is an absolute executable path,
    or a string if the command needs a shell (pipes, redirects...).
    Raises CommandError with a human-readable message if the executable is missing.
    """
    try:
        parts = _split_command(command)
    except ValueError as exc:
        raise CommandError(f"cannot parse {what} command {command!r}: {exc}")
    if not parts:
        raise CommandError(f"{what} command is empty")

    resolved = _resolve_executable(parts[0], what)
    if _needs_shell(command):
        # Swap in the resolved path so every shell (sh, cmd.exe) runs the same program
        return f'{_quote_arg(resolved)} {_strip_first_token(command)}'.rstrip()
    return [resolved] + parts[1:]


def _resolve_executable(prog: str, what: str) -> str:
    if not _is_path(prog):
        found = shutil.which(prog)
        if found:
            return found
        msg = f"{what} command '{prog}' was not found on PATH."
        if any(os.path.isfile(c) for c in _executable_candidates(prog)):
            msg += f"\n    A file named '{prog}' exists here; use './{prog}' to run it."
        raise CommandError(msg)

    for path in _executable_candidates(prog):
        if os.path.isfile(path):
            if not _IS_WINDOWS and not os.access(path, os.X_OK):
                raise CommandError(f"{what} '{path}' exists but is not executable.\n"
                                   f"    Try: chmod +x {path}")
            return os.path.abspath(path)

    raise CommandError(_not_found_message(prog, what))


def _not_found_message(prog: str, what: str) -> str:
    msg = (f"{what} '{prog}' not found.\n"
           f"    looked for : {os.path.abspath(prog)}")
    if _IS_WINDOWS and not os.path.splitext(prog)[1]:
        msg += " (also tried .exe, .bat, .cmd, ...)"
    msg += f"\n    cwd        : {os.getcwd()}"

    stem = os.path.splitext(prog)[0]
    if not _IS_WINDOWS and os.path.isfile(stem + '.exe'):
        msg += (f"\n    '{stem}.exe' exists, but that is a Windows binary; "
                f"recompile it on this machine.")

    for src_ext in ('.cpp', '.cc', '.c'):
        if os.path.isfile(stem + src_ext):
            msg += (f"\n    Found '{stem + src_ext}'; compile it first, e.g.:"
                    f"\n        g++ -std=c++17 -O2 -o {stem} {stem + src_ext}")
            break
    else:
        msg += f"\n    Compile your {what} first, or pass --{what} <command>."
    return msg


def check_command(command: str, what: str = 'solution') -> str | None:
    """Return None if `command` can be run, else a human-readable error message."""
    try:
        resolve_command(command, what)
        return None
    except CommandError as exc:
        return str(exc)
