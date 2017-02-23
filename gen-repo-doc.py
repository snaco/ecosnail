#!/usr/bin/env python

import os, argparse, git

ECOSNAIL_WIKI_REPO = 'git@bitbucket.org:ecosnail/ecosnail.git/wiki'
DEFAULT_WIKI_REPO_PATH = '__wiki'

if __name__ == '__main__':
    # Parse command line
    parser = argparse.ArgumentParser(
        description='Update ecosnail wiki with project dependency info')
    parser.add_argument('--repo-path', dest='repo_path', metavar='PATH',
        default=DEFAULT_WIKI_REPO_PATH,
        help='Temporary path for wiki repository')
    args = parser.parse_args()

    deps = {}

    script_dir = os.path.dirname(os.path.realpath(__file__))

    ecosnail_repo = git.Repo.init(script_dir)

    for submodule in ecosnail_repo.submodules:
        name = submodule.name
        project_repo = git.Repo.init(os.path.join(script_dir, name))

        deps[name] = []
        for project_submodule in project_repo.submodules:
            inner_name = project_submodule.name
            deps[name].append(inner_name)

    print deps

    exit(0)


    wiki_repo = git.Repo.init(args.repo_path)
    origin = wiki_repo.create_remote('origin', wiki_repo.remotes.origin.url)
    assert origin.exists()
    assert origin == wiki_repo.remotes.origin == wiki_repo.remotes['origin']
    origin.fetch()

    wiki_repo.create_head('master', origin.refs.master)
    wiki_repo.heads.master.set_tracking_branch(origin.refs.master)
    wiki_repo.heads.master.checkout()

    # Or just:
    #wiki_repo.create_head('master', origin.refs.master).set_tracking_branch(origin.refs.master).checkout()
