import re
import json

def custom_json_dump(obj, indent=2):
    def serialize(obj, indent_level=0):
        ind = ' ' * indent * indent_level
        if (isinstance(obj, list)) and all(isinstance(i, (int, float, str)) for i in obj):
            return '[' + ', '.join(json.dumps(i) for i in obj) + ']'
        elif isinstance(obj, dict):
            items = [f'"{k}": {serialize(v, indent_level+1)}' for k, v in obj.items()]
            return '{\n' + ',\n'.join(ind + '  ' + item for item in items) + '\n' + ind + '}'
        elif isinstance(obj, list):
            items = [f'{serialize(v, indent_level+1)}' for v in obj]
            return '[\n' + ',\n'.join(ind + '  ' + item for item in items) + '\n' + ind + ']'
        else:
            return json.dumps(obj)
    return serialize(obj)


def read_spec_file(spec_file_path):
    with open(spec_file_path, 'r') as file:
        return file.read()

def matcher_extract(matches):
    subtask_match_dict = dict()
    for num, sub in matches:
        content = []
        try:
            content = list(map(int, sub.replace(' ', '').replace('{', '').replace('}', '').split(',')))
        except Exception as e:
            content = []
        subtask_match_dict[int(num)] = content

    return subtask_match_dict

def extractor(spec_content, front, back):
    matches = re.findall(front + r'(\d+)[\s\S.]+?' + back + r'\((.*?)\)', spec_content)
    return matcher_extract(matches)

def extract_samples(spec_content):
    return extractor(spec_content, 'SampleTestCase', 'Subtasks')

def extract_test_groups(spec_content):
    return extractor(spec_content, 'TestGroup', 'Subtasks')

def extract_points(spec_content):
    return extractor(spec_content, 'Subtask', 'Points')

def extract_time_limit(spec_content):
    time_limit = re.search(r'TimeLimit\((\d+)\);', spec_content)
    return int(time_limit.group(1)) if time_limit else 0

def extract_memory_limit(spec_content):
    memory_limit = re.search(r'MemoryLimit\((\d+)\);', spec_content)
    return int(memory_limit.group(1)) if memory_limit else 0

def extract_interactive(spec_content):
    is_interactive = re.search(r'InteractiveEvaluator', spec_content)
    return True if is_interactive else False


def generate_subtask_dict(spec_file_path):
    spec_content = read_spec_file(spec_file_path)
    points = extract_points(spec_content)
    test_groups = extract_test_groups(spec_content)
    samples = extract_samples(spec_content)
    subtask_dict = {
        "time_limit": extract_time_limit(spec_content),
        "memory_limit": extract_memory_limit(spec_content),
        "samples": [None]*max(samples.keys()),
        "test_groups": [None]*max(test_groups.keys()),
        "points": [0]*max(points.keys()),
        "interactive": extract_interactive(spec_content)
    }

    for i, test_group in enumerate(test_groups, start=1):
        subtask_dict["test_groups"][test_group-1] = test_groups[test_group]

    for i, sample in enumerate(samples, start=1):
        subtask_dict["samples"][sample-1] = samples[sample]

    for i, point in enumerate(points, start=1):
        subtask_dict["points"][point-1] = points[point][0]

    return subtask_dict

if __name__ == "__main__":
    spec_file_path = "spec.cpp"  # replace with your file path
    subtask_dict = generate_subtask_dict(spec_file_path)
    with open('config.json', 'w') as file:
        print(custom_json_dump(subtask_dict, 2), file=file)
