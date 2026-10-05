import argparse
import sys
import time
from dataclasses import dataclass, field
from typing import Optional


def _default_seed() -> int:
    return int(time.time() * 1000) & 0x7FFFFFFF


@dataclass
class Args:
    command: str = 'generate'       # 'generate' or 'grade'
    solution: str = './solution'
    output_dir: str = 'tc'
    seed: int = field(default_factory=_default_seed)
    time_limit: Optional[int] = None
    no_time_limit: bool = False
    memory_limit: Optional[int] = None
    no_memory_limit: bool = False
    brief: bool = False
    scorer: str = './scorer'
    communicator: str = './communicator'


def parse_args(argv=None) -> Args:
    if argv is None:
        argv = sys.argv[1:]

    args = Args()

    if argv and argv[0] == 'grade':
        args.command = 'grade'
        argv = argv[1:]

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--solution', '-solution', default=None)
    parser.add_argument('--output', '-output', default=None)
    parser.add_argument('--seed', '-seed', type=int, default=None)
    parser.add_argument('--time-limit', '-time-limit', type=int, default=None, dest='time_limit')
    parser.add_argument('--no-time-limit', '-no-time-limit', action='store_true', dest='no_time_limit')
    parser.add_argument('--memory-limit', '-memory-limit', type=int, default=None, dest='memory_limit')
    parser.add_argument('--no-memory-limit', '-no-memory-limit', action='store_true', dest='no_memory_limit')
    parser.add_argument('--brief', '-brief', action='store_true')
    parser.add_argument('--scorer', '-scorer', default=None)
    parser.add_argument('--communicator', '-communicator', default=None)

    parsed, _ = parser.parse_known_args(argv)

    if parsed.solution is not None:
        args.solution = parsed.solution
    if parsed.output is not None:
        args.output_dir = parsed.output
    if parsed.seed is not None:
        args.seed = parsed.seed
    if parsed.time_limit is not None:
        args.time_limit = parsed.time_limit
    args.no_time_limit = parsed.no_time_limit
    if parsed.memory_limit is not None:
        args.memory_limit = parsed.memory_limit
    args.no_memory_limit = parsed.no_memory_limit
    args.brief = parsed.brief
    if parsed.scorer is not None:
        args.scorer = parsed.scorer
    if parsed.communicator is not None:
        args.communicator = parsed.communicator

    return args
