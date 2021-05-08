static int os_mkdir(const char* path, mode_t mode) {
    //find the parent directory root_dir 
    //create new directory
    //add new directory to the parent 
    dir_t* parent_ptr = (dir_t *)find_dir(root_dir, get_dirs_from_path(path));
    if (parent_ptr == NULL){
        return -ENOENT; //ERROR NO ENTITY 
    }
    
    char* newdir = (char *)get_name_from_path(path);
    dir_t* newdir_ptr = (dir_t *)create_dir(newdir);

    //Adds a directory to another given directory
    add_dir_to(parent_ptr,newdir_ptr);

    return 0;
}

/**
 * This function must be completed for the assignments!
 * 
 * rmdir() removes a directory (and its contents).
 * - path: path leading to the directory.
 * Returns 0, appropriate error otherwise.
 */
static int os_rmdir(const char* path) {
    //find a directory and remove it
    dir_t* remove_dir = (dir_t *)find_dir(root_dir,path);
    if (remove_dir == NULL){
        return -ENOENT;
    }

    destroy_dir(remove_dir);

    return 0;
}