#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* arc = (char*)malloc((strlen(argv[1])-5)*sizeof(char));
    for (int i = 6; i < strlen(argv[1]); i++) {
        arc[i - 6] = argv[1][i];
    }
    arc[strlen(argv[1]) - 6] = '\0';
    char* create = "--create";
    char* extract = "--extract";
    char* list = "--list";
    if (strcmp(create, argv[2]) == 0) {
        int N = argc - 3;
        FILE* arch;
        arch = fopen(arc, "wb");
        fwrite(&N, 4, 1, arch);
        for (int i = 0; i < N; i++) {
            char* file = argv[i + 3];
            printf("%s", file);
            FILE* pfile;
            pfile = fopen(file, "rb");
            fseek(pfile, 0, SEEK_END);
            int size = ftell(pfile);
            fseek(pfile, 0, SEEK_SET);
            int nameSize = sizeof(char) * strlen(file);
            fwrite(&size, 4, 1, arch);
            printf("%d ", size);
            fwrite(&nameSize, 4, 1, arch);
            printf("%d ", nameSize);
            fwrite(file, nameSize, 1, arch);
            printf("%s ", file);
            char c = 'b';
            for (int j = 0; j < size; j++) {
                fread(&c, 1, 1, pfile);
                printf("%c", c);
                fprintf(arch, "%c", c);
            }
            fclose(pfile);
            remove(file);
        }
        fclose(arch);
    }
    else {
        if (strcmp(extract, argv[2]) == 0) {
            FILE* archive;
            archive = fopen(arc, "rb");
            int M = 0;
            fread(&M, 4, 1, archive);
            printf("%d", M);
            for (int i = 0; i < M; i++) {
                int size = 0;
                fread(&size, 4, 1, archive);
                int nameSize = 0;
                fread(&nameSize, 4, 1, archive);
                printf("%d %d ", size, nameSize);
                char* file = (char*)malloc(nameSize * sizeof(char));
                fread(file, sizeof(char), nameSize, archive);
                file[nameSize * sizeof(char)] = '\0';
                printf(" %s ", file);
                FILE* pfile;
                pfile = fopen(file, "wb");
                char c = 'b';
                for (int j = 0; j < size; j++) {
                    fread(&c, 1, 1, archive);
                    printf("%c", c);
                    fprintf(pfile, "%c", c);
                }
                fclose(pfile);
            }
            fclose(archive);
            remove(arc);
        }
        else {
            FILE* archive;
            archive = fopen(arc, "rb");
            int M = 0;
            fread(&M, 4, 1, archive);
            for
                    (int i = 0; i < M; i++) {
                int size = 0;
                fread(&size, 4, 1, archive);
                int nameSize = 0;
                fread(&nameSize, 4, 1, archive);
                char* file = (char*)malloc(nameSize * sizeof
                        (char));
                fread(file, sizeof
                        (char), nameSize, archive);
                file[nameSize * sizeof
                        (char)] ='\0';
                printf(" %s ", file);
                fseek(archive, size*sizeof
                        (char), SEEK_CUR);
            }
            fclose(archive);
        }
    }
    return 0;
}