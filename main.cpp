#include <iostream>
#include <git2.h>
using namespace std;

void identifyCodeChanges(git_repository* repo)
{
    git_status_options git_status = GIT_STATUS_OPTIONS_INIT;
    git_status_list* git_status_list = nullptr;

    int error = git_status_list_new(&git_status_list, repo, &git_status);
    handleError(error, "Error getting status.");

    size_t entry_count = git_status_list_entrycount(git_status_list);

    if (entry_count > 0)
    {
        for (int i=0; i<entry_count; ++i)
        {
            const git_status_entry* entry = git_status_byindex(git_status_list, i);

            switch(entry->status) 
            {
                case GIT_STATUS_CURRENT :
                    cout << "Added: " << entry->index_to_workdir->new_file.path << endl;
                    break;
                case GIT_STATUS_INDEX_NEW :
                case GIT_STATUS_INDEX_MODIFIED :
                case GIT_STATUS_INDEX_DELETED :
                case GIT_STATUS_INDEX_TYPECHANGE :
                case GIT_STATUS_WT_NEW :
                case GIT_STATUS_WT_MODIFIED :
                case GIT_STATUS_WT_DELETED :
                case GIT_STATUS_WT_TYPECHANGE :
                case GIT_STATUS_WT_RENAMED :
                case GIT_STATUS_WT_UNREADABLE :
                case GIT_STATUS_IGNORED : 
                case GIT_STATUS_CONFLICTED :
            }
        }
    }
}

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