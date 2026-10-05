import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli.build import BuildError
from tcframe_cli.validate import VALIDATOR_BINARY, ship_validator, validate_distribution

needs_sh = pytest.mark.skipif(shutil.which('sh') is None, reason='sh not available')


def script(path: Path, body: str) -> Path:
    path.write_text('#!/bin/sh\n' + body)
    path.chmod(0o755)
    return path


@pytest.fixture
def build(tmp_path):
    build_dir = tmp_path / 'build'
    (build_dir / 'tc').mkdir(parents=True)
    (build_dir / 'tc' / 'aplusb_1.in').write_text('1 2\n')
    (build_dir / 'tc' / 'aplusb_2.in').write_text('3 4\n')
    return build_dir


@needs_sh
def test_accepts_when_validator_accepts_every_input(build):
    validator = script(build / VALIDATOR_BINARY, 'exit 0\n')

    assert validate_distribution(build, validator) is True


@needs_sh
def test_rejects_first_invalid_input_with_its_stderr(build):
    validator = script(build / VALIDATOR_BINARY, 'read x\ncase "$x" in 3*) echo "bad input" >&2; exit 1;; esac\nexit 0\n')

    with pytest.raises(BuildError, match=r"validator rejected 'aplusb_2.in' \(exit 1\):\nbad input"):
        validate_distribution(build, validator)


@needs_sh
def test_unsupported_spec_is_skipped(build):
    validator = script(build / VALIDATOR_BINARY, 'exit 2\n')

    assert validate_distribution(build, validator) is False


@needs_sh
def test_ship_validator_copies_spec_binary(build):
    spec = script(build / 'spec', 'exit 0\n')

    validator = ship_validator(build)

    assert validator == build / VALIDATOR_BINARY
    assert validator.read_bytes() == spec.read_bytes()
    assert subprocess.run([str(validator)]).returncode == 0
