static int os_readdir(const char *path, void *buff, fuse_fill_dir_t fill, off_t off, struct fuse_file_info *fi) {
    // Example: this is how you find the required directory.
    // This could be NULL though! You should return an error if this is so, in other functions as well
    dir_t* dir = find_dir(root_dir, path);
    if (dir == NULL) {
        return -ENOENT; // Error: no entity
    }

    // Example: this is how you add a folder "Documents" to the buffer
    // although this "Documents" folder does not exist.
    fill(buff, ".", NULL, 0);
    fill(buff, "..", NULL, 0);


    // Example: this is how you could iterate over all files in dir
    // and it's almost the same for directories
    for (unsigned int i = 0; i < dir->files->length; i++) {
        file_t* ptr = (file_t *) get_p(dir->files, i);
        fill(buff, ptr->name, NULL, 0);
        // Do something with this file
    }
    for (unsigned int i = 0; i < dir->dirs->length; i++){
        dir_t* ptr = (dir_t *) get_p(dir->dirs, i);
        fill(buff,ptr->name,NULL,0);
    }
    // Returning 0 means all-ok.
    return 0;

}