#include <minizip/miniunz.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (arc < 2) {
      printf("Not enough arguments: %s [files]...", argv[0]);
    }
    void *writer = NULL;
    int32_t err = MZ_OK;

    mz_zip_writer_create(&writer);
    mz_zip_writer_set_password(writer, "mypassword");
    mz_zip_writer_set_compress_method(writer, MZ_COMPRESS_METHOD_DEFLATE);
    mz_zip_writer_set_compress_level(writer, MZ_COMPRESS_LEVEL_DEFAULT);
    err = mz_zip_writer_open_file(writer, "output.zip", 0, 0);
    if (err != MZ_OK) {
        printf("Could not open zip file for writing.");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
      if (access(argv[i], R_OK) == 0) {
        err = mz_zip_writer_add_path(writer, "text1.txt", NULL, 0, 0);
      } else {
        printf("%s: File does not exist)", argv[i]);
      }
    }

    if (err != MZ_OK) {
        printf("Failed to add file to the archive.");
    }
    mz_zip_writer_close(writer);
    mz_zip_writer_delete(&writer);
    return 0;
}
