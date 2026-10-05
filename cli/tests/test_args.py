from tcframe_cli.args import parse_args


def test_defaults():
    args = parse_args([])
    assert args.command == 'generate'
    assert args.solution == './solution'
    assert args.output_dir == 'tc'
    assert args.scorer == './scorer'
    assert args.communicator == './communicator'
    assert args.time_limit is None
    assert args.memory_limit is None
    assert args.brief is False


def test_grade_with_flags():
    args = parse_args([
        'grade', '--solution', './sol', '--output', 'out', '--seed', '42',
        '--time-limit', '3', '--memory-limit', '256', '--brief',
        '--scorer', './sc', '--communicator', './comm',
    ])
    assert args.command == 'grade'
    assert args.solution == './sol'
    assert args.output_dir == 'out'
    assert args.seed == 42
    assert args.time_limit == 3
    assert args.memory_limit == 256
    assert args.brief is True
    assert args.scorer == './sc'
    assert args.communicator == './comm'


def test_single_dash_flags_accepted():
    args = parse_args(['grade', '-solution', './sol', '-no-time-limit', '-no-memory-limit'])
    assert args.solution == './sol'
    assert args.no_time_limit is True
    assert args.no_memory_limit is True


def test_unknown_flags_ignored():
    args = parse_args(['--whatever', 'x', '--seed', '7'])
    assert args.seed == 7
