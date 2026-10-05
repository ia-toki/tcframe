from tcframe_cli import __version__
from tcframe_cli.cli import main


def test_version_prints_version(capsys):
    assert main(['version']) == 0
    assert capsys.readouterr().out.strip() == __version__


def test_no_args_prints_usage_and_fails(capsys):
    assert main([]) == 1
    assert 'usage: tcframe' in capsys.readouterr().err


def test_unknown_command_fails(capsys):
    assert main(['nope']) == 1
    assert 'usage: tcframe' in capsys.readouterr().err


def test_new_creates_spec_in_cwd(tmp_path, monkeypatch, capsys):
    tc_home = tmp_path / 'tc'
    (tc_home / 'templates').mkdir(parents=True)
    (tc_home / 'templates' / 'batch.cpp').write_text('// batch\n')
    work = tmp_path / 'problem'
    work.mkdir()
    monkeypatch.chdir(work)
    monkeypatch.setenv('TCFRAME_HOME', str(tc_home))
    monkeypatch.setenv('HOME', str(tmp_path / 'home'))

    assert main(['new']) == 0

    assert (work / 'spec.cpp').read_text() == '// batch\n'
    assert 'created' in capsys.readouterr().out


def test_new_accepts_template_flag(tmp_path, monkeypatch):
    tc_home = tmp_path / 'tc'
    (tc_home / 'templates').mkdir(parents=True)
    (tc_home / 'templates' / 'interactive.cpp').write_text('// interactive\n')
    work = tmp_path / 'problem'
    work.mkdir()
    monkeypatch.chdir(work)
    monkeypatch.setenv('TCFRAME_HOME', str(tc_home))
    monkeypatch.setenv('HOME', str(tmp_path / 'home'))

    assert main(['new', '--template=interactive']) == 0
    assert (work / 'spec.cpp').read_text() == '// interactive\n'


def test_new_reports_error_and_fails(tmp_path, monkeypatch, capsys):
    work = tmp_path / 'problem'
    work.mkdir()
    monkeypatch.chdir(work)
    monkeypatch.delenv('TCFRAME_HOME', raising=False)
    monkeypatch.setenv('HOME', str(tmp_path / 'home'))

    assert main(['new', '--template=missing']) == 1
    assert 'tcframe: new error' in capsys.readouterr().err
    assert not (work / 'spec.cpp').exists()
