#include <iostream>
#include <git2.h>
using namespace std;

int main(){
    cout << "Welcome to GoodCommit" << endl;
    git_libgit2_init();
    const char* repo_path = "https://github.com/noranorab/mini-projet";

    git_repository* git_repo = nullptr;
    int error = git_repository_open(&git_repo, repo_path);

    if (error < 0)
    {
        cout << "Error opening the repository" << endl;
    }

    git_revwalk* walker = nullptr;
    git_revwalk_new(&walker, git_repo);
    git_revwalk_push_head(walker);

    git_oid commit_oid;
    while (git_revwalk_next(&commit_oid, walker)) 
    {
        git_commit* commit = nullptr;
        git_commit_lookup(&commit, git_repo, &commit_oid);

        cout << "Commit ID:" << git_oid_tostr_s(&commit_oid) <<endl;
        git_commit_free(commit);
    }

    
    
    git_revwalk_free(walker);
    git_repository_free(git_repo);
    git_libgit2_shutdown();
    return 0;
}