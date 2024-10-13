// find.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[128], *p;    //store path
    int fd, fd1;          //now&next fd
    struct dirent de;     //dirent
    struct stat st, st1;  //now&next status of fd

    // attempts to open the specified path file and retrieves its status information
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "path error\n");
        return;
    }//if open

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "path stat failed\n");
        close(fd);
        return;
    }//status of fd

    // performs different actions based on the file type
    switch (st.type)
    {
    case T_FILE:    //file
        fprintf(2, "path error\n");
        return;

    case T_DIR:     //directory
        strcpy(buf, path);      //copy path in buf
        p = buf + strlen(buf);  //make p point to the last position of buf
        *p++ = '/';             //add / to the last position
        while (read(fd, &de, sizeof(de)) == sizeof(de))  //judge if read the whole de
        {
            if (de.inum == 0)  //inode=0,invlid
            {
                continue;
            }
            if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))  //.&..,skip this cycle
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);  //completed path
            if ((fd1 = open(buf, 0)) >= 0)
            {
                if (fstat(fd1, &st1) >= 0)
                {
                    switch (st1.type)
                    {
                    case T_FILE:    //if it is file,compare its name
                        if (!strcmp(de.name, filename))
                        {
                            printf("%s\n", buf);
                        }
                        close(fd1);
                        break;
                    case T_DIR:    //if it is directory,recursive invocation find()
                        close(fd1);
                        find(buf, filename);
                        break;
                    case T_DEVICE:
                        close(fd1);
                        break;
                    }
                }
            }
        }

        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)  //input is wrong
    {
        fprintf(2, "Usage:find path fileName\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}