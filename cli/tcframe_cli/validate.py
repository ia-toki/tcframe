"""
Distribution validation (SPEC.md T7.3): ship the spec binary's `validate` command as
`validator` in the build directory, and check that the validator accepts every
generated test case input.

Exit codes of `validator` (SPEC.md T7.1, T7.3): 0 valid, 1 invalid input, 2 the spec
cannot be validated (multiple test cases). The last case is skipped, not an error.
"""

import shutil
import subprocess
from pathlib import Path
from typing import Optional

from tcframe_cli.build import BuildError, SPEC_BINARY, TC_DIR

VALIDATOR_BINARY = 'validator'
VALIDATE_COMMAND = 'validate'
EXIT_UNSUPPORTED = 2


def ship_validator(build_dir: Path) -> Path:
    """Copy the compiled spec binary to `build_dir/validator` and return the new path."""
    validator = build_dir / VALIDATOR_BINARY
    shutil.copy2(build_dir / SPEC_BINARY, validator)
    return validator


def validate_distribution(build_dir: Path, validator: Optional[Path] = None) -> bool:
    """
    Run the validator over every `tc/*.in`. Raises BuildError on the first rejected input.
    Returns False when the spec does not support validation (skipped), True otherwise.
    """
    validator = validator if validator is not None else build_dir / VALIDATOR_BINARY
    inputs = sorted((build_dir / TC_DIR).glob('*.in'))
    for tc_input in inputs:
        with tc_input.open('rb') as stdin:
            result = subprocess.run([str(validator.resolve()), VALIDATE_COMMAND], stdin=stdin, capture_output=True, text=True)
        if result.returncode == EXIT_UNSUPPORTED:
            return False
        if result.returncode != 0:
            detail = result.stderr.strip()
            raise BuildError(f"validator rejected '{tc_input.name}' (exit {result.returncode})"
                             + (f':\n{detail}' if detail else ''))
    return True
