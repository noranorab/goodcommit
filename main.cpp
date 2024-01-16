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


void out(string message, string entryChange)
{
    cout << message << entryChange << endl;
}

void identifyCodeChanges(git_repository* repo)
{
    git_status_options git_status = GIT_STATUS_OPTIONS_INIT;
    git_status_list* git_status_list = nullptr;

    int error = git_status_list_new(&git_status_list, repo, &git_status);
    handleError(error, "Error getting status.");
    
    size_t entry_count = git_status_list_entrycount(git_status_list);
    cout << entry_count <<endl;

    if (entry_count > 0)
    {
        for (size_t i=0; i<entry_count; ++i)
        {
            const git_status_entry* entry = git_status_byindex(git_status_list, i);
            
            switch(entry->status) 
            {
                case GIT_STATUS_CURRENT :
                    out("Added: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_INDEX_NEW :
                    out("Added to index: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_INDEX_MODIFIED :
                    out("Modified in index: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_INDEX_DELETED :
                    out("Deleted from index: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_INDEX_TYPECHANGE :
                    out("Type change in index: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_NEW :
                    out("Untracked in working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_MODIFIED :
                    out("Modified in working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_DELETED :
                    out("Deleted from working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_TYPECHANGE :
                    out("Type change in working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_RENAMED :
                    out("Renamed in working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_WT_UNREADABLE :
                    out("Unreadable in working directory: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_IGNORED : 
                    out("Ignored: ", entry->index_to_workdir->new_file.path);
                    break;
                case GIT_STATUS_CONFLICTED :
                    out("Conflicted: ", entry->index_to_workdir->new_file.path);
                    break;
                default:
                    out("Unknown status for file: ",entry->index_to_workdir->new_file.path);
                    break;
            }
        }
    } else {
        cout <<"No changes identified" <<endl;
    }

    git_status_list_free(git_status_list);
}

int main(){
    cout << "Welcome to GoodCommit" << endl;
    git_libgit2_init();

    git_repository* git_repo = nullptr;
    int error = openGitRepo(&git_repo, "/home/nora/Bureau/goodcommit/goodcommit");

    handleError(error, "Error opening the repository");

    identifyCodeChanges(git_repo);
    
    
    git_repository_free(git_repo);
    git_libgit2_shutdown();
    return 0;
}