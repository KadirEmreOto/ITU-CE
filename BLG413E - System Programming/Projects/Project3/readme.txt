Compile command:

gcc tidy_rofs.c -o tidy_rofs -Wall -ansi -W -std=c99 -g -ggdb -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lfuse -ltidy

Run command:

./tidy_rofs <config_file_path> <source_dir> <mount_dir>