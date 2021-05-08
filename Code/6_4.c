static int os_create(const char * path, mode_t mode, struct fuse_file_info* fi) {
    //find the parent directory
    //create a file
    //add it to the directory
    dir_t * parent_ptr;
    parent_ptr = (dir_t *)find_dir(root_dir, get_dirs_from_path(path));
    if (parent_ptr == NULL)
    {
        return -ENOENT;
    }
    file_t * new_file_ptr;
    new_file_ptr = (file_t *)create_file(get_name_from_path(path)," ");
    add_file_to(parent_ptr,new_file_ptr);
    return 0;
}

/**
 * This function must be completed for the assignments!
 * 
 * unlink() unlinks (deletes) a file.
 * - path: path leading to the file
 * Returns 0 if successful, appropriate error otherwise.
 */
static int os_unlink(const char* path) {
    //find the file
    //remove it
    file_t * remove_file = find_file(root_dir,path);
    if (remove_file == NULL){
        return -ENOENT;
    }
    destroy_file(remove_file);
    return 0;
}
