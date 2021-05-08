static int os_read(const char* path, char* buff, size_t size, off_t off, struct fuse_file_info* fi) {
    //find file
    file_t* read_ptr = (file_t *)find_file(root_dir,path);
    if ( read_ptr == NULL){
        return -ENOENT;
    }
    int length = strlen(read_ptr->contents);
    int ret_val;
    if ( length <= off)
    {
        ret_val = 0;
        memcpy(buff, read_ptr->contents+off, ret_val);
    }
    else if ( length > off && length <= size )
    {
        ret_val = length;
        memcpy(buff, read_ptr->contents+off, ret_val);
    }
    else 
    /*(length > off && length > size)*/
    {
        ret_val = size;
        memcpy(buff, read_ptr->contents+off, ret_val);
    }
    
    return ret_val;
}

/**
 * This function must be completed for the assignments!
 * 
 * write() writes (part of, aka a 'chunk' of) a file.
 * - path: file to be written to
 * - buff: buffer with data to write
 * - size: size of the chunk to be written
 * - off: offset from where chunk should start
 * - fi: not used
 */
static int os_write(const char* path, const char* buff, size_t size, off_t off, struct fuse_file_info* fi) {
    //find file to write to 
    file_t* write_ptr = (file_t *)find_file(root_dir,path);
    if ( write_ptr==NULL ){
        return -ENOENT;
    }
    int length = strlen(write_ptr->contents);
    int ret_val;

    if ( length <= size+off)
    {
        ret_val = size;
        write_ptr->contents = (char *)realloc(write_ptr->contents, size+off);
        memcpy(write_ptr->contents+off, buff, ret_val+1);
    }
    else
    /*length > size+off*/
    {
        ret_val = size;
        memcpy(write_ptr->contents+off, buff, ret_val+1);
    }
    
    return ret_val;
}
