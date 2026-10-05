import os
import stat

import pytest

from tcframe_cli.process import CommandError, check_command, resolve_command


def _make_exec(path, body='#!/bin/sh\nexit 0\n'):
    path.write_text(body)
    path.chmod(path.stat().st_mode | stat.S_IXUSR)
    return path


def test_resolves_relative_path_to_absolute(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    _make_exec(tmp_path / 'solution')

    argv = resolve_command('./solution', 'solution')

    assert argv == [os.path.abspath(tmp_path / 'solution')]


def test_keeps_extra_arguments(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    _make_exec(tmp_path / 'scorer')

    argv = resolve_command('./scorer --flag 1', 'scorer')

    assert argv[0] == os.path.abspath(tmp_path / 'scorer')
    assert argv[1:] == ['--flag', '1']


def test_shell_command_returns_string(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    _make_exec(tmp_path / 'solution')

    resolved = resolve_command('./solution | cat', 'solution')

    assert isinstance(resolved, str)
    assert resolved.startswith(os.path.abspath(tmp_path / 'solution'))
    assert resolved.endswith('| cat')


def test_missing_executable_reports_compile_hint(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    (tmp_path / 'solution.cpp').write_text('int main() {}')

    with pytest.raises(CommandError) as exc:
        resolve_command('./solution', 'solution')

    message = str(exc.value)
    assert "solution './solution' not found" in message
    assert 'solution.cpp' in message
    assert 'g++ -std=c++17' in message


def test_non_executable_file_reports_chmod_hint(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    (tmp_path / 'solution').write_text('x')
    (tmp_path / 'solution').chmod(0o644)

    with pytest.raises(CommandError) as exc:
        resolve_command('./solution', 'solution')

    assert 'not executable' in str(exc.value)
    assert 'chmod +x' in str(exc.value)


def test_bare_name_not_on_path(monkeypatch):
    monkeypatch.setenv('PATH', '')

    with pytest.raises(CommandError) as exc:
        resolve_command('definitely-not-a-real-program-xyz', 'scorer')

    assert "scorer command 'definitely-not-a-real-program-xyz' was not found on PATH" in str(exc.value)


def test_empty_command_rejected():
    with pytest.raises(CommandError, match='empty'):
        resolve_command('   ', 'solution')


def test_check_command_none_when_runnable(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    _make_exec(tmp_path / 'solution')

    assert check_command('./solution') is None


def test_check_command_returns_message_when_missing(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)

    message = check_command('./solution')

    assert message is not None
    assert 'not found' in message
