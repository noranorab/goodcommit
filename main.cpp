#include <iostream>
#include <git2.h>
using namespace std;

int openGitRepo(git_repository **out, const char *path){
    const char* repo_path = path;
    int error = git_repository_open(out, repo_path);
    return error;
}

void handleError(int error, string message)
{
    if (error < 0){
        cout << message << endl;
    }
}
void initWalker(git_revwalk **out, git_repository *rep)
{
    git_revwalk_new(out, rep);
    git_revwalk_push_head(*out);
}

int main(){
    cout << "Welcome to GoodCommit" << endl;
    git_libgit2_init();

    git_repository* git_repo = nullptr;
    int error = openGitRepo(&git_repo, "/home/nora/Bureau/goodcommit/goodcommit");

    handleError(error, "Error opening the repository");

    
    git_revwalk* walker = nullptr;
    initWalker(&walker, git_repo);
    
    git_oid commit_oid;
    while (git_revwalk_next(&commit_oid, walker) == 0) 
    {
        git_commit* commit = nullptr;
        git_commit_lookup(&commit, git_repo, &commit_oid);

        cout << "Commit ID:" << git_oid_tostr_s(&commit_oid) <<endl;
        cout << "Date: " << git_commit_author(commit)->when.time << endl;
        git_commit_free(commit);
    }

    
    
    git_revwalk_free(walker);
    git_repository_free(git_repo);
    git_libgit2_shutdown();
    return 0;
}