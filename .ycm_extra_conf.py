import os
import json

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
DEPENDS_PATH = os.path.join(SCRIPT_DIR, '.depends.json')

FLAGS = [
    '-Wall',
    '-Wextra',
    '-x', 'c++',
    '-std=c++17',

    '-isystem', '/usr/include/c++/8.2.1',
    '-isystem', '/usr/include/c++/8.2.1/x86_64-pc-linux-gnu',
    '-isystem', '/usr/include/c++/8.2.1/backward',
    '-isystem', '/usr/include/',
]

with open(DEPENDS_PATH) as f:
    DEPENDS = json.load(f)


def project_include_dir(project):
    return os.path.join(SCRIPT_DIR, 'src', project, 'include')


def get_project(file_path):
    src_path = os.path.join(SCRIPT_DIR, 'src')
    rel_to_src = os.path.relpath(file_path, src_path)
    return rel_to_src.split(os.path.sep)[0]


def Settings(**kwargs):
    if kwargs['language'] == 'python':
        return {}

    file_path = kwargs['filename']
    project = get_project(file_path)
    depends = DEPENDS[project]

    flags = FLAGS[:]
    for p in [project] + depends:
        flags.extend(['-I', project_include_dir(p)])

    return {'flags': flags}
