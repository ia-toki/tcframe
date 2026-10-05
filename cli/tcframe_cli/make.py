"""
`tcframe make`: build the package, which leaves `spec.yml` next to `tc/` in
the build directory (the distribution artifacts).
"""

from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.build import DEFAULT_SOLUTION, build_package
from tcframe_cli.validate import ship_validator, validate_distribution


def make_package(
    package_dir: Path,
    build_dir: Optional[Path] = None,
    solution: str = DEFAULT_SOLUTION,
    env: Optional[Mapping[str, str]] = None,
) -> Path:
    """
    Build the package, ship its validator, and check every generated test case against it.
    Returns the build directory (spec, spec.yml, tc/, validator).
    """
    build_dir = build_package(package_dir, build_dir, solution=solution, env=env)
    validator = ship_validator(build_dir)
    validate_distribution(build_dir, validator)
    return build_dir

