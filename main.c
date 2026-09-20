#include <stdio.h>
#include <string.h>
#include "btree.h"


/* =========================================
   문자열 첫 글자를 대문자로 변환
   ========================================= */

static char command_type(char *command)
{
    if (command == NULL)
        return '\0';

    return command[0];
}


/* =========================================
   메인
   ========================================= */

int main()
{
    BTree *tree;
    char input[200];

    /*
       최대 100개의 노드를 저장하는 트리 생성
    */
    tree = create_btree(100);

    if (tree == NULL) {
        printf("트리 생성 실패\n");
        return 1;
    }

    printf("이진트리 프로그램\n");
    printf("명령어를 입력하세요.\n");

    while (1) {

        char command[20];

        printf("\n> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        /*
           줄바꿈 제거
        */
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        /*
           첫 번째 단어를 명령어로 가져온다.
        */
        if (sscanf(input, "%19s", command) != 1)
            continue;


        /* =====================================
           INSERT
           ===================================== */

        if (command_type(command) == 'I') {

            char arg1[100];
            char arg2[20];
            char arg3[20];

            int count =
                sscanf(input,
                       "%*s %99s %19s %19s",
                       arg1, arg2, arg3);

            /*
               Insert / A
               → 루트 생성
            */
            if (count == 2) {

                if (strcmp(arg1, "/") != 0 ||
                    strlen(arg2) != 1 ||
                    arg2[0] < 'A' ||
                    arg2[0] > 'Z') {

                    printf("오류: 잘못된 Insert 형식입니다.\n");
                    continue;
                }

                if (!insert_root(tree, arg2[0])) {
                    printf("오류: 루트를 생성할 수 없습니다.\n");
                }
                else {
                    printf("루트 %c가 생성되었습니다.\n",
                           arg2[0]);
                }
            }

            /*
               Insert /A/B L C
            */
            else if (count == 3) {

                char child;

                if (strlen(arg2) != 1 ||
                    strlen(arg3) != 1) {

                    printf("오류: 잘못된 Insert 형식입니다.\n");
                    continue;
                }

                child = arg2[0];

                if (child != 'L' && child != 'R') {

                    printf("오류: L 또는 R만 사용할 수 있습니다.\n");
                    continue;
                }

                if (arg3[0] < 'A' ||
                    arg3[0] > 'Z') {

                    printf("오류: 데이터는 영문 대문자여야 합니다.\n");
                    continue;
                }

                if (!insert_child(tree,
                                  arg1,
                                  child,
                                  arg3[0])) {

                    printf("오류: 노드를 추가할 수 없습니다.\n");
                }
                else {

                    printf("노드 %c가 추가되었습니다.\n",
                           arg3[0]);
                }
            }

            else {
                printf("오류: Insert 인자가 잘못되었습니다.\n");
            }
        }


        /* =====================================
           DELETE
           ===================================== */

        else if (command_type(command) == 'D') {

            char path[100];

            if (sscanf(input,
                       "%*s %99s",
                       path) != 1) {

                printf("오류: Delete 인자가 없습니다.\n");
                continue;
            }

            if (!delete_node(tree, path)) {

                printf("오류: 삭제할 수 없습니다.\n");
            }
            else {

                printf("노드가 삭제되었습니다.\n");
            }
        }


        /* =====================================
           UPDATE
           ===================================== */

        else if (command_type(command) == 'U') {

            char path[100];
            char value[20];

            if (sscanf(input,
                       "%*s %99s %19s",
                       path, value) != 2) {

                printf("오류: Update 형식이 잘못되었습니다.\n");
                continue;
            }

            if (strlen(value) != 1 ||
                value[0] < 'A' ||
                value[0] > 'Z') {

                printf("오류: 데이터는 영문 대문자여야 합니다.\n");
                continue;
            }

            if (!update_value(tree,
                              path,
                              value[0])) {

                printf("오류: 값을 수정할 수 없습니다.\n");
            }
            else {

                printf("노드 값이 수정되었습니다.\n");
            }
        }


        /* =====================================
           READ
           ===================================== */

        else if (command_type(command) == 'R') {

            char path[100];

            if (sscanf(input,
                       "%*s %99s",
                       path) != 1) {

                printf("오류: Read 인자가 없습니다.\n");
                continue;
            }

            if (!read_child(tree, path)) {

                printf("오류: 존재하지 않는 노드입니다.\n");
            }
        }


        /* =====================================
           PRINT
           ===================================== */

        else if (command_type(command) == 'P') {

            print_btree(tree);
        }


        /* =====================================
           잘못된 명령
           ===================================== */

        else {

            printf("오류: 잘못된 명령입니다.\n");
        }
    }

    destroy_btree(tree);

    return 0;
}
