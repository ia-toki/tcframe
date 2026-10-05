import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli.scaffold import create_package

REPO_ROOT = Path(__file__).resolve().parents[2]
TEMPLATES = ['batch', 'batch-subtasks', 'interactive', 'interactive-subtasks']


@pytest.mark.parametrize('name', TEMPLATES)
def test_shipped_template_is_valid_spec(tmp_path, name):
    env = {'TCFRAME_HOME': str(REPO_ROOT)}
    target = create_package(name, tmp_path / name, env=env, home=tmp_path / 'nohome')
    assert target.read_text() == (REPO_ROOT / 'templates' / f'{name}.cpp').read_text()

    gxx = shutil.which('g++')
    if gxx is None:
        pytest.skip('g++ not available')
    result = subprocess.run(
        [gxx, '-std=c++17', '-fsyntax-only', f'-I{REPO_ROOT / "include"}', str(target)],
        capture_output=True, text=True,
    )
    assert result.returncode == 0, result.stderr
