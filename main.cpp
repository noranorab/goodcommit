#include <iostream>
#include <git2.h>
#include <vector>
#include <clang/14/include/clang-c/clang-c/Index.h>
#include <clang/14/include/clang-c/clang-c/CXString.h>
using namespace std;

ostream& operator<<(ostream& stream, const CXString& str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
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
        cout <<"Exited with code: " << error <<endl;
        exit(EXIT_FAILURE);
    
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
    git_status.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    git_status.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED;

    git_status_list* git_status_list = nullptr;

    int error = git_status_list_new(&git_status_list, repo, &git_status);
    handleError(error, "Error getting status.");
    
    size_t entry_count = git_status_list_entrycount(git_status_list);
    cout <<entry_count<<endl;

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

int print_patch_line(
    const git_diff_delta *delta,       // Delta information
    const git_diff_hunk *hunk ,         // Hunk information
    const git_diff_line *line,         // Line information
    void *payload                // Custom payload (can be nullptr)
) {
    // Example: Print the content of each line

    
    std::cout << "Line Status: ";
    
    switch (line->origin) {
        case GIT_DIFF_LINE_ADDITION:
            std::cout << "Added";
            break;
        case GIT_DIFF_LINE_DELETION:
            std::cout << "Deleted";
            break;
        default:
            std::cout << "Modified";
    }

    std::cout << ", Content: " << line->content << std::endl;
    // Return 0 to continue processing lines
    return 0;
}

void print_diff(git_diff *diff)
{
    size_t nums_deltas = git_diff_num_deltas(diff);
    cout << "Number of Deltas: " << nums_deltas << endl;

    for (size_t i=0; i < nums_deltas; ++i)
    {
        const git_diff_delta *delta = git_diff_get_delta(diff, i);
        out("File : ", delta->new_file.path);

        git_patch *patch = nullptr;

        if (git_patch_from_diff(&patch, diff, i) == GIT_OK)
        {
            cout << "--------------------------------------------" << endl;
            git_patch_print(patch, print_patch_line, nullptr);
            git_patch_free(patch);
        }
        std::cout << std::endl;
    }
}
void tokenizeCode() {
    CXIndex index = clang_createIndex(1, 1);
    CXTranslationUnit unit = clang_parseTranslationUnit(
    index,
    "/home/nora/Bureau/goodcommit/goodcommit/main.cpp", nullptr, 0,
    nullptr, 0,
    CXTranslationUnit_None);
    cout << unit <<endl;
    if (unit == nullptr)
    {
        cerr << "Unable to parse translation unit. Quitting." << endl;
        exit(-1);
    }
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
        cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
            << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
        return CXChildVisit_Recurse;
        },
        nullptr);


    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}


int main(){
    cout << "Welcome to GoodCommit" << endl;
    tokenizeCode();
    // git_libgit2_init();

    // git_repository* git_repo = nullptr;
    // int error = openGitRepo(&git_repo, "/home/nora/Bureau/goodcommit/goodcommit");

    // handleError(error, "Error opening the repository");
    
    // identifyCodeChanges(git_repo);
    
    // //Get the HEAD commit
    // git_oid head_oid;
    // git_reference_name_to_id(&head_oid, git_repo, "HEAD");
    

    // git_commit *head_commit = nullptr;
    // git_commit_lookup(&head_commit, git_repo, &head_oid);
    // git_tree *head_tree;
    // git_commit_tree(&head_tree, head_commit);

    // git_index *index = nullptr;
    // git_repository_index(&index, git_repo);

    // git_diff *diff = nullptr;
    // git_diff_index_to_workdir(&diff, git_repo, index, nullptr);

    
    // print_diff(diff);
    // cout << "i am here " << endl;

    // git_diff_free(diff);
    // git_index_free(index);
    // git_tree_free(head_tree);
    // git_commit_free(head_commit);
    // git_repository_free(git_repo);
    // git_libgit2_shutdown();
    return 0;
}